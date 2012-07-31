#ifndef COOPY_DBISQLWRAPPER_INC
#define COOPY_DBISQLWRAPPER_INC

namespace coopy {
  namespace store {
    class SqlTableName;
    class SqlTable;
    class DbiSqlWrapper;
  }
}

#include <string>
#include <vector>

#include <coopy/ColumnInfo.h>
#include <coopy/SheetCell.h>

class coopy::store::SqlTableName {
public:
  std::string prefix;
  std::string name;
};


class coopy::store::DbiSqlWrapper {
public:
  virtual ~DbiSqlWrapper() {}

  virtual std::vector<ColumnInfo> getColumns(const SqlTableName& name) = 0;
  virtual std::string getQuotedTableName(const SqlTableName& name) = 0;
  virtual std::string getQuotedColumnName(const std::string& name) = 0;

  virtual bool begin(const std::string& query) = 0;
  virtual bool read() = 0;
  virtual SheetCell get(int index) = 0;
  virtual bool end() = 0;
  virtual int width() = 0;
};

class coopy::store::SqlTable {
private:
  DbiSqlWrapper *db;
  std::vector<ColumnInfo> columns;
  SqlTableName name;
  std::string quotedTableName;

  void getColumns() {
    if (columns.size()>0) return;
    if (!db) return;
    columns = db->getColumns(name);
  }

public:
  SqlTable(DbiSqlWrapper *db, const SqlTableName& name) {
    this->db = db;
    this->name = name;
  }

  std::vector<std::string> getPrimaryKey() {
    getColumns();
    std::vector<std::string> result;
    for (int i=0; i<(int)columns.size(); i++) {
      ColumnInfo& col = columns[i];
      if (!col.isPrimaryKey()) continue;
      result.push_back(col.getName());
    }
    return result;
  }

  std::vector<std::string> getAllButPrimaryKey() {
    getColumns();
    std::vector<std::string> result;
    for (int i=0; i<(int)columns.size(); i++) {
      ColumnInfo& col = columns[i];
      if (col.isPrimaryKey()) continue;
      result.push_back(col.getName());
    }
    return result;
  }

  std::vector<std::string> getColumnNames() {
    getColumns();
    std::vector<std::string> result;
    for (int i=0; i<(int)columns.size(); i++) {
      ColumnInfo& col = columns[i];
      result.push_back(col.getName());
    }
    return result;
  }


  std::string getQuotedTableName() {
    if (quotedTableName!="") return quotedTableName;
    quotedTableName = db->getQuotedTableName(name);
    return quotedTableName;
  }


  std::string getQuotedColumnName(const std::string& name) {
    return db->getQuotedColumnName(name);
  }

};




#endif

