#ifndef COOPY_SHEETCELL_INC
#define COOPY_SHEETCELL_INC

#include <string>
#include <coopy/RefCount.h>

namespace coopy {
  namespace store {
    class SheetCell;
    class SheetCellMeta;
    class SheetCellSimpleMeta;
  }
}

enum {
  SHEET_CELL_NONE,
  SHEET_CELL_URL,
};

class coopy::store::SheetCellMeta : public RefCount {
public:
  virtual ~SheetCellMeta() {
  }

  virtual bool getCode() const { return SHEET_CELL_NONE; }

  virtual bool isUrl() const { return false; }

  virtual std::string getUrl() const { return ""; }

  virtual std::string getText() const { return ""; }
};

class coopy::store::SheetCellSimpleMeta : public SheetCellMeta {
public:
  std::string url, txt;

  virtual bool getCode() const { 
    return isUrl()?SHEET_CELL_URL:SHEET_CELL_NONE; 
  }

  virtual bool isUrl() const { return url!=""; }

  virtual std::string getUrl() const { return url; }

  virtual std::string getText() const { return txt; }
};


class coopy::store::SheetCell {
public:
  std::string text;
  bool escaped;
  Poly<SheetCellMeta> meta;

  SheetCell() {
    escaped = true;
  }

  SheetCell(const char *text, bool escaped) : text(text), escaped(escaped) {
  }

  SheetCell(const std::string& text, bool escaped) : text(text), 
    escaped(escaped) {
  }

  SheetCell(int x);

  bool operator==(const SheetCell& alt) const {
    if (escaped!=alt.escaped) return false;
    return text==alt.text;
  }

  bool operator!=(const SheetCell& alt) const {
    if (escaped!=alt.escaped) return true;
    return text!=alt.text;
  }
  
  std::string toString() const {
    if (!meta.isValid()) {
      if (!escaped) {
	return text;
      }
      return std::string("{") + text + "}";
    }
    if (meta->isUrl()) {
      return std::string("url:") + meta->getUrl() + "::" + text;
    }
    return text;
  }

  int asInt() const;
  
  static SheetCell makeInt(int x);
};

#endif


