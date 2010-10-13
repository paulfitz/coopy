
#include <coopy/SqliteSheet.h>
#include <coopy/SqliteTextBook.h>

#include <sqlite3.h>

#include <stdio.h>

using namespace coopy::store;
using namespace std;

int main() {
  const char *filename = "test.db";
  sqlite3 *db = NULL;
  int result = sqlite3_open_v2(filename,
			       &db,
			       SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE|
			       SQLITE_OPEN_NOMUTEX,
			       NULL);
  if (result!=SQLITE_OK) {
    fprintf(stderr,"Failed to open database %s\n", filename);
    if (db!=NULL) {
      sqlite3_close(db);
    }
    return 1;
  }


  if (db!=NULL) {
    sqlite3_close(db);
  }

  SqliteTextBook book;
  book.read("test.db");
  vector<string> names = book.getNames();

  return 1;
}

