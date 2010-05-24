#ifndef COOPY_SHEETSCHEMA
#define COOPY_SHEETSCHEMA

#include <coopy/ColumnInfo.h>

class SheetSchema {
public:
  virtual ColumnInfo getColumnInfo(int x) {
    return ColumnInfo(x);
  }

  virtual int headerHeight() {
    return 0;
  }
};

#endif
