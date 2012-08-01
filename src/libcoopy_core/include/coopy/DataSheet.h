#ifndef COOPY_DATASHEET
#define COOPY_DATASHEET

#include <coopy/Dbg.h>
#include <coopy/SheetStyle.h>
#include <coopy/SheetSchema.h>
#include <coopy/RowRef.h>
#include <coopy/ColumnRef.h>
#include <coopy/RefCount.h>
#include <coopy/SheetCell.h>
#include <coopy/Appearance.h>

#include <string>
#include <vector>

namespace coopy {
  namespace store {
    class RowCache;
    class DataSheet;
    class SheetRow;
    class OrderedSheetRow;
    class CacheSheetRow;
    class Pool;
    class SheetSchema;
  }
}

namespace coopy {
  namespace cmp {
    class Compare;
  }
}

class coopy::store::RowCache {
public:
  std::vector<bool> flags;
  std::vector<bool> invent;
  std::vector<SheetCell> cells;

  RowCache(int len) {
    flags.resize(len,false);
    invent.resize(len,false);
    cells.resize(len);
  }
};


/**
 *
 * An abstract table.  
 *
 */
class coopy::store::DataSheet : public RefCount {
public:
  DataSheet() {
    pool = 0 /*NULL*/;
    meta_hint = 0 /*NULL*/;
  }

  virtual ~DataSheet();

  /**
   *
   * @return width of table
   *
   */
  virtual int width() const = 0;

  /**
   *
   * @return height of table
   *
   */
  virtual int height() const = 0;

  /**
   *
   * @return contents of cell in column x, row y, as a string.
   * The value returned by a cell with a NULL value is unspecified.
   *
   */
  virtual std::string cellString(int x, int y) const = 0;

  /**
   *
   * @return contents of cell in column x, row y, as a string.  The
   * "escaped" variable is set to true iff the cell has a NULL value.
   *
   */
  virtual std::string cellString(int x, int y, bool& escaped) const {
    escaped = false;
    return cellString(x,y);
  }

  /**
   *
   * @return contents of cell in column x, row y
   *
   */
  virtual SheetCell cellSummary(int x, int y) const {
    SheetCell c;
    c.text = cellString(x,y,c.escaped);
    return c;
  }

  /**
   *
   * sets the contents of cell in column x, row y to a specified string value
   *
   */
  virtual bool cellString(int x, int y, const std::string& str) {
    return false;
  }

  /**
   *
   * sets the contents of cell in column x, row y to a specified string value,
   * or to NULL if "escaped" is set.
   *
   */
  virtual bool cellString(int x, int y, const std::string& str, bool escaped) {
    return cellString(x,y,str);
  }

  /**
   *
   * sets the contents of cell in column x, row y to a specified value.
   *
   */
  virtual bool cellSummary(int x, int y, const SheetCell& c) {
    return cellString(x,y,c.text,c.escaped);
  }

  /**
   *
   * @return true iff cells in this table may be NULL
   *
   */
  virtual bool canEscape() const {
    return false;
  }

  /**
   *
   * @return contents of cell in column x, row y
   *
   */
  virtual SheetCell getCell(int x, int y) const {
    return cellSummary(x,y);
  }

  /**
   *
   * set contents of cell in column x, row y
   *
   */
  virtual bool setCell(int x, int  y, const SheetCell& c) {
    return cellSummary(x,y,c);
  }

  /**
   *
   * Render table as serialized text, using the specified style.
   *
   */
  std::string encode(const SheetStyle& style) const;

  /**
   *
   * Render table as serialized text, using a default style.
   *
   */
  std::string toString() const {
    SheetStyle style;
    return encode(style);
  }

  /**
   *
   * Encode a cell value as text using a specified style.
   *
   */
  static std::string encodeCell(const SheetCell& str, 
				const SheetStyle& style);

  /**
   *
   * Remove any cached values if used, e.g. in proxies for remote tables.
   *
   */
  virtual bool clearCache() { return true; }

  virtual SheetSchema *getSchema() const {
    return 0 /*NULL*/;
  }

  virtual bool deleteColumn(const ColumnRef& column) {
    return false;
  }

  // insert a column before base; if base is invalid insert after all columns
  virtual ColumnRef insertColumn(const ColumnRef& base) {
    return ColumnRef(); // invalid column
  }

  virtual ColumnRef insertColumn(const ColumnRef& base, 
				 const ColumnInfo& info) = 0;

  virtual bool modifyColumn(const ColumnRef& base, 
			    const ColumnInfo& info) = 0;

  // move a column before base; if base is invalid move after all columns
  virtual ColumnRef moveColumn(const ColumnRef& src, const ColumnRef& base) {
    return ColumnRef(); // invalid column
  }

  virtual bool deleteRow(const RowRef& src) {
    return false;
  }

  virtual bool deleteRows(const RowRef& first, const RowRef& last) {
    for (int i=first.getIndex(); i<=last.getIndex(); i++) {
      bool ok = deleteRow(RowRef(i));
      if (!ok) return false;
    }
    return true;
  }

  virtual bool deleteData(int offset = 0) {
    int h = height();
    for (int i=offset; i<h; i++) {
      deleteRow(RowRef(offset));
    }
    return true;
  }

  virtual bool hasDimension() const {
    return true;
  }

  virtual bool forceWidth(int width) {
    return false;
  }

  virtual bool forceHeight(int height) {
    return false;
  }

  // insert a row before base; if base is invalid insert after all rows
  virtual RowRef insertRow(const RowRef& base) {
    return RowRef(); // invalid row
  }

