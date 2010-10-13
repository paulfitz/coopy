#include <coopy/SqliteTextBook.h>
#include <coopy/SqliteSheet.h>

#include <sqlite3.h>
#include <stdio.h>

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
  return true;
}

vector<string> SqliteTextBook::getNames() {
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
    printf("Got name %s\n", name);
    result.push_back(name);
  }
  sqlite3_finalize(statement);

  return result;
}

PolySheet SqliteTextBook::readSheet(const string& name) {
  SqliteSheet *sheet = new SqliteSheet(implementation,name.c_str());
  return PolySheet(sheet,true);
}
