#ifndef COOPY_SHEETSCHEMA
#define COOPY_SHEETSCHEMA

#include <coopy/ColumnInfo.h>
#include <coopy/RefCount.h>

#include <vector>
#include <string>

namespace coopy {
  namespace store {
    class SheetSchema;
    class SimpleSheetSchema;
  }
}

class coopy::store::SheetSchema : public RefCount {
public:
  virtual ~SheetSchema() {
  }

  virtual ColumnInfo getColumnInfo(int x) const {
    return ColumnInfo();
  }

  virtual int headerHeight() const {
    return 0;
  }

  virtual int getColumnCount() const {
    return -1;
  }

  virtual int getColumnIndexByName(const char *name) const {
    return -1;
  }

  virtual bool providesPrimaryKeys() const {
    return false;
  }

  virtual std::string getSheetName() const {
    return "";
  }

  virtual SheetSchema *clone() const;
};

class coopy::store::SimpleSheetSchema : public SheetSchema {
private:
  std::string sheetName;
  std::vector<std::string> columns;
  std::vector<ColumnType> kinds;
  int hh;
public:
  SimpleSheetSchema() {
    hh = 0;
  }

  bool copy(const SheetSchema& ss) {
    sheetName = ss.getSheetName();
    hh = ss.headerHeight();
    int ct = ss.getColumnCount();
    columns.clear();
    kinds.clear();
    for (int i=0; i<ct; i++) {
      ColumnInfo c = ss.getColumnInfo(i);
      columns.push_back(c.getName());
      kinds.push_back(c.getColumnType());
    }
    return true;
  }
  

  void setHeaderHeight(int hh) {
    this->hh = hh;
  }

  virtual int headerHeight() const {
    return hh;
  }

  void setSheetName(const char *name) {
    sheetName = name;
  }

  void addColumn(const char *name) {
    columns.push_back(name);
    kinds.push_back(ColumnType());
  }

  void addColumn(const char *name, const ColumnType& kind) {
    columns.push_back(name);
    kinds.push_back(kind);
  }

  virtual ColumnInfo getColumnInfo(int x) const {
    return ColumnInfo(columns[x],kinds[x]);
  }

  virtual int getColumnCount() const {
    return columns.size();
  }

  virtual int getColumnIndexByName(const char *name) const {
    return -1;
  }

  virtual bool providesPrimaryKeys() const {
    return false;
  }

  virtual std::string getSheetName() const {
    return sheetName;
  }
};

#endif
