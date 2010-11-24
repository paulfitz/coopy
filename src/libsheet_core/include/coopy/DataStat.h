#ifndef SSFOSSIL_DATASTAT
#define SSFOSSIL_DATASTAT

#include <coopy/DataColumn.h>
#include <coopy/ColumnInfo.h>
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
  std::vector<coopy::store::ColumnType> ct;
  coopy::store::FloatSheet oddness;
  coopy::store::FloatSheet oddness_accum;

  void clear() {
    rowDivider = -1;
    ct.clear();
  }

  void evaluate(const coopy::store::DataSheet& sheet);

  const std::vector<coopy::store::ColumnType>& suggestTypes() const {
    return ct;
  }

  int getRowDivider() {
    return rowDivider;
  }
};

#endif
