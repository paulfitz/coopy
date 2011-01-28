#ifndef COOPY_SQLITESHEET
#define COOPY_SQLITESHEET

#include <coopy/DataSheet.h>

#include <vector>

namespace coopy {
  namespace store {
    class SqliteSheet;
    class SqliteSheetSchema;
  }
}

class coopy::store::SqliteSheet : public DataSheet {
public:
  SqliteSheet(void *db, const char *name);

  bool connect();
  bool create(const SheetSchema& schema);

  virtual ~SqliteSheet();

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

  virtual ColumnInfo getColumnInfo(int x) {
    ColumnType t;
    t.primaryKey = col2pk[x];
    t.primaryKeySet = true;
    //printf(">> %d %s\n", x, col2sql[x].c_str());
    return ColumnInfo(col2sql[x],t);
  }

  virtual int getColumnCount() const {
    return (int)col2sql.size();
  }

  virtual SheetSchema *getSchema() const;

  virtual bool applyRowCache(const RowCache& cache, int row);

  virtual bool deleteData();  

  virtual bool hasExternalColumnNames() const {
    return true;
  }

  std::string getName() const {
    return name;
  }

private:
  SqliteSheetSchema *schema;
  void *implementation;
  std::string name;
  int w, h;
  std::vector<int> row2sql;
  std::vector<std::string> col2sql;
  std::vector<std::string> primaryKeys;
  std::vector<bool> col2pk;
  void checkKeys();
};

class coopy::store::SqliteSheetSchema : public SheetSchema {
public:
  SqliteSheet *sheet;

  virtual std::string getSheetName() const {
    return sheet->getName();
  }

  virtual ColumnInfo getColumnInfo(int x) const {
    return sheet->getColumnInfo(x);
  }

  virtual int getColumnCount() const {
    return sheet->getColumnCount();
  }

  virtual bool providesPrimaryKeys() const {
    return true;
  }
};

#endif
