#ifndef COOPY_POLYBOOK
#define COOPY_POLYBOOK

#include <coopy/TextBook.h>
#include <coopy/PolySheet.h>
#include <coopy/Property.h>

namespace coopy {
  namespace store {
    class PolyBook;
  }
}

class coopy::store::PolyBook : public TextBook {
private:
  TextBook *book;
  coopy::store::Property options;
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

  void take(TextBook *book) {
    clear();
    this->book = book;
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

  bool read(const char *fname) {
    coopy::store::Property p;
    p.put("file",fname);
    p.put("can_create",false);
    p.put("should_read",true);
    p.put("should_write",false);
    return attach(p);
  }

  bool attach(const char *fname) {
    coopy::store::Property p;
    p.put("file",fname);
    p.put("can_create",true);
    p.put("should_read",true);
    p.put("should_attach",true);
    p.put("should_write",false);
    return attach(p);
  }

  bool write(const char *fname) {
    coopy::store::Property p;
    p.put("file",fname);
    p.put("can_create",true);
    p.put("should_read",false);
    p.put("should_write",true);
    return attach(p);
  }

  bool attach(coopy::store::Property& config);

  bool flush();

  bool inplace() const {
    if (book) {
      return book->inplace();
    }
    return false;
  }

  virtual bool copy(const DataBook& alt, const Property& options) {
    if (book)
      return book->copy(alt,options);
    return false;
  }

  virtual bool addSheet(const SheetSchema& schema) {
    if (book)
      return book->addSheet(schema);
    return false;
  }

  virtual PolySheet provideSheet(const SheetSchema& schema) {
    if (book)
      return book->provideSheet(schema);
    return PolySheet();
  }

  virtual std::string desc() const {
    if (book) {
      return book->desc();
    }
    return "PolyBook";
  }

  virtual DataBook& tail() {
    if (book) {
      return book->tail();
    }
    return *this;
  }


  static void showFormats();
};

#endif

