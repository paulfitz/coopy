#include <coopy/Dbg.h>

#include <coopy/SqliteSheet.h>

//extern "C" {
//#include <coopy/gnumeric_link.h>
//}

using namespace coopy::store;

using namespace std;

//#define SHEET(x) ((SqliteSheetPtr)(x))

SqliteSheet::SqliteSheet(void *db, const char *name) {
  implementation = db;
  this->name = name;
  w = h = 0;
}

SqliteSheet::~SqliteSheet() {
}

std::string SqliteSheet::cellString(int x, int y) const {
  return "";
}

bool SqliteSheet::cellString(int x, int y, const std::string& str) {
  return false;
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
