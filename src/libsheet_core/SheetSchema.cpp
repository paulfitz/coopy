#include <coopy/SheetSchema.h>
#include <coopy/Stringer.h>

using namespace coopy::store;

using namespace std;

SheetSchema *SheetSchema::clone() const {
  SimpleSheetSchema *ss = new SimpleSheetSchema();
  if (ss==NULL) return ss;
  ss->setHeaderHeight(headerHeight());
  ss->setSheetName(getSheetName().c_str());
  for (int i=0; i<getColumnCount(); i++) {
    ColumnInfo inf = getColumnInfo(i);
    ss->addColumn(inf.getName().c_str(),inf.getColumnType());
  }
  return ss;
}


std::string SimpleSheetSchema::toString() const {
  std::string r = "";
  r += "sheet ";
  r += getSheetName();
  r += " header height ";
  int h = headerHeight();
  if (h>0) {
    r += ">0";
  } else if (h==0) {
    r += "=0";
  } else {
    r += "<0";
  }
  r += " columns ";
  r += stringer_encoder(getColumnCount());
  r += " (";
  for (int i=0; i<getColumnCount(); i++) {
    if (i>0) r += ", ";
    r += getColumnInfo(i).getName();
  }
  r += ")";
  return r;
}

