#ifndef COOPY_SHEETSCHEMA
#define COOPY_SHEETSCHEMA

#include <coopy/ColumnInfo.h>

namespace coopy {
  namespace store {
    class SheetSchema;
  }
}

class coopy::store::SheetSchema {
public:
  virtual ColumnInfo getColumnInfo(int x) {
    return ColumnInfo(x);
  }

  virtual int headerHeight() {
    return 0;
  }
};

#endif
