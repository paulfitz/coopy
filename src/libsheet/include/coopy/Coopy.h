#ifndef COOPY_COOPY_H
#define COOPY_COOPY_H

#include <coopy/PolySheet.h>
#include <coopy/PolyBook.h>
#include <coopy/Patcher.h>
#include <coopy/WrapBook.h>
#include <coopy/Patcher.h>
#include <coopy/CompareFlags.h>

namespace coopy {
  namespace app {
    class Coopy;
  }
}

/**
 * Collect basic Coopy functionality in one place.
 * Intended to simplify wrappers for the library in other languages.
 * 
 */
class coopy::app::Coopy {
private:
  coopy::cmp::Patcher *patcher;
  std::string output;
  coopy::store::TextBook *output_book;
  coopy::store::WrapBook output_sheet;
public:
  coopy::cmp::CompareFlags flags;

  Coopy() {
    patcher = 0/*NULL*/;
    output = "-";
    output_book = 0/*NULL*/;
  }

  virtual ~Coopy() {
    reset();
  }

  void reset() {
    if (patcher!=0/*NULL*/) {
      delete patcher;
      patcher = 0/*NULL*/;
    }
  }

  bool setFormat(const char *name, const char *version = NULL) {
    reset();
    patcher = coopy::cmp::Patcher::createByName(name,version);
    return patcher!=0/*NULL*/;
  }

  bool checkFormat() {
    if (patcher) return true;
    return setFormat("tdiff");
  }

  bool setOutput(const char *filename) {
    output = filename;
    return true;
  }

  bool setOutput(coopy::store::DataSheet& sheet) {
    output_sheet.attach(sheet,false);
    output_book = &output_sheet;
    return true;
  }

  bool setOutput(coopy::store::TextBook& book) {
    output_book = &book;
    return true;
  }

  bool compare(coopy::store::TextBook& pivot, 
	       coopy::store::TextBook& local, 
	       coopy::store::TextBook& remote);

  bool compare(coopy::store::TextBook& local, 
	       coopy::store::TextBook& remote) {
    return compare(local,local,remote);
  }

  bool compare(coopy::store::DataSheet& pivot, 
	       coopy::store::DataSheet& local, 
	       coopy::store::DataSheet& remote) {
    coopy::store::WrapBook book1(pivot,false);
    coopy::store::WrapBook book2(local,false);
    coopy::store::WrapBook book3(remote,false);
    return compare(book1,book2,book3);
  }

  bool compare(coopy::store::DataSheet& local, 
	       coopy::store::DataSheet& remote) {
    return compare(local,local,remote);
  }

  bool merge(coopy::store::DataSheet& pivot, 
	     coopy::store::DataSheet& local, 
	     coopy::store::DataSheet& remote) {
    coopy::store::WrapBook book1(pivot,false);
    coopy::store::WrapBook book2(local,false);
    coopy::store::WrapBook book3(remote,false);
    return merge(book1,book2,book3);
  }

  bool merge(coopy::store::TextBook& pivot, 
	     coopy::store::TextBook& local, 
	     coopy::store::TextBook& remote);

  bool patch(coopy::store::TextBook& local, 
	     coopy::store::TextBook& mod);

  bool patch(coopy::store::DataSheet& local, 
	     coopy::store::DataSheet& mod) {
    coopy::store::WrapBook book2(local,false);
    coopy::store::WrapBook book3(mod,false);
    return patch(book2,book3);
  }

  int getChangeCount() const {
    if (!patcher) return 0;
    return patcher->getChangeCount();
  }

  static coopy::store::PolySheet createSheet();

  static coopy::store::PolyBook createBook(coopy::store::PolySheet sheet);
};


#endif
