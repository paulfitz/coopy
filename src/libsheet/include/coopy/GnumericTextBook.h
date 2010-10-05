#ifndef COOPY_GNUMERICTEXTBOOK
#define COOPY_GNUMERICTEXTBOOK

#include <coopy/TextBook.h>

namespace coopy {
  namespace store {
    class GnumericTextBook;
  }
}

class coopy::store::GnumericTextBook : public TextBook {
private:
  void *implementation;
public:

  std::vector<std::string> getNames();

  bool readSheet(const std::string& name, TextSheet& sheet);
};

#endif
