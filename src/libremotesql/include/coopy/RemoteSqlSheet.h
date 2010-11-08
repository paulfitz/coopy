#ifndef COOPY_REMOTESQLSHEET
#define COOPY_REMOTESQLSHEET

#include <coopy/DataSheet.h>
#include <coopy/Property.h>
#include <coopy/SparseSheet.h>

#include <vector>

namespace coopy {
  namespace store {
    class RemoteSqlSheet;
    class RemoteSqlSheetSchema;
    class RemoteSqlTextBook;
  }
}

class coopy::store::RemoteSqlSheet : public DataSheet {
public:
  RemoteSqlSheet(RemoteSqlTextBook *owner, const char *name);

  virtual ~RemoteSqlSheet();

  virtual int width() const  { return w; }
  virtual int height() const { return h; }

  virtual std::string cellString(int x, int y) const;

  virtual std::string cellString(int x, int y, bool& escaped) const;

  virtual bool cellString(int x, int y, const std::string& str);

  virtual ColumnRef moveColumn(const ColumnRef& src, const ColumnRef& base);

  virtual bool deleteColumn(const ColumnRef& column);

  virtual ColumnRef insertColumn(const ColumnRef& base);

  virtual RowRef insertRow(const RowRef& base);

  virtual bool deleteRow(const RowRef& src);

  virtual bool clearCache() { 
    cache.clear();
    cacheFlag.clear();
    return true;
  }

  virtual ColumnInfo getColumnInfo(int x) {
    return ColumnInfo(col2sql[x],col2pk[x]);
  }

  virtual SheetSchema *getSchema() const;

private:
  RemoteSqlSheetSchema *schema;
  RemoteSqlTextBook *book;
  void *implementation;
  SparseStringSheet cache;
  SparseByteSheet cacheFlag;
  std::string name;
  int w, h;
  // row2sql is complicated without rowid equivalent.
  // see sqlitesheet for simpler implementation with rowid.
  std::vector<std::vector<std::string> > row2sql;
  std::vector<std::string> col2sql;
  std::vector<bool> col2pk;
  std::vector<std::string> keys;
  std::vector<int> key_cols;
};

class coopy::store::RemoteSqlSheetSchema : public SheetSchema {
public:
  RemoteSqlSheet *sheet;

  virtual ColumnInfo getColumnInfo(int x) {
    return sheet->getColumnInfo(x);
  }

  virtual bool providesPrimaryKeys() const {
    return true;
  }
};


#endif
