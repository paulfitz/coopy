#ifndef COOPY_DATASHEET
#define COOPY_DATASHEET

#include <coopy/SheetStyle.h>
#include <coopy/SheetSchema.h>
#include <coopy/RowRef.h>
#include <coopy/ColumnRef.h>

#include <string>

namespace coopy {
  namespace store {
    class DataSheet;
  }
}

class coopy::store::DataSheet {
public:
  virtual int width() const = 0;
  virtual int height() const = 0;

  virtual std::string cellString(int x, int y) const = 0;

  std::string encode(const SheetStyle& style) const;

  static std::string encodeCell(const std::string& str, 
				const SheetStyle& style);

  virtual SheetSchema *getSchema() {
    return 0 /*NULL*/;
  }

  virtual bool deleteColumn(const ColumnRef& column) {
    return false;
  }

  // insert a column before base; if base is invalid insert after all columns
  virtual ColumnRef insertColumn(const ColumnRef& base) {
    return ColumnRef(); // invalid column
  }

  // move a column before base; if base is invalid move after all columns
  virtual ColumnRef moveColumn(const ColumnRef& src, const ColumnRef& base) {
    return ColumnRef(); // invalid column
  }
};

#endif
