#ifndef COOPY_ROWMAN
#define COOPY_ROWMAN

#include <coopy/Measure.h>
#include <coopy/FMap.h>

class RowMan : public Measure {
public:
  int vigor;
  int bound;
  FMap m;
  coopy::store::SparseFloatSheet match;

  RowMan() : m(match) {
    vigor = 0;
    bound = -1;
  }

  void setVigor(int vigor) {
    this->vigor = vigor;
  }

  virtual void setup(MeasurePass& pass) {
    pass.setSize(pass.a.height(),pass.b.height());
  }

  void apply(coopy::store::TextSheet& a, 
	     coopy::store::IntSheet& asel, bool query, int ctrl) {
    int w = a.width();
    int h = a.height();
    m.resetCount();
    int top = (bound<0)?h:bound;
    int at = 0;
    for (int y=0; y<h; y++) {
      if (asel.cell(0,y)==-1) {
	if (at<top) {
	  at++;
	  for (int x=0; x<w; x++) {
	    std::string txt = a.cell(x,y);
	    m.setCurr(x,y);
	    m.add(txt,query,ctrl);
	  }
	} else {
	  match.cell(0,y) = -2;
	}
      }
    }
    if (vigor==1) {
      for (int y=0; y<h; y++) {
	if (asel.cell(0,y)>=-1) {
	  for (int x=0; x<w-1; x++) {
	    std::string txt = a.cell(x,y);
	    txt += a.cell(x+1,y);
	    m.setCurr(x,y);
	    m.add(txt,query,ctrl);
	  }
	}
      }
    }
    m.summarize(true);
  }

  void apply(coopy::store::TextSheet& a, 
	     coopy::store::TextSheet& b, 
	     coopy::store::IntSheet& asel, 
	     coopy::store::IntSheet& bsel,
	     int ctrl) {
    match.resize(a.height(),b.height(),0);
    apply(a,asel,false,ctrl);
    apply(b,bsel,true,ctrl);
  }

  virtual void measure(MeasurePass& pass, int ctrl) {
    bound = pass.bound;
    apply(pass.a, pass.b, pass.asel, pass.bsel, ctrl);
    pass.match = match;
  }

  virtual int getCtrlMax() {
    return FMap::getCtrlMax();
  }
};

#endif
