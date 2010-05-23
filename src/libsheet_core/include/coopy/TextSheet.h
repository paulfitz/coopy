#ifndef COOPY_TEXTSHEET
#define COOPY_TEXTSHEET

#include <coopy/TypedSheet.h>

class TextSheet : public TypedSheet<std::string> {
public:
  virtual std::string cellString(int x, int y) const {
    return cell(x,y);
  }
};


#endif
