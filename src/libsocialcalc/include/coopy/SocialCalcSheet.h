#ifndef COOPY_SOCIALCALCSHEET
#define COOPY_SOCIALCALCSHEET

#include <coopy/DataSheet.h>
#include <coopy/JsWrap.h>
#include <coopy/SparseSheet.h>

namespace coopy {
  namespace store {
    namespace socialcalc {
      class SocialCalcSheet;
    }
  }
}

class coopy::store::socialcalc::SocialCalcSheet : public DataSheet {
private:
  coopy::js::JsWrap js;
  jsval *jssheet;
  int id;
  int w, h;
  SparseStringSheet cache;
  SparseByteSheet cacheFlag;
public:
  SocialCalcSheet() {
    jssheet = 0/*NULL*/;
    w = h = 0;
    id = -1;
  }

  virtual ~SocialCalcSheet() {
    reset();
  }

  static bool jsSetup(coopy::js::JsWrap& j);

  bool select() const;

  bool reset();

  bool setup();

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

  virtual bool deleteRows(const RowRef& first, const RowRef& last);

  virtual RowRef moveRow(const RowRef& src, const RowRef& base);

  virtual bool hasDimension() const {
    return false;
  }

  virtual bool forceWidth(int width) {
    w = width;
    return true;
  }

  bool fromCsvString(const std::string& str);
  bool fromSocialCalcString(const std::string& str);
  std::string toSocialCalcString(const Property& config);

  bool updateSize();

  //virtual Poly<Appearance> getCellAppearance(int x, int y);
  //virtual Poly<Appearance> getRowAppearance(int y);
  //virtual Poly<Appearance> getColAppearance(int x);
};

#endif
