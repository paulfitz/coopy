#include <coopy/SqliteTextBook.h>
#include <coopy/SqliteSheet.h>
#include <coopy/MergeOutputSqlDiff.h>

#include <sqlite3.h>
#include <stdio.h>

#define WANT_VECTOR2STRING
#include <coopy/Stringer.h>

#include <algorithm>

using namespace std;
using namespace coopy::store;
using namespace coopy::store::sqlite;

#define DB(x) ((sqlite3 *)(x))

SqliteTextBook::SqliteTextBook(bool textual) {
  implementation = NULL;
  this->textual = textual;
}

SqliteTextBook::~SqliteTextBook() {
  clear();
}

void SqliteTextBook::clear() {
  if (implementation!=NULL) {
    sqlite3_close(DB(implementation));
    implementation = NULL;
  }
}

bool SqliteTextBook::read(const char *fname, bool can_create) {
  dbg_printf("SqliteTextBook: reading %s\n", fname);
  clear();

  string alt_fname = fname;
  if (textual) {
    alt_fname = ":memory:";
  }

  int result = sqlite3_open_v2(alt_fname.c_str(),
			       (sqlite3**)(&implementation),
			       SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE|
			       SQLITE_OPEN_NOMUTEX,
			       NULL);
  if (result!=SQLITE_OK) {
    fprintf(stderr,"Failed to open database %s\n", fname);
    clear();
    return false;
  }

  const char *query = "PRAGMA synchronous = 0;";
  sqlite3_exec((sqlite3*)implementation, query, NULL, NULL, NULL);

  if (textual) {
    string txt = "";
    FILE *fin = fopen(fname,"r");
    if (fin==NULL && !can_create) {
	fprintf(stderr,"Failed to read database %s\n", fname);
	clear();
	return false;
    }
    if (fin!=NULL) {
      while (!feof(fin)) {
	char buf[1000];
	int len = fread(buf,1,sizeof(buf),fin);
	if (len<0) { 
	  fprintf(stderr,"Glitch in database %s\n", fname);
	  clear();
	  return false;
	}
	txt += string(buf,len);
      }
      sqlite3_exec((sqlite3*)implementation, txt.c_str(), NULL, NULL, NULL);
      //printf("LOADED: [%s]\n", txt.c_str());
    }
    fclose(fin); fin = NULL;
  }

  names = getNamesSql();
  return true;
}

bool SqliteTextBook::save(const char *fname, const char *format) {
  if (!textual) {
    fprintf(stderr,"Error, tried to save an inplace-database\n");
  }
  sqlite3 *db = DB(implementation);
  if (db==NULL) return false;
  FILE *fout = fopen(fname,"w");
  if (fout==NULL) return false;
  fprintf(fout,"PRAGMA foreign_keys=OFF;\n");
  fprintf(fout,"BEGIN TRANSACTION;\n");


  sqlite3_stmt *statement = NULL;
  string cmd_get_tables = "SELECT name, sql, type FROM sqlite_master;";
  int iresult = sqlite3_prepare_v2(db, cmd_get_tables.c_str(), -1, 
				   &statement, NULL);
  if (iresult!=SQLITE_OK) {
    const char *msg = sqlite3_errmsg(db);
    if (msg!=NULL) {
      fprintf(stderr,"Error: %s\n", msg);
    }
    clear();
    return false;
  }
  while (sqlite3_step(statement) == SQLITE_ROW) {
    char *name = (char *)sqlite3_column_text(statement,0);
    char *sql = (char *)sqlite3_column_text(statement,1);
    char *type = (char *)sqlite3_column_text(statement,2);
    if (sql!=NULL) {
      fprintf(fout,"%s;\n", sql);
    }


    if (string(type)=="table") {
      sqlite3_stmt *statement = NULL;
      char *query = NULL;

      query = sqlite3_mprintf("SELECT * FROM %Q ORDER BY ROWID",
			      name);
      
      int iresult = sqlite3_prepare_v2(db, query, -1, 
				       &statement, NULL);
      if (iresult!=SQLITE_OK) {
	const char *msg = sqlite3_errmsg(db);
	if (msg!=NULL) {
	  fprintf(stderr,"Error: %s\n", msg);
	  fprintf(stderr,"Query was: %s\n", query);
	}
	sqlite3_finalize(statement);
	sqlite3_free(query);
      }

      string prefix = "INSERT INTO ";
      prefix += SqliteSheet::_quoted(name,'\"', true);
      prefix += " VALUES(";
      string postfix = ");";

      int w = sqlite3_column_count(statement);
      while (sqlite3_step(statement) == SQLITE_ROW) {
	string txt = "";
	for (int xx=0; xx<w; xx++) {
	  if (xx>0) txt += ",";
	  int t = sqlite3_column_type(statement,xx);
	  if (t==SQLITE_NULL) {
	    txt += "NULL";
	  } else if (t==SQLITE_INTEGER) {
	    int r = sqlite3_column_int(statement,xx);
	    txt += stringer_encoder(r);
	  } else if (t==SQLITE_FLOAT) {
	    const char *r = (const char *)sqlite3_column_text(statement,xx);
	    txt += r;
	  } else {
	    const char *r = (const char *)sqlite3_column_text(statement,xx);
	    txt += SqliteSheet::_quoted(r,'\'', true);
	  }
	}
	fprintf(fout,"%s%s%s\n",prefix.c_str(),txt.c_str(),postfix.c_str());
        }
      sqlite3_finalize(statement);
      sqlite3_free(query);
    }
  }
  sqlite3_finalize(statement);


  
  fprintf(fout,"COMMIT;\n");
  fclose(fout); fout = NULL;
  return true;
}

