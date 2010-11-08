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

  virtual bool open(const Property& config);
  
  std::vector<std::string> getNames() {
    return names;
  }

  PolySheet readSheet(const std::string& name);

  virtual bool inplace() const {
    return true;
  }

private:
  void *implementation;
  bool tableSet;
  std::vector<std::string> names;

  std::vector<std::string> getNamesSql();

};

#endif
