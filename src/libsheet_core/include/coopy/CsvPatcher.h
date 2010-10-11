#ifndef COOPY_CSVPATCHER
#define COOPY_CSVPATCHER

#include <coopy/Patcher.h>
#include <coopy/DataSheet.h>

namespace coopy {
  namespace cmp {
    class CsvPatcher;
  }
}

class coopy::cmp::CsvPatcher : public Patcher {
public:
  coopy::store::DataSheet *sheet;

  CsvPatcher(coopy::store::DataSheet *sheet) : sheet(sheet) {
  }

  virtual bool changeColumn(const OrderChange& change);
  virtual bool changeRow(const RowChange& change);
  virtual bool declareNames(const std::vector<std::string>& names, bool final);
};

#endif

