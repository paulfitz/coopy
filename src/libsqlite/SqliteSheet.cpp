#include <coopy/Dbg.h>

#include <coopy/SqliteSheet.h>

#include <sqlite3.h>

using namespace coopy::store;
using namespace std;

#define DB(x) ((sqlite3 *)(x))

SqliteSheet::SqliteSheet(void *db1, const char *name) {
  implementation = db1;
  this->name = name;
  w = h = 0;

  sqlite3 *db = DB(implementation);
  if (db==NULL) return;

  sqlite3_stmt *statement = NULL;
  char *query = NULL;

  
  //////////////////////////////////////////////////////////////////
  // Check dimensions

  query = sqlite3_mprintf("SELECT COUNT(*), * FROM %Q",
			  this->name.c_str());
 
  int iresult = sqlite3_prepare_v2(db, query, -1, 
				   &statement, NULL);
  if (iresult!=SQLITE_OK) {
    const char *msg = sqlite3_errmsg(db);
    if (msg!=NULL) {
      fprintf(stderr,"Error: %s\n", msg);
    }
    sqlite3_finalize(statement);
    sqlite3_free(query);
  } 
  if (sqlite3_step(statement) == SQLITE_ROW) {
    h = sqlite3_column_int(statement,0);
    w = sqlite3_column_count(statement)-1;
  }
  //printf("Dimensions %dx%d\n", w, h);
  sqlite3_finalize(statement);
  sqlite3_free(query);


  //////////////////////////////////////////////////////////////////
  // Check ROWIDs

  query = sqlite3_mprintf("SELECT ROWID FROM %Q ORDER BY ROWID",
			  this->name.c_str());

  iresult = sqlite3_prepare_v2(db, query, -1, 
			       &statement, NULL);
  if (iresult!=SQLITE_OK) {
    const char *msg = sqlite3_errmsg(db);
    if (msg!=NULL) {
      fprintf(stderr,"Error: %s\n", msg);
    }
    sqlite3_finalize(statement);
    sqlite3_free(query);
  } 

  while (sqlite3_step(statement) == SQLITE_ROW) {
    row2sql.push_back(sqlite3_column_int(statement,0));
  }
  sqlite3_finalize(statement);
  sqlite3_free(query);


  //////////////////////////////////////////////////////////////////
  // Check column names

  query = sqlite3_mprintf("PRAGMA table_info(%Q)",
			  this->name.c_str());

  iresult = sqlite3_prepare_v2(db, query, -1, 
			       &statement, NULL);
  if (iresult!=SQLITE_OK) {
    const char *msg = sqlite3_errmsg(db);
    if (msg!=NULL) {
      fprintf(stderr,"Error: %s\n", msg);
    }
    sqlite3_finalize(statement);
    sqlite3_free(query);
  } 

  while (sqlite3_step(statement) == SQLITE_ROW) {
    col2sql.push_back((const char *)sqlite3_column_text(statement,1));
  }
  sqlite3_finalize(statement);
  sqlite3_free(query);

}

SqliteSheet::~SqliteSheet() {
}

std::string SqliteSheet::cellString(int x, int y) const {
  // starting with a COMPLETELY brain-dead implementation

  sqlite3 *db = DB(implementation);
  if (db==NULL) return "";

  sqlite3_stmt *statement = NULL;
  char *query = NULL;

  query = sqlite3_mprintf("SELECT * FROM %Q WHERE ROWID = %d", 
			  name.c_str(),
			  row2sql[y]);
 
  int iresult = sqlite3_prepare_v2(db, query, -1, 
				   &statement, NULL);
  if (iresult!=SQLITE_OK) {
    sqlite3_finalize(statement);
    sqlite3_free(query);
    return "";
  }
  string result = "";
  if (sqlite3_step(statement) == SQLITE_ROW) {
    char *txt = (char *)sqlite3_column_text(statement,x);
    //printf("Got field %s\n", txt);
    result = txt;
  }
  sqlite3_finalize(statement);
  sqlite3_free(query);
  
  return result;
}

bool SqliteSheet::cellString(int x, int y, const std::string& str) {
  // starting with a COMPLETELY brain-dead implementation

  sqlite3 *db = DB(implementation);
  if (db==NULL) return false;

  sqlite3_stmt *statement = NULL;
  char *query = NULL;

  query = sqlite3_mprintf("UPDATE %Q SET %Q = %Q WHERE ROWID = %d", 
			  name.c_str(),
			  col2sql[x].c_str(),
			  str.c_str(),
			  row2sql[y]);

  int iresult = sqlite3_exec(db, query, NULL, NULL, NULL);
  if (iresult!=SQLITE_OK) {
    const char *msg = sqlite3_errmsg(db);
    if (msg!=NULL) {
      fprintf(stderr,"Error: %s\n", msg);
    }
    sqlite3_free(query);
    return false;
  }
  sqlite3_free(query);
  
  return true;
}


ColumnRef SqliteSheet::moveColumn(const ColumnRef& src, 
				    const ColumnRef& base) {
  return ColumnRef();
}


bool SqliteSheet::deleteColumn(const ColumnRef& column) {
  return false;
}

ColumnRef SqliteSheet::insertColumn(const ColumnRef& base) {
  return ColumnRef(-1);
}

RowRef SqliteSheet::insertRow(const RowRef& base) {
  return RowRef(-1);  
}

bool SqliteSheet::deleteRow(const RowRef& src) {
  return false;
}
