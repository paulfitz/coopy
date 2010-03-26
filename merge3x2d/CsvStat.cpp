#include "CsvStat.h"
#include "CsvWrite.h"

#include <math.h>

using namespace std;

void CsvStat::evaluate(CsvSheet& sheet) {
  col.clear();
  oddness.resize(sheet.width(),sheet.height(),0);
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
      dev = sqrt(tot2 / sheet.height() - mean*mean);
    }
    int normal = 0;
    int odd = 0;
    for (int i=0; i<sheet.height(); i++) {
      float v = cmp[i]-(mean-2*dev);
      oddness.cell(j,i) = v;
      if (cmp[i]>mean-2*dev) {
	normal++;
      } else {
	odd++;
	printf("Column %d: Oddness at %d,%d: %s\n", j, j, i,
	       sheet.cell(j,i).c_str());
      }
    }
    printf("Column %d: Normal %d odd %d / mean %g dev %g\n", 
	   j, normal, odd, mean, dev);
  }

  CsvFile::write(oddness,"oddness.csv");
}
