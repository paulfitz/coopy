#ifndef COOPY_FLOATSHEET
#define COOPY_FLOATSHEET

#include <coopy/TypedSheet.h>
#include <coopy/Stat.h>

#include <stdio.h>

namespace coopy {
  namespace store {
    class FloatSheet;
  }
}

class coopy::store::FloatSheet : public TypedSheet<float> {
public:
  virtual std::string cellString(int x, int y) const {
    char buf[256];
    snprintf(buf,sizeof(buf),"%g",cell(x,y));
    return buf;
  }

  void rescale(float factor) {
    int w = width();
    int h = height();
    for (int x=0; x<w; x++) {
      for (int y=0; y<h; y++) {
	cell(x,y) *= factor;
      }
    }
  }

  Stat normalize(int first=-1, int last=-1, float sc=0.1, bool modify = true);

};



#endif
