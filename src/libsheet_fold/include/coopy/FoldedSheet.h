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

  FoldedCell(const FoldedCell& alt) {
    sheet = 0 /*NULL*/;
    if (alt.sheet) {
      sheet = alt.sheet;
      ((coopy::store::RefCount*)sheet)->addReference();
    }
    datum = alt.datum;
  }

  const FoldedCell& operator = (const FoldedCell& alt) {
    clear();
    if (alt.sheet) {
      sheet = alt.sheet;
      ((coopy::store::RefCount*)sheet)->addReference();
    }    
    datum = alt.datum;
    return *this;
  }

  void clear();

  FoldedSheet *getSheet() { return sheet; }

  FoldedSheet *getOrCreateSheet();
};

class coopy::fold::FoldedSheet : public coopy::store::TypedSheet<FoldedCell> {
public:
  using coopy::store::DataSheet::cellString;

  virtual std::string cellString(int x, int y) const {
    bool b;
    return cellString(x,y,b);
  }

  virtual std::string cellString(int x, int y, bool& escaped) const {
    coopy::store::SheetCell c = cellSummary(x,y);
    escaped = c.escaped;
    return c.text;
  }

  virtual bool cellString(int x, int y, const std::string& str) {
    return cellSummary(x,y,coopy::store::SheetCell(str,false));
  }
  
  virtual bool cellString(int x, int y, const std::string& str, bool escaped) {
    return cellSummary(x,y,coopy::store::SheetCell(str,escaped));
  }

  virtual coopy::store::SheetCell cellSummary(int x, int y) const;

  virtual bool cellSummary(int x, int y, const coopy::store::SheetCell& c) {
    FoldedCell& c2 = cell(x,y);
    c2.datum = c;
    c2.sheet = 0/*NULL*/;
    return true;
  }

  virtual DataSheet *getNestedSheet(int x, int y) {
    FoldedCell& c = cell(x,y);
    return c.sheet;
  }

  virtual bool hasExternalColumnNames() const {
    return true;
  }
};

#endif
