#ifndef COOPY_SQLITETEXTBOOK
#define COOPY_SQLITETEXTBOOK

#include <coopy/TextBook.h>

namespace coopy {
  namespace store {
    class SqliteTextBook;
  }
}

class coopy::store::SqliteTextBook : public TextBook {
public:
  SqliteTextBook();
  virtual ~SqliteTextBook();

  void clear();

  bool read(const char *fname);

  std::vector<std::string> getNames();

  PolySheet readSheet(const std::string& name);

private:
  void *implementation;
};

#endif
