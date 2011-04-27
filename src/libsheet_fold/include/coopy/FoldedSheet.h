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
  virtual std::string cellString(int x, int y) const;
};

#endif
