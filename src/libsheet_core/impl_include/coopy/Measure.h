#ifndef COOPY_MEASURE
#define COOPY_MEASURE

#include <coopy/MeasurePass.h>

namespace coopy {
  namespace cmp {
    class Measure;
  }
}


class coopy::cmp::Measure {
public:
  virtual void setup(MeasurePass& pass) = 0;
  virtual void measure(MeasurePass& pass, int ctrl=0) = 0;

  virtual int getCtrlMax() = 0;
};

#endif
