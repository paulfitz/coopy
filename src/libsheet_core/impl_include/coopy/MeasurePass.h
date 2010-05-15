#ifndef COOPY_MEASUREPASS
#define COOPY_MEASUREPASS

#include <coopy/CsvSheet.h>
#include <coopy/OrderResult.h>
#include <coopy/Dbg.h>

class MeasurePass {
public:
  CsvSheet& a;
  CsvSheet& b;
  IntSheet asel, bsel;
  FloatSheet match;

  MeasurePass(CsvSheet& a, CsvSheet& b) : a(a), b(b) {
  }

  void setSize(int alen, int blen) {
    asel.resize(1,alen,-1);
    bsel.resize(1,blen,-1);
    match.resize(alen,blen,0);
  }

  void clearMatch() {
    match.resize(match.width(),match.height(),0);
  }

  Stat flatten() {
    int w = match.width();
    int h = match.height();
    double mean = 0;
    int ct = 0;
    for (int y=0; y<h; y++) {
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
    }
    if (ct>0) { mean /= ct; }
    Stat s;
    s.mean = mean;
    s.stddev = 0;
    s.valid = (ct>10);
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

