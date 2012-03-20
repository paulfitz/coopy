#ifndef COOPY_JACKSHEET
#define COOPY_JACKSSHEET

#include <coopy/DataSheet.h>
#include <coopy/SparseSheet.h>
#include <coopy/Property.h>

#include <vector>

namespace coopy {
  namespace store {
    namespace mdb {
      class JackSheet;
      class JackSheetSchema;
    }
  }
}

class coopy::store::mdb::JackSheet : public DataSheet {
public:
  JackSheet(void *db, void *box, const char *name, const Property& config);

  bool connect();

  virtual ~JackSheet();

  virtual int width() const  { return w; }
  virtual int height() const { return h; }

  virtual std::string cellString(int x, int y) const;

  virtual std::string cellString(int x, int y, bool& escaped) const;

  virtual bool cellString(int x, int y, const std::string& str) {
    return cellString(x,y,str,false);
  }

  virtual bool cellString(int x, int y, const std::string& str, bool escaped);

  virtual ColumnInfo getColumnInfo(int x) {
    ColumnType t(col2type[x]);
    t.primaryKey = col2pk[x];
    t.primaryKeySet = true;
    return ColumnInfo(col2sql[x],t);
  }

  virtual int getColumnCount() const {
    return (int)col2sql.size();
  }

  virtual SheetSchema *getSchema() const;

  virtual bool hasExternalColumnNames() const {
    return true;
  }

  std::string getName() const {
    return name;
  }

  virtual ColumnRef insertColumn(const ColumnRef& base, const ColumnInfo& kind) {
    return ColumnRef();
  }

  virtual bool modifyColumn(const ColumnRef& base, const ColumnInfo& kind) {
    return false;
  }

  virtual bool canWrite() { return true; }

private:
  JackSheetSchema *schema;
  Property config;
  void *implementation;
  void *box_implementation;
  void *table_implementation;
  void *cursor_implementation;
  std::string name;
  int w, h;
  std::vector<std::string> col2sql;
  std::vector<std::string> col2type;
  std::vector<bool> col2pk;
  std::vector<void *> savepoint;
  std::vector<void *> col2raw;
  SparseStringSheet cache;
  SparseByteSheet cacheFlag;
 
};

class coopy::store::mdb::JackSheetSchema : public SheetSchema {
public:
  JackSheet *sheet;

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
