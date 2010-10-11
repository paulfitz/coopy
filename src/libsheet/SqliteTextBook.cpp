#include <coopy/SqliteTextBook.h>

#include "sqlite3.h"

using namespace std;
using namespace coopy::store;

vector<string> SqliteTextBook::getNames() {
  vector<string> result;
  return result;
}

PolySheet SqliteTextBook::readSheet(const string& name) {
  return PolySheet();
}