bool SqliteTextBook::open(const Property& config) {
  clear();
  if (!config.check("file")) {
    fprintf(stderr,"file parameter needed\n");
    return false;
  }
  if (!read(config.get("file").asString().c_str(),true)) {
    fprintf(stderr,"failed to read %s\n", config.get("file").asString().c_str());
    return false;
  }
  if (!config.check("table")) {
    dbg_printf("Loaded sqlite workbook\n");
    return true;
  }
  names.clear();
  names.push_back(config.get("table").asString().c_str());
  dbg_printf("Loaded sqlite workbook, table: %s\n",
	     vector2string(names).c_str());
  return true;
}


vector<string> SqliteTextBook::getNamesSql() {
  vector<string> result;
  sqlite3 *db = DB(implementation);
  if (db==NULL) return result;

  sqlite3_stmt *statement = NULL;
  string cmd_get_tables = "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;";
  int iresult = sqlite3_prepare_v2(db, cmd_get_tables.c_str(), -1, 
				   &statement, NULL);
  if (iresult!=SQLITE_OK) {
    const char *msg = sqlite3_errmsg(db);
    if (msg!=NULL) {
      fprintf(stderr,"Error: %s\n", msg);
    }
    clear();
    return result;
  }
  while (sqlite3_step(statement) == SQLITE_ROW) {
    char *name = (char *)sqlite3_column_text(statement,0);
    //printf("Got name %s\n", name);
    if (string(name)!="SQLITE_SEQUENCE"&&string(name)!="sqlite_sequence") {
      result.push_back(name);
    }
  }
  sqlite3_finalize(statement);

  return result;
}

PolySheet SqliteTextBook::readSheet(const std::string& name) {
  getNames();
  if (find(names.begin(),names.end(),name)==names.end()) {
    return PolySheet();
  }
  SqliteSheet *sheet = new SqliteSheet(implementation,name.c_str());
  if (sheet!=NULL) sheet->connect();
  return PolySheet(sheet,true);
}

bool SqliteTextBook::addSheet(const SheetSchema& schema) {
  dbg_printf("sqlitetextbook::addsheet %s\n", schema.getSheetName().c_str());
  string name = schema.getSheetName();
  getNames();
  if (find(names.begin(),names.end(),name)!=names.end()) {
    return false;
  }
  sqlite3 *db = DB(implementation);
  if (db==NULL) return false;
  SqliteSheet sheet(db,schema.getSheetName().c_str());
  bool ok = sheet.create(schema);
  names.push_back(name);
  return ok;
}

