#ifndef COOPY_FILTEREDTEXTBOOK
#define COOPY_FILTEREDTEXTBOOK

#include <coopy/PolyBook.h>

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
class coopy::store::FilteredTextBook : public PolyBook {
public:
  std::vector<std::string> names;
  PolyBook orig;

  FilteredTextBook(TextBook *book, const  std::vector<std::string>& names) {
    this->names = names;
    take(book);
  }

  virtual std::vector<std::string> getNames() {
    return names;
  }
};

#endif
