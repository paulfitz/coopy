#ifndef COOPY_POLYBOOK
#define COOPY_POLYBOOK

#include <coopy/TextBook.h>

namespace coopy {
  namespace store {
    class PolyBook;
  }
}

class coopy::store::PolyBook : public TextBook {
private:
  TextBook *book;
public:
  PolyBook() {
    book = NULL;
  }

  virtual ~PolyBook() {
    clear();
  }

  void clear() {
    if (book!=NULL) {
      delete book;
      book = NULL;
    }
  }

  virtual std::vector<std::string> getNames() {
    if (book) {
      return book->getNames();
    }
    return std::vector<std::string>();
  }

  virtual bool readSheet(const std::string& name, TextSheet& sheet) {
    if (book) {
      return book->readSheet(name,sheet);
    }
    return false;
  }

  bool isValid() { return book!=NULL; }

  bool read(const char *fname);

  bool write(const char *fname);
};

#endif

