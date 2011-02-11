#ifndef COOPY_SHEETPATCHER
#define COOPY_SHEETPATCHER

#include <coopy/Patcher.h>
#include <coopy/DataSheet.h>
#include <coopy/PolySheet.h>
#include <coopy/TextBook.h>
#include <coopy/MergeOutputTdiff.h>

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
  coopy::store::PolySheet psheet;
  bool summary;
  MergeOutput *chain;
public:
  coopy::store::DataSheet *sheet;
  coopy::store::TextBook *book;

  SheetPatcher(coopy::store::DataSheet *sheet) : sheet(sheet) {
    rowCursor = -1;
    book = NULL;
    summary = false;
    chain = NULL;
  }

  SheetPatcher(coopy::store::TextBook *book) : book(book) {
    rowCursor = -1;
    sheet = NULL;
    summary = false;
    chain = NULL;
  }

  virtual ~SheetPatcher() {
    if (chain!=NULL) {
      delete chain;
      chain = NULL;
    }
  }

  void showSummary() {
    summary = true;
    chain = new MergeOutputTdiff();
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

