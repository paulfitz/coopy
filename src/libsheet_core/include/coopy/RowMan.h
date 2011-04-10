#ifndef COOPY_ROWMAN
#define COOPY_ROWMAN

#include <coopy/Measure.h>
#include <coopy/FMap.h>

namespace coopy {
  namespace cmp {
    template <class map_type> class RowManOf;
    typedef RowManOf<FMap> RowMan;
    typedef RowManOf<FMultiMap> RowMan2;
    class CombinedRowMan;
  }
}

template <class map_type> 
class coopy::cmp::RowManOf : public Measure {
public:
  int vigor;
  int bound;
  map_type m;
  coopy::store::SparseFloatSheet match;

  RowManOf() : m(match) {
    vigor = 0;
    bound = -1;
  }

  void setVigor(int vigor) {
    this->vigor = vigor;
  }

  virtual void setup(MeasurePass& pass) {
    pass.setSize(pass.a.height(),pass.b.height());
  }

  void apply(coopy::store::DataSheet& a, 
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
	    std::string txt = a.cellString(x,y);
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
	    std::string txt = a.cellString(x,y);
	    txt += a.cellString(x+1,y);
	    m.setCurr(x,y);
	    m.add(txt,query,ctrl);
	  }
	}
      }
    }
    m.summarize(true);
  }

  void apply(coopy::store::DataSheet& a, 
	     coopy::store::DataSheet& b, 
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
    return map_type::getCtrlMax();
  }
};

class coopy::cmp::CombinedRowMan : public Measure {
public:
  RowMan man1;
  RowMan2 man2;
  int theta;
  bool flip;

  CombinedRowMan() {
    theta = man1.getCtrlMax()/2;
    flip = false;
  }

  virtual void setup(MeasurePass& pass) {
    man1.setup(pass);
  }

  virtual void measure(MeasurePass& pass, int ctrl) {
    /*
    if (ctrl==theta) {
      int h = pass.asel.height();
      int hits = 0;
      for (int y=0; y<h; y++) {
	if (pass.asel.cell(0,y)!=-1) hits++;
      }
      if (hits<h*0.25) {
	flip = true;
      }
    }
    */
    if (ctrl>=theta) {
      man2.measure(pass,ctrl);
    } else {
      man1.measure(pass,ctrl);
    }
      /*
    if (!flip) {
      man1.measure(pass,ctrl);
    } else {
      man2.measure(pass,ctrl);
    }
      */
  }

  virtual int getCtrlMax() {
    return man1.getCtrlMax();
  }

};

#endif
