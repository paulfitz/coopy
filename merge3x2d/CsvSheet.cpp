#include "CsvSheet.h"

using namespace std;

void FloatSheet::normalize(int first, int last, float sc) {
  int hh = height();
  int ww = width();
  if (first==-1) first = 0;
  if (last==-1) last = ww-1;
  float tot = 0;
  float tot2 = 0;
  int ct = 0;
  for (int i=0; i<hh; i++) {
    for (int j=first; j<=last; j++) {
      float r = cell(j,i);
      tot += r;
      tot2 += r*r;
      ct++;
    }
  }
  if (ct==0) return;
  float mean = tot;
  float dev = 1;
  mean /= ct;
  dev = tot2 / ct - mean*mean;
  if (dev<0) { 
    dev = 0; 
  } else {
    dev = sqrt(dev);
  }
  //printf("mean %g, dev %g\n", mean, dev);
  if (dev<sc) dev = sc;
  for (int i=0; i<hh; i++) {
    for (int j=first; j<=last; j++) {
      float r = cell(j,i);
      r = (r-mean)/dev;
      cell(j,i) = r;
    }
  }
}
