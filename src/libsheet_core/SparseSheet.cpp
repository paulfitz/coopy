#include <coopy/SparseSheet.h>

using namespace std;

Stat SparseFloatSheet::normalize(int first, int last, float sc, bool modify) {
  Stat s;
  int hh = height();
  int ww = width();
  if (first==-1) first = 0;
  if (last==-1) last = ww-1;
  float tot = 0;
  float tot2 = 0;
  int ct = 0;
  for (efficient_map<long int,float>::iterator it=data.begin(); it!=data.end(); it++) {
    long int col = it->first % ww;
    if (col>=first&&col<=last) {
      float r = it->second;
      tot += r;
      tot2 += r*r;
      ct++;
    }
  }
  if (ct==0) return s;
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
  if (modify) {
    for (efficient_map<long int,float>::iterator it=data.begin(); it!=data.end(); it++) {
      long int col = it->first % ww;
      if (col>=first&&col<=last) {
	float r = it->second;
	r = (r-mean)/dev;
	it->second = r;
      }
    }
  }
  s.mean = mean;
  s.stddev = dev;
  s.valid = true;
  return s;
}