  // move a row before base; if base is invalid move after all rows
  virtual RowRef moveRow(const RowRef& src, const RowRef& base) {
    return RowRef(); // invalid column
  }

  virtual bool copyData(const DataSheet& src);

  virtual bool canWrite() { return true; }

  virtual bool canResize() { return false; }

  virtual bool hasExternalColumnNames() const {
    return false;
  }

  virtual bool resize(int w, int h) {
    return false;
  }

  virtual Poly<SheetRow> insertRow();

  virtual Poly<SheetRow> insertRowOrdered(const RowRef &base);

  virtual bool applyRowCache(const RowCache& cache, int row,
			     SheetCell *result);

  virtual bool applySchema(const SheetSchema& ss) {
    return false;
  }

  virtual bool addedHeader() {
    if (getSchema()) {
      getSchema()->addedHeader();
    }
    return true;
  }

  virtual std::string getDescription() const {
    return "data";
  }

  virtual std::vector<std::string> getNestedDescription() const {
    std::string name = getDescription();
    std::vector<std::string> v;
    v.push_back(name);
    return v;
  }

  virtual std::string desc() const {
    std::vector<std::string> v = getNestedDescription();
    std::string output;
    for (int i=0; i<(int)v.size(); i++) {
      if (output!="") {
	output += ".";
      }
      output += v[i];
    }
    return output;
  }

  virtual std::string getHash(bool cache=false) const;

  virtual std::string getRawHash() const {
    return "";
  }

  virtual DataSheet& tail() {
    return *this;
  }

  virtual const DataSheet& tail_const() const {
    return *this;
  }

  virtual const DataSheet& dataTail() const {
    return *this;
  }

  // has spreadsheet-like ordering, inserts happen in a "place"
  virtual bool isSequential() const {
    return true;
  }

  virtual DataSheet *getNestedSheet(int x, int y) {
    return 0/*NULL*/; 
  }

  virtual Poly<Appearance> getCellAppearance(int x, int y) {
    return Poly<Appearance>();
  }

  virtual Poly<Appearance> getRowAppearance(int y) {
    return Poly<Appearance>();
  }

  virtual Poly<Appearance> getColAppearance(int x) {
    return Poly<Appearance>();
  }

  virtual bool hasSheetName() const {
    return true;
  }

  virtual bool hasRowOffset() const {
    return false;
  }

  virtual bool setPool(Pool *pool) {
    this->pool = pool;
    return true;
  }

  virtual Pool *getPool() const {
    return pool;
  }

  virtual void setMeta(SheetSchema *hint);

  virtual SheetSchema *getMeta() const;

  virtual bool beginTransaction() {
    return false;
  }
  
  virtual bool rollbackTransaction() {
    return false;
  }
  
  virtual bool endTransaction() {
    return false;
  }


  virtual void *getDatabase() const {
    return 0/*NULL*/;
  }

  virtual coopy::cmp::Compare *getComparisonMethod() {
    return 0/*NULL*/;
  }

private:
  std::string hash_cache;
  SheetSchema *meta_hint;
  Pool *pool;
};

class coopy::store::SheetRow : public RefCount {
public:
  int delta;

  SheetRow() {
    delta = 0;
  }

  virtual SheetCell getCell(int x) const = 0;
  
  virtual bool setCell(int x, const SheetCell& c) = 0;

  virtual bool flush() = 0;

  virtual bool invent(int x) = 0;

  virtual bool setDelta(int dh) { delta = dh; return true; }

  // only valid AFTER flush
  virtual RowRef getRowAfterFlush() = 0;

  virtual bool undo() = 0;
};



class coopy::store::OrderedSheetRow : public SheetRow {
protected:
  int y;
  DataSheet *sheet;
public:
  OrderedSheetRow() {
    y = -1;
    sheet = 0 /*NULL*/;
  }

  OrderedSheetRow(DataSheet *sheet, int y) : sheet(sheet), y(y) {
  }

  virtual SheetCell getCell(int x) const {
    return sheet->cellSummary(x,y);
  }
  
  virtual bool setCell(int x, const SheetCell& c) {
    return sheet->cellSummary(x,y,c);
  }

  virtual bool invent(int x);

  virtual bool flush() {
    return true;
  }

  virtual RowRef getRowAfterFlush() {
    return RowRef((y!=-1)?(y+delta):y);
  }

  virtual bool undo() {
    return sheet->deleteRow(y);
  }
};


/**
 * Cache for row insertion.
 */
class coopy::store::CacheSheetRow : public SheetRow {
protected:
  RowCache cache;
  SheetCell result;
  DataSheet *sheet;
public:
  CacheSheetRow(DataSheet *sheet) : SheetRow(), 
    cache(sheet->width()) {
    this->sheet = sheet;
  }

  virtual SheetCell getCell(int x) const {
    return SheetCell();
  }
  
  virtual bool setCell(int x, const SheetCell& c) {
    cache.cells[x] = c;
    cache.flags[x] = true;
    return true;
  }

  virtual bool invent(int x) {
    cache.invent[x] = true;
    return true;
  }

  virtual bool flush() {
    return sheet->applyRowCache(cache,-1,&result);
  }

  SheetCell getResult() {
    return result;
  }

  virtual RowRef getRowAfterFlush() {
    if (result.escaped) return -1;
    int y = result.asInt();
    return RowRef((y!=-1)?(y+delta):y);
  }

  virtual bool undo() {
    return true;
  }
};

#endif
