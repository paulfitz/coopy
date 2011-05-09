#ifndef COOPY_SHEETPATCHER
#define COOPY_SHEETPATCHER

#include <coopy/Patcher.h>
#include <coopy/DataSheet.h>
#include <coopy/PolySheet.h>
#include <coopy/TextBook.h>

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
  std::vector<std::string> column_names;
  int rowCursor;
  //coopy::store::PolySheet psheet;
  bool summary;
  Patcher *chain;
  int changeCount;
public:
  //coopy::store::DataSheet *sheet;
  //coopy::store::TextBook *book;

  //SheetPatcher(coopy::store::DataSheet *sheet) : sheet(sheet) {
  SheetPatcher() {
    rowCursor = -1;
    //book = NULL;
    summary = false;
    chain = NULL;
    changeCount = 0;
  }

  /*
  SheetPatcher(coopy::store::TextBook *book) : book(book) {
    rowCursor = -1;
    sheet = NULL;
    summary = false;
    chain = NULL;
    changeCount = 0;
  }
  */

  virtual ~SheetPatcher() {
  }

  void showSummary(Patcher *chain) {
    summary = true;
    this->chain = chain;
  }

  int getChangeCount() {
    return changeCount;
  }

  virtual bool changeConfig(const ConfigChange& change) { 
    if (chain) chain->changeConfig(change);
    this->config = change;
    return true; 
  }
  virtual bool changeColumn(const OrderChange& change);
  virtual bool changeRow(const RowChange& change);
  virtual bool declareNames(const std::vector<std::string>& names, bool final);

  virtual bool setSheet(const char *name);
};

#endif

