#ifndef SSFOSSIL_CSVSTAT
#define SSFOSSIL_CSVSTAT

#include "CsvColumn.h"

class CsvStat {
public:
  std::vector<CsvColumn> col;
  FloatSheet oddness;

  void evaluate(CsvSheet& sheet);
};

#endif
