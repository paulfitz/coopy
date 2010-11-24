#include <math.h>

#include <coopy/DataStat.h>
#include <coopy/CsvWrite.h>

using namespace std;
using namespace coopy::store;
using namespace coopy::cmp;

void DataStat::evaluate(const DataSheet& sheet) {
  int hh = 50;
  if (sheet.height()<hh) hh = sheet.height();
  clear();
  col.clear();
  if (sheet.width()==0) return;
  if (hh==0) return;
  oddness.resize(sheet.width(),hh,0);
  oddness_accum.resize(1,hh,0);
  for (int j=0; j<sheet.width(); j++) {
    col.push_back(DataColumn(sheet,j,hh));
    ct.push_back(ColumnType());
  }
  for (int j=0; j<sheet.width(); j++) {
    col[j].evaluate();
  }
  for (int j=0; j<sheet.width(); j++) {
    DataColumn& c = col[j];
    Nature n = c.getNature();
    vector<float> cmp;
    float tot = 0;
    float tot2 = 0;
    for (int i=0; i<hh; i++) {
      float r = n.compare(sheet.cellString(j,i).c_str());
      cmp.push_back(r);
      tot += r;
      tot2 += r*r;
    }
    float mean = tot;
    float dev = 1;
    if (hh>0) {
      mean /= hh;
      dev = tot2 / hh - mean*mean;
      // occasionally dev is just slightly less than zero due to round-offs
      if (dev<0) { 
	dev = 0; 
      } else {
	dev = sqrt(dev);
      }
    }
    if (dev<0.1) dev = 0.1;
    for (int i=0; i<hh; i++) {
      float v = (cmp[i]-mean)/dev;
      v = -v;
      if (v<0) v = 0;
      if (sheet.cellString(j,i)=="") {
	v = 0;
      }
      oddness.cell(j,i) = v;
      oddness_accum.cell(0,i) += v;
    }

    ColumnType mct;
    if (n.couldBeInteger()) {
      mct.setType("INTEGER");
    }
    ct[j] = mct;
  }

  oddness_accum.normalize(-1,-1,1);

  //CsvFile::write(oddness,"-");
  //CsvFile::write(oddness_accum,"oddness_accum.csv");

  int top = -1;
  int evidence = 0;
  for (int i=0; i<hh; i++) {
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
    dbg_printf("Header guess: [%d]", top);
    for (int j=0; j<sheet.width(); j++) {
      dbg_printf(" [%s]", sheet.cellString(j,top).c_str());
    }
    dbg_printf("\n");
  } else {
    dbg_printf("Cannot guess header\n");
  }
  rowDivider = top;
}
