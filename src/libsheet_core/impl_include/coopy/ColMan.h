#ifndef COOPY_COLMAN
#define COOPY_COLMAN

#include <coopy/Measure.h>

class ColMan : public Measure {
public:
  const OrderResult& comp;

  ColMan(const OrderResult& comp) : comp(comp) {
  }

  virtual void setup(MeasurePass& pass) {
    pass.setSize(pass.a.width(),pass.b.width());
  }


  void measure(MeasurePass& pass, int ctrl);

  virtual int getCtrlMax() {
    return 5;
  }

};



#endif
