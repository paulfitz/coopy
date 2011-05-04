#ifndef COOPY_FOLDEDSHEET
#define COOPY_FOLDEDSHEET

#include <string>
#include <coopy/TypedSheet.h>

namespace coopy {
  namespace fold {
    class FoldedCell;
    class FoldedSheet;
  }
}

class coopy::fold::FoldedCell {
public:
  coopy::store::SheetCell datum;
  FoldedSheet *sheet;

  FoldedCell() {
    sheet = 0 /*NULL*/;
  }

  ~FoldedCell() {
    clear();
  }

  void clear();

  FoldedSheet *getSheet() { return sheet; }

  FoldedSheet *getOrCreateSheet();
};

class coopy::fold::FoldedSheet : public coopy::store::TypedSheet<FoldedCell> {
public:
  using coopy::store::DataSheet::cellString;

  virtual std::string cellString(int x, int y) const;

 virtual bool cellString(int x, int y, const std::string& str) {
   return cellSummary(x,y,coopy::store::SheetCell(str,false));
  }

  virtual bool cellString(int x, int y, const std::string& str, bool escaped) {
    return cellSummary(x,y,coopy::store::SheetCell(str,escaped));
  }

  virtual bool cellSummary(int x, int y, const coopy::store::SheetCell& c) {
    FoldedCell& c2 = cell(x,y);
    c2.datum = c;
    c2.sheet = 0/*NULL*/;
    return true;
  }
};

#endif
