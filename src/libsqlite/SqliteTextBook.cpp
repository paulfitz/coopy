#include <coopy/SqliteTextBook.h>
#include <coopy/SqliteSheet.h>

#include <sqlite3.h>
#include <stdio.h>

#define WANT_VECTOR2STRING
#include <coopy/Stringer.h>

#include <algorithm>

using namespace std;
using namespace coopy::store;

#define DB(x) ((sqlite3 *)(x))

SqliteTextBook::SqliteTextBook() {
  implementation = NULL;
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

bool SqliteTextBook::read(const char *fname) {
  clear();
  int result = sqlite3_open_v2(fname,
			       (sqlite3**)(&implementation),
			       SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE|
			       SQLITE_OPEN_NOMUTEX,
			       NULL);
  if (result!=SQLITE_OK) {
    fprintf(stderr,"Failed to open database %s\n", fname);
    clear();
    return false;
  }  
  names = getNamesSql();
  return true;
}

bool SqliteTextBook::open(const Property& config) {
  clear();
  if (!config.check("file")) {
    fprintf(stderr,"file parameter needed\n");
    return false;
  }
  if (!read(config.get("file").asString().c_str())) {
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
    result.push_back(name);
  }
  sqlite3_finalize(statement);

  return result;
}

PolySheet SqliteTextBook::readSheet(const string& name) {
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

