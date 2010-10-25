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

private:
  void *implementation;
};

#endif
