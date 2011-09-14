#include <coopy/FoldedSheet.h>

using namespace std;
using namespace coopy::fold;
using namespace coopy::store;

void FoldedCell::clear() {
  if (sheet!=NULL) {
    sheet->removeReference();
    if (sheet->getReferenceCount()==0) {
      delete sheet;
    }
  }
  sheet = NULL;
  datum.text = "";
  datum.escaped = true;
}

FoldedSheet *FoldedCell::getOrCreateSheet() {
  if (sheet!=NULL) return sheet;
  sheet = new FoldedSheet;
  sheet->addReference();
  return sheet;
}

SheetCell FoldedSheet::cellSummary(int x, int y) const {
  const FoldedCell& c = cell(x,y);
  if (!c.sheet) return c.datum;
  string result = "(";
  SheetStyle style;
  style.setEol("; ");
  style.setEolAtEof(false);
  result += c.sheet->encode(style);
  result += ")";
  return SheetCell(result,false);
}

