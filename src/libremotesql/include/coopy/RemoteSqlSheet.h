#ifndef COOPY_REMOTESQLSHEET
#define COOPY_REMOTESQLSHEET

#include <coopy/DataSheet.h>
#include <coopy/Property.h>

#include <vector>

namespace coopy {
  namespace store {
    class RemoteSqlSheet;
  }
}

class coopy::store::RemoteSqlSheet : public DataSheet {
public:
  RemoteSqlSheet();

  virtual ~RemoteSqlSheet();

  bool open(const Property& config);

  virtual int width() const  { return w; }
  virtual int height() const { return h; }

  virtual std::string cellString(int x, int y) const;

  virtual bool cellString(int x, int y, const std::string& str);

  virtual ColumnRef moveColumn(const ColumnRef& src, const ColumnRef& base);

  virtual bool deleteColumn(const ColumnRef& column);

  virtual ColumnRef insertColumn(const ColumnRef& base);

  virtual RowRef insertRow(const RowRef& base);

  virtual bool deleteRow(const RowRef& src);

private:
  void *implementation;
  std::string name;
  int w, h;
  std::vector<int> row2sql;
  std::vector<std::string> col2sql;
};

#endif
