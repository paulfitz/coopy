#ifndef COOPY_FILTEREDTEXTBOOK
#define COOPY_FILTEREDTEXTBOOK

#include <coopy/TextBook.h>
#include <coopy/PolyBook.h>
#include <coopy/TextBookFactory.h>
#include <coopy/Dbg.h>

namespace coopy {
  namespace store {
    class FilteredTextBook;
  }
}

/**
 *
 * View limited set of sheets of a book.
 *
 */
class coopy::store::FilteredTextBook : public TextBook {
public:
  std::vector<std::string> names;
  PolyBook orig;

  FilteredTextBook(TextBook *book, const  std::vector<std::string>& names) {
    this->names = names;
    orig.take(book);
  }

  virtual std::vector<std::string> getNames() {
    return names;
  }

  virtual PolySheet readSheet(const std::string& name) {
    return orig.readSheet(name);
  }

  virtual bool open(const Property& config) {
    return orig.open(config);
  }

  virtual PolySheet provideSheet(const SheetSchema& schema) {
    return orig.provideSheet(schema);
  }

  virtual std::string desc() const {
    return orig.desc();
  }

  virtual bool namedSheets() const {
    return orig.namedSheets();
  }
};

#endif
