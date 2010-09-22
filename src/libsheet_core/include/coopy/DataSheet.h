#ifndef COOPY_DATASHEET
#define COOPY_DATASHEET

#include <coopy/SheetStyle.h>
#include <coopy/SheetSchema.h>

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

};


#endif
