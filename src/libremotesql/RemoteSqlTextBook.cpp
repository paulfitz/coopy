#include <coopy/RemoteSqlTextBook.h>
#include <stdio.h>

using namespace coopy::store;
using namespace std;

RemoteSqlTextBook::RemoteSqlTextBook() {
  implementation = NULL;
}

RemoteSqlTextBook::~RemoteSqlTextBook() {
}

void RemoteSqlTextBook::clear() {
}

bool RemoteSqlTextBook::read(const Property& config) {
  printf("RemoteSqlTextBook is a stub\n");
  return false;
}

std::vector<std::string> RemoteSqlTextBook::getNames() {
  return vector<string>();
}

PolySheet RemoteSqlTextBook::readSheet(const std::string& name) {
  return PolySheet();
}
