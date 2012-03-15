#ifndef COOPY_GNUMERICSHEET
#define COOPY_GNUMERICSHEET

#include <coopy/DataSheet.h>

namespace coopy {
  namespace store {
    namespace gnumeric {
      class GnumericSheet;
    }
  }
}

class coopy::store::gnumeric::GnumericSheet : public DataSheet {
public:
  GnumericSheet(void *sheet);

  virtual ~GnumericSheet();

  virtual int width() const  { return w; }
  virtual int height() const { return h; }


  virtual SheetCell cellSummary(int x, int y) const;

  virtual bool cellSummary(int x, int y, const SheetCell& c);

  virtual std::string cellString(int x, int y) const {
    SheetCell cell = cellSummary(x,y);
    return cell.text;
  }

  virtual std::string cellString(int x, int y, bool& escaped) const {
    SheetCell cell = cellSummary(x,y);
    escaped = cell.escaped;
    return cell.text;
  }

  virtual bool cellString(int x, int y, const std::string& str) {
    return cellString(x,y,str,false);
  }

  virtual bool cellString(int x, int y, const std::string& str, bool escaped) {
    SheetCell cell(str,escaped);
    return cellSummary(x,y,cell);
  }

  virtual ColumnRef moveColumn(const ColumnRef& src, const ColumnRef& base);

  virtual bool deleteColumn(const ColumnRef& column);

  virtual ColumnRef insertColumn(const ColumnRef& base);

  virtual ColumnRef insertColumn(const ColumnRef& base, const ColumnInfo& kind);

  virtual bool modifyColumn(const ColumnRef& base, const ColumnInfo& kind);

  virtual RowRef insertRow(const RowRef& base);

  virtual bool deleteRow(const RowRef& src);

  virtual bool deleteRows(const RowRef& first, const RowRef& last);

  virtual RowRef moveRow(const RowRef& src, const RowRef& base);


  virtual bool hasDimension() const {
    return false;
  }

  virtual bool forceWidth(int width) {
    w = width;
    return true;
  }

  virtual bool deleteData(int offset=0);

  virtual Poly<Appearance> getCellAppearance(int x, int y);

  virtual Poly<Appearance> getRowAppearance(int y);

  virtual Poly<Appearance> getColAppearance(int x);

  virtual std::string getDescription() const {
    return "gnumeric";
  }

private:
  void *implementation;
  int w, h;
};

#endif
