#include <coopy/SheetSchema.h>

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

