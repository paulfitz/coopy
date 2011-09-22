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
  int dh;
public:
  PolySheet() {
    sheet = 0/*NULL*/;
    owned = false;
    schema = 0/*NULL*/;
    owned_schema = false;
    dh = 0;
  }

  PolySheet(DataSheet *sheet, bool owned) : sheet(sheet), owned(owned) {
    if (sheet!=0/*NULL*/&&owned) {
      sheet->addReference();
    }
    schema = 0/*NULL*/;
    owned_schema = false;
    dh = 0;
  }

  PolySheet(const PolySheet& alt) {
    owned = alt.owned;
    sheet = alt.sheet;
    dh = alt.dh;
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
    dh = alt.dh;
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
    if (!sheet) return 0;
    return sheet->width();
  }

  virtual int height() const {
    if (!sheet) return 0;
    return sheet->height()-dh;
  }

  virtual std::string cellString(int x, int y) const {
    COOPY_ASSERT(sheet);
    return sheet->cellString(x,y+dh);
  }

  virtual std::string cellString(int x, int y, bool& escaped) const {
    COOPY_ASSERT(sheet);
    return sheet->cellString(x,y+dh,escaped);
  } 

  virtual bool cellString(int x, int y, const std::string& str) {
    COOPY_ASSERT(sheet);
    return sheet->cellString(x,y+dh,str);
  }

  virtual bool cellString(int x, int y, const std::string& str, bool escaped) {
    COOPY_ASSERT(sheet);
    return sheet->cellString(x,y+dh,str,escaped);
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
    bool ok = sheet->deleteColumn(column);
    if (ok&&owned_schema&&schema!=0/*NULL*/) {
      schema->deleteColumn(column);
    }
    return ok;
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
    return sheet->deleteRow((dh==0)?src:src.delta(dh));
  }

  // insert a row before base; if base is invalid insert after all rows
  virtual RowRef insertRow(const RowRef& base) {
    COOPY_ASSERT(sheet);
    return sheet->insertRow((dh==0)?base:base.delta(dh));
  }

  // move a row before base; if base is invalid move after all rows
  virtual RowRef moveRow(const RowRef& src, const RowRef& base) {
    COOPY_ASSERT(sheet);
    return sheet->moveRow(src,(dh==0)?base:base.delta(dh));
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
    return sheet->resize(w,h+dh);
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
    return sheet->forceHeight(height+dh);
  }

  virtual bool hasExternalColumnNames() const {
    if (dh!=0) return true;
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

  virtual std::string getHash(bool cache) const {
    if (dh==0) {
      COOPY_ASSERT(sheet);
      return sheet->getHash(cache);
    }
    return DataSheet::getHash(cache);
  }

  virtual DataSheet& tail() {
    if (dh!=0) {
      return *this;
    }
     COOPY_ASSERT(sheet);
     return sheet->tail();
  }

  virtual bool isSequential() const {
    COOPY_ASSERT(sheet);
    return sheet->isSequential();
  }

  virtual DataSheet *getNestedSheet(int x, int y) {
    COOPY_ASSERT(sheet);
    return sheet->getNestedSheet(x,y);
  }

  bool setRowOffset(int dh) {
    this->dh = dh;
    return true;
  }

  bool setRowOffset() {
    COOPY_ASSERT(sheet);
    if (dh>0) return true;
    SheetSchema *schema = getSchema();
    if (!schema) {
      schema = sheet->getSchema();
    }
    if (!schema) return true;
    if (sheet->hasExternalColumnNames()) return true;
    if (schema->headerHeight()>=0) {
      setRowOffset(schema->headerHeight()+1);
    }
    return true;
  }

  bool createHeaders() {
    if (dh>0) return false;
    if (!hasExternalColumnNames()) {
      SheetSchema *schema = getSchema();
      if (schema) {
	insertRow(RowRef(0));
	for (int i=0; i<schema->getColumnCount(); i++) {
	  cellString(i,0,schema->getColumnInfo(i).getName());
	}
	setRowOffset(1);
	schema->setHeaderHeight(1);
	return true;
      }
    }
    return false;
  }

  bool hideHeaders() {
    if (!hasExternalColumnNames()) {
      if (getSchema()==0/*NULL*/) {
	setRowOffset(1);
	return true;
      }
    }
    return false;
  }

  virtual Poly<Appearance> getCellAppearance(int x, int y) {
    COOPY_ASSERT(sheet);
    return sheet->getCellAppearance(x,y+dh);
  }

  virtual Poly<Appearance> getRowAppearance(int y) {
    COOPY_ASSERT(sheet);
    return sheet->getRowAppearance(y+dh);
  }

  virtual Poly<Appearance> getColAppearance(int x) {
    COOPY_ASSERT(sheet);
    return sheet->getColAppearance(x);
  }

  virtual bool hasSheetName() const {
    COOPY_ASSERT(sheet);
    return sheet->hasSheetName();
  }

  virtual bool addedHeader() {
    COOPY_ASSERT(sheet);
    return sheet->addedHeader();
  }
};

#endif
