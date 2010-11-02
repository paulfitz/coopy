#ifndef SSFOSSIL_DATASTAT
#define SSFOSSIL_DATASTAT

#include <coopy/DataColumn.h>
#include <coopy/FloatSheet.h>

namespace coopy {
  namespace cmp {
    class DataStat;
  }
}

class coopy::cmp::DataStat {
private:
  int rowDivider;
public:
  DataStat() {
    clear();
  }

  std::vector<DataColumn> col;
  coopy::store::FloatSheet oddness;
  coopy::store::FloatSheet oddness_accum;

  void clear() {
    rowDivider = -1;
  }

  void evaluate(coopy::store::DataSheet& sheet);

  int getRowDivider() {
    return rowDivider;
  }
};

#endif
