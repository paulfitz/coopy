#ifndef COOPY_GNUMERICTEXTBOOK
#define COOPY_GNUMERICTEXTBOOK

#include <coopy/TextBook.h>

namespace coopy {
  namespace store {
    class GnumericTextBook;
    class GnumericTextBookFactory;
  }
}

class coopy::store::GnumericTextBook : public TextBook {
private:
  void *implementation;
public:
  GnumericTextBook();

  virtual ~GnumericTextBook();

  virtual void clear();
  
  virtual bool load(const char *fname);

  virtual std::vector<std::string> getNames();

  virtual PolySheet readSheet(const std::string& name);
};


#endif
