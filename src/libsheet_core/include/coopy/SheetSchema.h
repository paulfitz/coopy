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
};

class coopy::store::SimpleSheetSchema : public SheetSchema {
private:
  std::vector<std::string> columns;
public:
  void addColumn(const char *name) {
    columns.push_back(name);
  }

  virtual ColumnInfo getColumnInfo(int x) const {
    return ColumnInfo(columns[x]);
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
};

#endif
