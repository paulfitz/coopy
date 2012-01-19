#ifndef COOPY_POLYBOOK
#define COOPY_POLYBOOK

#include <coopy/TextBook.h>
#include <coopy/PolySheet.h>
#include <coopy/Property.h>
#include <coopy/FormatDesc.h>

namespace coopy {
  namespace store {
    class PolyBook;
  }
}

/**
 *
 * Container for a workbook (collection of tables) backed by any kind
 * of spreadsheet or database.
 *
 */
class coopy::store::PolyBook : public TextBook {
private:
  TextBook *book;
  coopy::store::Property options;
public:
  PolyBook() {
    book = 0/*NULL*/;
  }

  virtual ~PolyBook() {
    clear();
  }

  void clear() {
    if (book!=0/*NULL*/) {
      int ref = book->removeReference();
      if (ref==0) {
	delete book;
      }
      book = 0/*NULL*/;
    }
  }

  void take(TextBook *book) {
    clear();
    this->book = book;
    if (book!=0) {
      book->addReference();
    }
  }

  PolyBook(const PolyBook& alt) {
    book = alt.book;
    if (book!=0) {
      book->addReference();
    }
  }

  const PolyBook& operator=(const PolyBook& alt) {
    clear();
    book = alt.book;
    if (book!=0) {
      book->addReference();
    }
  }

  virtual std::vector<std::string> getNames() {
    if (book) {
      return book->getNames();
    }
    return std::vector<std::string>();
  }

  virtual int getSheetCount() {
    return (int)getNames().size();
  }

  virtual PolySheet readSheet(const std::string& name) {
    if (book) {
      return book->readSheet(name);
    }
    return PolySheet();
  }

  bool isValid() const { return book!=NULL; }

  bool read(const char *fname, const char *ext = NULL) {
    coopy::store::Property p;
    p.put("file",fname);
    if (ext!=NULL) {
      if (ext!="") {
	p.put("ext",ext);
      }
    }
    p.put("can_create",false);
    p.put("should_read",true);
    p.put("should_write",false);
    return attach(p);
  }

  bool attach(const char *fname, const char *ext = NULL) {
    coopy::store::Property p;
    p.put("file",fname);
    if (ext!=NULL) {
      if (ext!="") {
	p.put("ext",ext);
      }
    }
    p.put("can_create",true);
    p.put("should_read",true);
    p.put("should_attach",true);
    p.put("should_write",false);
    return attach(p);
  }

  bool attachReadWrite(const char *fname) {
    coopy::store::Property p;
    p.put("file",fname);
    p.put("can_create",true);
    p.put("should_read",true);
    p.put("should_attach",true);
    p.put("should_write",false);
    p.put("attach_read",true);
    return attach(p);
  }

  bool write(const char *fname, const char *ext = NULL) {
    coopy::store::Property p;
    p.put("file",fname);
    if (ext!=NULL) {
      if (ext!="") {
	p.put("ext",ext);
      }
    }
    p.put("can_create",true);
    p.put("should_read",false);
    p.put("should_write",true);
    return attach(p);
  }

  bool attach(coopy::store::Property& config);

  bool expand(coopy::store::Property& config);

  coopy::store::Property getType(const char *fname, const char *ext = NULL) {
    coopy::store::Property p;
    p.put("file",fname);
    if (ext!=NULL) {
      if (ext!="") {
	p.put("ext",ext);
      }
    }
    expand(p);
    return p;
  }

  bool flush();

  bool inplace() const {
    if (book) {
      return book->inplace();
    }
    return false;
  }

  virtual bool canWrite() const { 
    if (book) {
      return book->inplace();
    }
    return false;
  }


  virtual bool copy(const TextBook& alt, const Property& options);

  virtual bool addSheet(const SheetSchema& schema) {
    if (book)
      return book->addSheet(schema);
    return false;
  }

  virtual bool namedSheets() const {
    if (book)
      return book->namedSheets();
    return true;
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

  virtual TextBook& tail() {
    if (book) {
      return book->tail();
    }
    return *this;
  }

  static void showFormats();

  static std::vector<coopy::store::FormatDesc> getFormatList();

  static bool copyFile(const char *src, const char *dest);

  std::string toString() {
    std::string result;
    std::vector<std::string> names = getNames();
    for (int i=0; i<getSheetCount(); i++) {
      result += names[i];
      result += ":\n";
      result += readSheet(names[i]).toString();
      result += "\n";
      return result;
    }
  }

  virtual bool setPool(Pool *pool) {
    if (book)
      return book->setPool(pool);
    return false;
  }

  virtual Pool *getPool() const {
    if (book)
      return book->getPool();
    return 0 /*NULL*/;
  }

};

#endif

