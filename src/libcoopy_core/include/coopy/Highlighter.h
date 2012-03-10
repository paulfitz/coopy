#ifndef COOPY_HIGHLIGHTER
#define COOPY_HIGHLIGHTER

#include <coopy/DataSheet.h>
#include <coopy/TextBook.h>

namespace coopy {
  namespace store {
    class Highlighter;
  }
}

class coopy::store::Highlighter {
public:
  bool apply(DataSheet& sheet);
  bool apply(TextBook& book);
};

#endif

