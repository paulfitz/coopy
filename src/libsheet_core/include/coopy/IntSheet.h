#ifndef COOPY_INTSHEET
#define COOPY_INTSHEET

#include <coopy/TypedSheet.h>

#include <stdio.h>

namespace coopy {
  namespace store {
    class IntSheet;
  }
}

class coopy::store::IntSheet : public TypedSheet<int> {
public:
  static std::string int2string(int v) {
    char buf[256];
    snprintf(buf,sizeof(buf),"%d",v);
    return buf;
  }

  virtual std::string cellString(int x, int y) const {
    return int2string(cell(x,y));
  }
};


#endif
