#ifndef COOPY_SHEETSCHEMA
#define COOPY_SHEETSCHEMA

#include <coopy/ColumnInfo.h>
#include <coopy/ColumnRef.h>
#include <coopy/RefCount.h>
#include <coopy/Dbg.h>

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
    for (int i=0; i<getColumnCount(); i++) {
      ColumnInfo info = getColumnInfo(i);
      if (info.getName()==name) {
	return i;
      }
    }
    return -1;
  }

  virtual bool providesPrimaryKeys() const {
    return false;
  }

  virtual std::string getSheetName() const {
    return "";
  }

  virtual bool hasSheetName() const {
    return true;
  }

  virtual SheetSchema *clone() const;

  virtual bool isGuess() const {
    return false;
  }

  virtual bool addedHeader() {
    fprintf(stderr, "Don't know what to do with header\n");
    COOPY_ASSERT(1==0);
  }

  virtual bool deleteColumn(const ColumnRef& column) {
    return false;
  }

  virtual bool renameColumn(int x, const char *name) {
    fprintf(stderr, "Don't know how to rename column\n");
    return false;
  }


};

class coopy::store::SimpleSheetSchema : public SheetSchema {
private:
  std::string sheetName;
  std::vector<std::string> columns;
  std::vector<ColumnType> kinds;
  int hh;
  bool guessed;
  bool hasName;
public:
  SimpleSheetSchema() {
    hh = 0;
    guessed = false;
    hasName = true;
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
    guessed = ss.isGuess();
    return true;
  }

  virtual bool addedHeader() {
    hh++;
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

  virtual bool renameColumn(int x, const char *name) {
    columns[x] = name;
    return true;
  }

  virtual int getColumnCount() const {
    return columns.size();
  }

  virtual bool providesPrimaryKeys() const {
    return false;
  }

  virtual std::string getSheetName() const {
    return sheetName;
  }

  virtual bool isGuess() const {
    return guessed;
  }

  void setGuess(bool flag) {
    guessed = flag;
  }

  void setHasSheetName(bool flag) {
    hasName = flag;
  }

  virtual bool hasSheetName() const {
    return hasName;
  }

  virtual bool deleteColumn(const ColumnRef& column) {
    columns.erase(columns.begin()+column.getIndex());
    kinds.erase(kinds.begin()+column.getIndex());
    return true;
  }

};

#endif
