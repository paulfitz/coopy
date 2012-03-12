#ifndef COOPY_MEASUREPASS
#define COOPY_MEASUREPASS

#include <coopy/DataSheet.h>
#include <coopy/OrderResult.h>
#include <coopy/Dbg.h>
#include <coopy/SparseSheet.h>
#include <coopy/SheetView.h>

#include <math.h>

namespace coopy {
  namespace cmp {
    class MeasurePass;
  }
}

class coopy::cmp::MeasurePass {
public:
  coopy::store::SheetView& va;
  coopy::store::SheetView& vb;
  coopy::store::DataSheet& a;
  coopy::store::DataSheet& b;
  coopy::store::IntSheet asel, bsel;
  coopy::store::SparseFloatSheet match;
  int bound;

  MeasurePass(coopy::store::SheetView& va, 
	      coopy::store::SheetView& vb) : va(va), vb(vb), 
					     a(va.sheet), b(vb.sheet) {
    bound = -1;
  }

  void setSize(int alen, int blen) {
    asel.resize(1,alen,-1);
    bsel.resize(1,blen,-1);
    match.resize(alen,blen,0);
  }

  void setBound(int bound) {
    this->bound = bound;
  }

  int getBound() {
    return bound;
  }

  void clearMatch() {
    match.resize(match.width(),match.height(),0);
  }

  coopy::store::Stat flatten() {
    int w = match.width();
    int h = match.height();
    double mean = 0;
    double var = 0;
    int ct = 0;
    for (int y=0; y<h; y++) {
      if (y<w) {
	float tmp = match.cell_const(y,y);
	float tmp2 = match.cell_const(0,y);
	if (tmp2>=-0.0001) {
	  match.cell(y,y) = tmp2;
	  match.cell(0,y) = tmp;
	  if (asel.cell(0,y)==-1) {
	    //printf(" [%d:%g] ", ct, tmp);
	    mean += tmp;
	    var += tmp*tmp;
	    ct++;
	  }
	}
      }
      /*
      if (y<w) {
	float tmp = match.cell(0,y);
	match.cell(0,y) = match.cell(y,y);
	match.cell(y,y) = tmp;
	if (asel.cell(0,y)==-1) {
	  mean += match.cell(0,y);
	  ct++;
	}
      } else {
	match.cell(0,y) = 0;
      }
      */
    }
    if (ct>0) { 
      mean /= ct; 
      var /= ct;
    }
    coopy::store::Stat s;
    s.mean = mean;
    s.stddev = sqrt(var);
    s.valid = (ct>=10);
    dbg_printf("Mean is %g (count %d)\n", s.mean, ct);
    return s;
  }

  OrderResult getOrder() {
    OrderResult order;
    order.setup(asel,bsel);
    return order;
  }
};

#endif

