#ifndef COOPY_REMOTESQLTEXTBOOK
#define COOPY_REMOTESQLTEXTBOOK

#include <coopy/TextBook.h>
#include <coopy/Property.h>

namespace coopy {
  namespace store {
    class RemoteSqlTextBook;
  }
}

class coopy::store::RemoteSqlTextBook : public TextBook {
public:
  RemoteSqlTextBook();
  virtual ~RemoteSqlTextBook();

  void clear();

  bool read(const Property& config);

  std::vector<std::string> getNames();

  PolySheet readSheet(const std::string& name);

  virtual bool inplace() const {
    return true;
  }

  void *getSqlInterface() {
    return implementation;
  }

  std::string getDatabaseName() {
    return database_name;
  }

private:
  void *implementation;
  std::string database_name;
  std::vector<std::string> names_cache;
  bool dirty;
};

#endif
