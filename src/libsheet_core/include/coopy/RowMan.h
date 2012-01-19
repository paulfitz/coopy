#ifndef COOPY_ROWMAN
#define COOPY_ROWMAN

#include <coopy/Measure.h>
#include <coopy/CompareFlags.h>
#include <coopy/FMap.h>

namespace coopy {
  namespace cmp {
    template <class map_type> class RowManOf;
    typedef RowManOf<FMap> RowMan;
    typedef RowManOf<FMultiMap> RowMan2;
    class CombinedRowMan;
  }
}

/**
 * Compare rows of a table.
 *
 */
template <class map_type> 
class coopy::cmp::RowManOf : public Measure {
public:
  CompareFlags flags;
  int vigor;
  int bound;
  map_type m;
  std::vector<int> ref_subset, query_subset;
  coopy::store::SparseFloatSheet match;

  RowManOf(const CompareFlags& flags) : flags(flags), m(match) {
    vigor = 0;
    bound = -1;
  }

  void setVigor(int vigor) {
    this->vigor = vigor;
  }

  virtual void setup(MeasurePass& pass) {
    pass.setSize(pass.a.height(),pass.b.height());
    if (flags.trust_ids||flags.bias_ids) {
      ref_subset = pass.va.meta.getSubset();
      query_subset = pass.vb.meta.getSubset();
    }
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
	  if (!(flags.trust_ids)) {
	    if (!flags.bias_ids) {
	      for (int x=0; x<w; x++) {
		std::string txt = a.cellString(x,y);
		m.setCurr(x,y);
		m.add(txt,query,ctrl);
	      }
	    } else {
	      const std::vector<int>& subset = query?query_subset:ref_subset;
	      for (int x=0; x<(int)subset.size(); x++) {
		std::string txt = a.cellString(subset[x],y);
		m.setCurr(subset[x],y);
		m.add(txt,query,ctrl);
	      }
	    }
	  } else {
	    const std::vector<int>& subset = query?query_subset:ref_subset;
	    std::string txt = "";
	    for (int x=0; x<(int)subset.size(); x++) {
	      txt += a.cellString(subset[x],y) + "__";
	    }
	    m.setCurr(0,y);
	    m.add(txt,query,0);
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

/**
 * Compare rows of a table using multiple methods as appropriate.
 *
 */
class coopy::cmp::CombinedRowMan : public Measure {
public:
  RowMan man1;
  RowMan2 man2;
  int theta;
  bool flip;

 CombinedRowMan(const CompareFlags& flags) : man1(flags), man2(flags) {
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
      man2.measure(pass,ctrl-theta);
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
