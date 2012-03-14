#ifndef COOPY_ROWMAN
#define COOPY_ROWMAN

#include <coopy/Measure.h>
#include <coopy/CompareFlags.h>
#include <coopy/FMap.h>
#include <coopy/OrderResult.h>

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
  const OrderResult& comp;

 RowManOf(const CompareFlags& flags,
	  const OrderResult& comp,
	  int len) : flags(flags), m(match,len), comp(comp) {
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
	     coopy::store::IntSheet& asel, 
	     int target,
	     bool query, 
	     bool alt, int ctrl) {
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
	      int first = target;
	      int last = target;
	      if (target==-1) {
		first = 0;
		last = w-1;
	      }
	      for (int x=first; x<=last; x++) {
		std::string txt = a.cellString(x,y);
		m.setCurr(x,y);
		m.add(txt,query,alt,ctrl);
		//printf("ADD %d %d %s %d\n", x, y, txt.c_str(), query);
	      }
	    } else {
	      const std::vector<int>& subset = query?query_subset:ref_subset;
	      for (int x=0; x<(int)subset.size(); x++) {
		std::string txt = a.cellString(subset[x],y);
		m.setCurr(subset[x],y);
		m.add(txt,query,alt,ctrl);
	      }
	    }
	  } else {
	    const std::vector<int>& subset = query?query_subset:ref_subset;
	    std::string txt = "";
	    for (int x=0; x<(int)subset.size(); x++) {
	      txt += a.cellString(subset[x],y) + "__";
	    }
	    m.setCurr(0,y);
	    m.add(txt,query,alt,0);
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
	    m.add(txt,query,alt,ctrl);
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
    if (flags.trust_ids||flags.bias_ids||comp.isBlank()) {
      apply(a,asel,-1,false,false,ctrl);
      apply(b,bsel,-1,true,false,ctrl);
      return;
    }
    // Have a column mapping

    int w = a.width();
    for (int i=0; i<w; i++) {
      int j = comp.a2b(i);
      if (j!=-1) {
	m.resetCache();
	apply(a,asel,i,false,false,ctrl);
	apply(b,bsel,j,false,true,ctrl);
	apply(b,bsel,j,true,false,ctrl);
      }
    }
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

 CombinedRowMan(const CompareFlags& flags,
		const OrderResult& comp,
		int len) : man1(flags,comp,len), man2(flags,comp,len) {
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
