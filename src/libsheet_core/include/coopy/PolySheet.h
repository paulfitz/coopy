#ifndef COOPY_POLYSHEET
#define COOPY_POLYSHEET

#include <coopy/DataSheet.h>

namespace coopy {
  namespace store {
    class PolySheet;
  }
}

class coopy::store::PolySheet : public DataSheet {
private:
  DataSheet *sheet;
  bool owned;
public:
  PolySheet() {
    sheet = 0/*NULL*/;
    owned = false;
  }

  PolySheet(DataSheet *sheet, bool owned) : sheet(sheet), owned(owned) {
    if (sheet!=0/*NULL*/&&owned) {
      sheet->addReference();
    }
  }

  PolySheet(const PolySheet& alt) {
    owned = alt.owned;
    sheet = alt.sheet;
    if (sheet!=NULL&&owned) {
      sheet->addReference();
    }
  }

  const PolySheet& operator=(const PolySheet& alt) {
    clear();
    owned = alt.owned;
    sheet = alt.sheet;
    if (sheet!=0/*NULL*/&&owned) {
      sheet->addReference();
    }
    return *this;
  }

  virtual ~PolySheet() {
    clear();
  }

  void clear() {
    if (sheet==0/*NULL*/) return;
    if (owned) {
      int ref = sheet->removeReference();
      if (ref==0) {
	delete sheet;
      }
    }
    sheet = 0/*NULL*/;
  }

  bool isValid() const {
    return sheet!=0/*NULL*/;
  }

  virtual int width() const {
    return sheet->width();
  }

  virtual int height() const {
    return sheet->height();
  }

  virtual std::string cellString(int x, int y) const {
    return sheet->cellString(x,y);
  }

  virtual bool cellString(int x, int y, const std::string& str) {
    return sheet->cellString(x,y,str);
  }

  std::string encode(const SheetStyle& style) const {
    return sheet->encode(style);
  }

  virtual SheetSchema *getSchema() {
    return sheet->getSchema();
  }

  virtual bool deleteColumn(const ColumnRef& column) {
    return sheet->deleteColumn(column);
  }

  virtual ColumnRef insertColumn(const ColumnRef& base) {
    return sheet->insertColumn(base);
  }

  // move a column before base; if base is invalid move after all columns
  virtual ColumnRef moveColumn(const ColumnRef& src, const ColumnRef& base) {
    return sheet->moveColumn(src,base);
  }

  virtual bool deleteRow(const RowRef& src) {
    return sheet->deleteRow(src);
  }

  // insert a row before base; if base is invalid insert after all rows
  virtual RowRef insertRow(const RowRef& base) {
    return sheet->insertRow(base);
  }

  // move a row before base; if base is invalid move after all rows
  virtual RowRef moveRow(const RowRef& src, const RowRef& base) {
    return sheet->moveRow(src,base);
  }

};

#endif
