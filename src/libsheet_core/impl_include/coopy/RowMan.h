#ifndef COOPY_ROWMAN
#define COOPY_ROWMAN

#include <coopy/RowMan.h>

class RowMan : public Measure {
public:
  int vigor;
  FMap m;
  FloatSheet match;

  RowMan() : m(match) {
    vigor = 0;
  }

  void setVigor(int vigor) {
    this->vigor = vigor;
  }

  virtual void setup(MeasurePass& pass) {
    pass.setSize(pass.a.height(),pass.b.height());
  }

  void apply(CsvSheet& a, IntSheet& asel, bool query, int ctrl) {
    int w = a.width();
    int h = a.height();
    m.resetCount();
    for (int y=0; y<h; y++) {
      if (asel.cell(0,y)==-1) {
	for (int x=0; x<w; x++) {
	  string txt = a.cell(x,y);
	  m.setCurr(x,y);
	  m.add(txt,query,ctrl);
	}
      }
    }
    if (vigor==1) {
      for (int y=0; y<h; y++) {
	if (asel.cell(0,y)==-1) {
	  for (int x=0; x<w-1; x++) {
	    string txt = a.cell(x,y);
	    txt += a.cell(x+1,y);
	    m.setCurr(x,y);
	    m.add(txt,query,ctrl);
	  }
	}
      }
    }
    m.summarize(true);
  }

  void apply(CsvSheet& a, CsvSheet& b, IntSheet& asel, IntSheet& bsel,
	     int ctrl) {
    match.resize(a.height(),b.height(),0);
    apply(a,asel,false,ctrl);
    apply(b,bsel,true,ctrl);
  }

  virtual void measure(MeasurePass& pass, int ctrl) {
    apply(pass.a, pass.b, pass.asel, pass.bsel, ctrl);
    pass.match = match;
  }

  virtual int getCtrlMax() {
    return FMap::getCtrlMax();
  }
};

#endif
