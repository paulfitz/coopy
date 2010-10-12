#ifndef COOPY_POLYBOOK
#define COOPY_POLYBOOK

#include <coopy/TextBook.h>
#include <coopy/PolySheet.h>

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

  virtual PolySheet readSheet(const std::string& name) {
    if (book) {
      return book->readSheet(name);
    }
    return PolySheet();
  }

  bool isValid() { return book!=NULL; }

  bool read(const char *fname);

  bool write(const char *fname, const char *format = NULL);
};

#endif

