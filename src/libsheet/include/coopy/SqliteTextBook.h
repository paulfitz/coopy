#ifndef COOPY_SQLITETEXTBOOK
#define COOPY_SQLITETEXTBOOK

#include <coopy/TextBook.h>

class SqliteTextBook : public TextBook {
public:
  std::vector<std::string> getNames();

  bool readSheet(const std::string& name, TextSheet& sheet);
};

#endif
