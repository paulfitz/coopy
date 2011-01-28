#ifndef COOPY_POLYSHEET
#define COOPY_POLYSHEET

#include <coopy/DataSheet.h>
#include <coopy/SheetSchema.h>

namespace coopy {
  namespace store {
    class PolySheet;
  }
}

class coopy::store::PolySheet : public DataSheet {
private:
  DataSheet *sheet;
  bool owned;
  SheetSchema *schema;
  bool owned_schema;
public:
  PolySheet() {
    sheet = 0/*NULL*/;
    owned = false;
    schema = 0/*NULL*/;
    owned_schema = false;
  }

  PolySheet(DataSheet *sheet, bool owned) : sheet(sheet), owned(owned) {
    if (sheet!=0/*NULL*/&&owned) {
      sheet->addReference();
    }
    schema = 0/*NULL*/;
    owned_schema = false;
  }

  PolySheet(const PolySheet& alt) {
    owned = alt.owned;
    sheet = alt.sheet;
    if (sheet!=0/*NULL*/&&owned) {
      sheet->addReference();
    }
    schema = alt.schema;
    owned_schema = alt.owned_schema;
    if (schema!=0/*NULL*/&&owned_schema) {
      schema->addReference();
    }
  }

  const PolySheet& operator=(const PolySheet& alt) {
    clear();
    owned = alt.owned;
    sheet = alt.sheet;
    if (sheet!=0/*NULL*/&&owned) {
      sheet->addReference();
    }
    schema = alt.schema;
    owned_schema = alt.owned_schema;
    if (schema!=0/*NULL*/&&owned_schema) {
      schema->addReference();
    }
    return *this;
  }

  virtual ~PolySheet() {
    clear();
  }

  void setSchema(SheetSchema *schema, bool owned) {
    clearSchema();
    this->schema = schema;
    this->owned_schema = owned;
    if (schema!=0/*NULL*/&&owned_schema) {
      schema->addReference();
    }
  }

  virtual SheetSchema *getSchema() const {
    if (schema!=0/*NULL*/) return schema;
    if (sheet==0/*NULL*/) return 0/*NULL*/;
    return sheet->getSchema();
  }

  void clear() {
    clearSheet();
    clearSchema();
  }

  void clearSheet() {
    if (sheet==0/*NULL*/) return;
    if (owned) {
      int ref = sheet->removeReference();
      if (ref==0) {
	delete sheet;
      }
    }
    sheet = 0/*NULL*/;
  }
  
  void clearSchema() {
    if (schema==0/*NULL*/) return;
    if (owned_schema) {
      int ref = schema->removeReference();
      if (ref==0) {
	delete schema;
      }
    }
    schema = 0/*NULL*/;
  }

  bool isValid() const {
    return sheet!=0/*NULL*/;
  }

  virtual int width() const {
    COOPY_ASSERT(sheet);
    return sheet->width();
  }

  virtual int height() const {
    COOPY_ASSERT(sheet);
    return sheet->height();
  }

  virtual std::string cellString(int x, int y) const {
    COOPY_ASSERT(sheet);
    return sheet->cellString(x,y);
  }

  virtual std::string cellString(int x, int y, bool& escaped) const {
    COOPY_ASSERT(sheet);
    return sheet->cellString(x,y,escaped);
  } 

  virtual bool cellString(int x, int y, const std::string& str) {
    COOPY_ASSERT(sheet);
    return sheet->cellString(x,y,str);
  }

  virtual bool cellString(int x, int y, const std::string& str, bool escaped) {
    COOPY_ASSERT(sheet);
    return sheet->cellString(x,y,str,escaped);
  }

  std::string encode(const SheetStyle& style) const {
    COOPY_ASSERT(sheet);
    return sheet->encode(style);
  }

  //virtual SheetSchema *getSchema() {
  //return sheet->getSchema();
  //}

  virtual bool deleteColumn(const ColumnRef& column) {
    COOPY_ASSERT(sheet);
    return sheet->deleteColumn(column);
  }

  virtual ColumnRef insertColumn(const ColumnRef& base) {
    COOPY_ASSERT(sheet);
    return sheet->insertColumn(base);
  }

  // move a column before base; if base is invalid move after all columns
  virtual ColumnRef moveColumn(const ColumnRef& src, const ColumnRef& base) {
    COOPY_ASSERT(sheet);
    return sheet->moveColumn(src,base);
  }

  virtual bool deleteRow(const RowRef& src) {
    COOPY_ASSERT(sheet);
    return sheet->deleteRow(src);
  }

  // insert a row before base; if base is invalid insert after all rows
  virtual RowRef insertRow(const RowRef& base) {
    COOPY_ASSERT(sheet);
    return sheet->insertRow(base);
  }

  // move a row before base; if base is invalid move after all rows
  virtual RowRef moveRow(const RowRef& src, const RowRef& base) {
    COOPY_ASSERT(sheet);
    return sheet->moveRow(src,base);
  }

  virtual bool copyData(const DataSheet& src) {
    COOPY_ASSERT(sheet);
    return sheet->copyData(src);
  }

  virtual bool canWrite() { 
    COOPY_ASSERT(sheet);
    return sheet->canWrite();
  }

  virtual bool canResize() {
    COOPY_ASSERT(sheet);
    return sheet->canResize();
  }

  virtual bool resize(int w, int h) {
    COOPY_ASSERT(sheet);
    return sheet->resize(w,h);
  }

  virtual Poly<SheetRow> insertRow() {
    COOPY_ASSERT(sheet);
    return sheet->insertRow();
  }

  virtual bool applySchema(const SheetSchema& ss) {
    COOPY_ASSERT(sheet);
    return sheet->applySchema(ss);
  }

  virtual bool applyRowCache(const RowCache& cache, int row) {
    COOPY_ASSERT(sheet);
    return sheet->applyRowCache(cache,row);
  }

  virtual bool deleteData() {
    COOPY_ASSERT(sheet);
    return sheet->deleteData();
  }

  virtual bool hasDimension() const {
    COOPY_ASSERT(sheet);
    return sheet->hasDimension();
  }

  virtual bool forceWidth(int width) {
    COOPY_ASSERT(sheet);
    return sheet->forceWidth(width);
  }

  virtual bool forceHeight(int height) {
    COOPY_ASSERT(sheet);
    return sheet->forceHeight(height);
  }

  virtual bool hasExternalColumnNames() const {
    COOPY_ASSERT(sheet);
    return sheet->hasExternalColumnNames();
  }


  virtual std::string getDescription() const {
    return "poly";
  }

  virtual std::vector<std::string> getNestedDescription() const {
    COOPY_ASSERT(sheet);
    std::vector<std::string> v = sheet->getNestedDescription();
    std::string name = getDescription();
    v.insert(v.begin(),name);
    return v;
  }
};

#endif
