#include <coopy/RemoteSqlSheet.h>

using namespace std;
using namespace coopy::store;

RemoteSqlSheet::RemoteSqlSheet() {
  implementation = NULL;
  w = h = 0;
}

RemoteSqlSheet::~RemoteSqlSheet() {
}

bool RemoteSqlSheet::open(const Property& config) {
  printf("RemoteSqlSheet is a stub\n");
  return false;
}

std::string RemoteSqlSheet::cellString(int x, int y) const {
  return "";
}

bool RemoteSqlSheet::cellString(int x, int y, const std::string& str) {
  return false;
}


ColumnRef RemoteSqlSheet::moveColumn(const ColumnRef& src, 
				     const ColumnRef& base) {
  return ColumnRef();
}

bool RemoteSqlSheet::deleteColumn(const ColumnRef& column) {
  return false;
}

ColumnRef RemoteSqlSheet::insertColumn(const ColumnRef& base) {
  return ColumnRef();
}

RowRef RemoteSqlSheet::insertRow(const RowRef& base) {
  return RowRef();
}

bool RemoteSqlSheet::deleteRow(const RowRef& src) {
  return false;
}

