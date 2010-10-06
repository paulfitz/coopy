#ifndef COOPY_CSVPATCHER
#define COOPY_CSVPATCHER

#include <coopy/Patcher.h>

namespace coopy {
  namespace cmp {
    class CsvPatcher;
  }
}

// not yet implemented
class coopy::cmp::CsvPatcher {
public:
  virtual bool changeColumn(const OrderChange& change);
  virtual bool changeRow(const RowChange& change);
  virtual bool declareNames(const std::vector<std::string>& names, bool final);
};

#endif

