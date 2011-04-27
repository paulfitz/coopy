#include <coopy/FoldedSheet.h>

using namespace std;
using namespace coopy::fold;
using namespace coopy::store;

void FoldedCell::clear() {
  if (sheet!=NULL) delete sheet;
  sheet = NULL;
  datum.text = "";
  datum.escaped = true;
}

FoldedSheet *FoldedCell::getOrCreateSheet() {
  if (sheet!=NULL) return sheet;
  sheet = new FoldedSheet;
  return sheet;
}

std::string FoldedSheet::cellString(int x, int y) const {
  const FoldedCell& c = cell(x,y);
  string result;
  if (c.sheet) {
    result += "(";
    SheetStyle style;
    style.setEol("; ");
    style.setEolAtEof(false);
    result += c.sheet->encode(style);
    result += ")";
  } else {
    result = c.datum.toString();
  }
  return result;
}

