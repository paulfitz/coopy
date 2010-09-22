#ifndef SSFOSSIL_CSVSTAT
#define SSFOSSIL_CSVSTAT

#include <coopy/CsvColumn.h>
#include <coopy/FloatSheet.h>

class CsvStat {
private:
  int rowDivider;
public:
  CsvStat() {
    clear();
  }

  std::vector<CsvColumn> col;
  coopy::store::FloatSheet oddness;
  coopy::store::FloatSheet oddness_accum;

  void clear() {
    rowDivider = -1;
  }

  void evaluate(coopy::store::CsvSheet& sheet);

  int getRowDivider() {
    return rowDivider;
  }
};

#endif
