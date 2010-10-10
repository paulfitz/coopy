#ifndef COOPY_TEXTSHEET
#define COOPY_TEXTSHEET

#include <coopy/TypedSheet.h>

namespace coopy {
  namespace store {
    class TextSheet;
  }
}

class coopy::store::TextSheet : public TypedSheet<std::string> {
public:
  virtual std::string cellString(int x, int y) const {
    return cell(x,y);
  }

  virtual bool cellString(int x, int y, const std::string& str) {
    cell(x,y) = str;
    return true;
  }


  const TextSheet& copy(const TextSheet& alt) {
    arr = alt.arr;
    h = alt.h;
    w = alt.w;
    return *this;
  }
};


#endif
