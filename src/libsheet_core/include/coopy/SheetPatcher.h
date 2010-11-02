#ifndef COOPY_SHEETPATCHER
#define COOPY_SHEETPATCHER

#include <coopy/Patcher.h>
#include <coopy/DataSheet.h>

#include <vector>

namespace coopy {
  namespace cmp {
    class SheetPatcher;
  }
}

class coopy::cmp::SheetPatcher : public Patcher {
private:
  ConfigChange config;
  std::vector<int> columns;
public:
  coopy::store::DataSheet *sheet;

  SheetPatcher(coopy::store::DataSheet *sheet) : sheet(sheet) {
  }

  virtual bool changeConfig(const ConfigChange& change) { 
    this->config = change;
    return true; 
  }
  virtual bool changeColumn(const OrderChange& change);
  virtual bool changeRow(const RowChange& change);
  virtual bool declareNames(const std::vector<std::string>& names, bool final);
};

#endif

