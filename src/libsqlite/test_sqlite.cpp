
#include <coopy/SqliteSheet.h>
#include <coopy/SqliteTextBook.h>

#include <sqlite3.h>

#include <stdio.h>

using namespace coopy::store;
using namespace coopy::store::sqlite;
using namespace std;

int main(int argc, char *argv[]) {
  if (argc<2) {
    printf("Call with database_filename.db [table]\n");
  }
  SqliteTextBook book;
  book.read(argv[1]);
  vector<string> names = book.getNames();
  PolySheet sheet;
  if (argc>2) {
    sheet = book.readSheet(argv[2]);
  } else {
    sheet = book.readSheet(names[0]);
  }

  for (int i=0; i<sheet.height(); i++) {
    for (int j=0; j<sheet.width(); j++) {
      printf("\"%s\" ", sheet.cellString(j,i).c_str());
    }
    printf("\n");
  }

  string tmp = sheet.cellString(2,1);
  sheet.cellString(2,1,sheet.cellString(2,0));
  sheet.cellString(2,0,tmp);

  printf("===\n");
  for (int i=0; i<sheet.height(); i++) {
    for (int j=0; j<sheet.width(); j++) {
      printf("\"%s\" ", sheet.cellString(j,i).c_str());
    }
    printf("\n");
  }
  return 1;
}

