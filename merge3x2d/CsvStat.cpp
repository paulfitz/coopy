#include "CsvStat.h"
#include "CsvWrite.h"

#include <math.h>

using namespace std;

void CsvStat::evaluate(CsvSheet& sheet) {
  col.clear();
  oddness.resize(sheet.width(),sheet.height(),0);
  oddness_accum.resize(1,sheet.height(),0);
  if (sheet.width()==0) return;
  if (sheet.height()==0) return;
  for (int j=0; j<sheet.width(); j++) {
    col.push_back(CsvColumn(sheet,j));
  }
  for (int j=0; j<sheet.width(); j++) {
    col[j].evaluate();
  }
  for (int j=0; j<sheet.width(); j++) {
    CsvColumn& c = col[j];
    Nature n = c.getNature();
    vector<float> cmp;
    float tot = 0;
    float tot2 = 0;
    for (int i=0; i<sheet.height(); i++) {
      float r = n.compare(sheet.cell(j,i).c_str());
      cmp.push_back(r);
      tot += r;
      tot2 += r*r;
    }
    float mean = tot;
    float dev = 1;
    if (sheet.height()>0) {
      mean /= sheet.height();
      dev = tot2 / sheet.height() - mean*mean;
      // occasionally dev is just slightly less than zero due to round-offs
      if (dev<0) { 
	dev = 0; 
      } else {
	dev = sqrt(dev);
      }
    }
    if (dev<0.1) dev = 0.1;
    int normal = 0;
    int odd = 0;
    for (int i=0; i<sheet.height(); i++) {
      float v = (cmp[i]-mean)/dev;
      v = -v;
      if (v<0) v = 0;
      oddness.cell(j,i) = v;
      oddness_accum.cell(0,i) += v;
    }
  }

  oddness_accum.normalize(-1,-1,1);

  CsvFile::write(oddness,"oddness.csv");
  CsvFile::write(oddness_accum,"oddness_accum.csv");

  int top = -1;
  int evidence = 0;
  for (int i=0; i<sheet.height(); i++) {
    float v = oddness_accum.cell(0,i);
    if (v>=1) {
      if (i<10) {
	top = i;
      } else {
	top = -1;
      }
    }
    if (v<0.5) {
      evidence++;
    }
  }
  /*
  if (top>=0) {
    if (evidence<top*2+2) {
      top = -1;
    }
  }
  */
  if (top>=0) {
    printf("Header guess: [%d]", top);
    for (int j=0; j<sheet.width(); j++) {
      printf(" [%s]", sheet.cellString(j,top).c_str());
    }
    printf("\n");
  } else {
    printf("Cannot guess header\n");
  }
}
