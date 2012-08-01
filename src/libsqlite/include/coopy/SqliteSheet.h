#ifndef COOPY_SQLITESHEET
#define COOPY_SQLITESHEET

#include <coopy/DataSheet.h>
#include <coopy/SparseSheet.h>
#include <coopy/EfficientMap.h>
#include <coopy/ColumnInfo.h>
#include <coopy/Compare.h>

#include <vector>

namespace coopy {
  namespace store {
    namespace sqlite {
      class SqliteSheet;
      class SqliteSheetSchema;
    }
  }
}

class coopy::store::sqlite::SqliteSheet : public DataSheet, 
  public coopy::cmp::Compare  {
public:
  SqliteSheet(void *db, const char *name, const char *prefix);

  bool connect();
  bool create(const SheetSchema& schema);

  virtual ~SqliteSheet();

  virtual int width() const  { return w; }
  virtual int height() const { return h; }

  virtual std::string cellString(int x, int y) const;

  virtual std::string cellString(int x, int y, bool& escaped) const;

  virtual bool cellString(int x, int y, const std::string& str) {
    return cellString(x,y,str,false);
  }

  virtual bool cellString(int x, int y, const std::string& str, bool escaped);

  virtual ColumnRef moveColumn(const ColumnRef& src, const ColumnRef& base);

  virtual bool deleteColumn(const ColumnRef& column);

  virtual ColumnRef insertColumn(const ColumnRef& base);

  virtual ColumnRef insertColumn(const ColumnRef& base, const ColumnInfo& kind);

  virtual bool modifyColumn(const ColumnRef& base, const ColumnInfo& kind);

  virtual RowRef insertRow(const RowRef& base);

  virtual bool deleteRow(const RowRef& src);

  virtual ColumnInfo getColumnInfo(int x) {
    /*
    ColumnType t;
    t.primaryKey = col2pk[x];
    t.primaryKeySet = true;
    //printf(">> %d %s\n", x, col2sql[x].c_str());
    return ColumnInfo(col2sql[x],t);
    */
    return col2sql[x];
  }

  virtual int getColumnCount() const {
    return (int)col2sql.size();
  }

  virtual SheetSchema *getSchema() const;

  virtual bool applyRowCache(const RowCache& cache, int row, SheetCell *result);

  virtual bool deleteData(int offset);

  virtual bool hasExternalColumnNames() const {
    return true;
  }

  std::string getName() const {
    return name;
  }

  virtual bool clearCache() { 
    cache.clear();
    cacheFlag.clear();
    return true;
  }

 virtual bool isSequential() const {
   return false;
 }

 virtual void *getDatabase() const {
   return implementation;
 }

  virtual std::string getDescription() const {
    return "sqlite";
  }

 virtual coopy::cmp::Compare *getComparisonMethod() {
   return this;
 }

 virtual int compare(coopy::store::DataSheet& pivot, 
		     coopy::store::DataSheet& local, 
		     coopy::store::DataSheet& remote, 
		     coopy::cmp::Patcher& output, 
		     const coopy::cmp::CompareFlags& flags);

  virtual std::string getRawHash() const;

private:
  SqliteSheetSchema *schema;
  void *implementation;
  std::string name;
  std::string quoted_name;
  std::string prefix;
  std::string prefix_dot;
  int w, h;
  bool pending_load;
  std::vector<int> row2sql;
  std::vector<ColumnInfo> col2sql;

  //std::vector<std::string> col2sql;
  std::vector<std::string> primaryKeys;
  //std::vector<bool> col2pk;
  SparseStringSheet cache;
  SparseByteSheet cacheFlag;
  void checkPrimaryKeys();
  void checkForeignKeys();

  void check();

public:
  static bool isReserved(const std::string& name);
  static std::string _quoted(const std::string& x, char ch, bool force);
  static std::string _quoted_double(const std::string& x);
  static std::string _quoted_single(const std::string& x);
};

class coopy::store::sqlite::SqliteSheetSchema : public SheetSchema {
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

  virtual bool isShadow() const {
    return true;
  }
};

#endif
