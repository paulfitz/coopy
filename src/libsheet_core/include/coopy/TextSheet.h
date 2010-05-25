#ifndef COOPY_TEXTSHEET
#define COOPY_TEXTSHEET

#include <coopy/TypedSheet.h>

class TextSheet : public TypedSheet<std::string> {
public:
  virtual std::string cellString(int x, int y) const {
    return cell(x,y);
  }

  const TextSheet& copy(const TextSheet& alt) {
    arr = alt.arr;
    h = alt.h;
    w = alt.w;
    return *this;
  }
};


#endif
