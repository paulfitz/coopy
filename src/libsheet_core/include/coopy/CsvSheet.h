#ifndef SSFOSSIL_CSVSHEET
#define SSFOSSIL_CSVSHEET

#include <coopy/TypedSheet.h>

#include <vector>
#include <string>

namespace coopy {
  namespace store {
    class CsvSheet;
  }
}

class coopy::store::CsvSheet : public EscapedTypedSheet<std::string> {
private:
  std::vector<pairCellType> rec;
  int th, tw;
  bool valid;
  SheetStyle style;
  bool strict;
  Poly<SheetSchema> pSchema;
public:
  using EscapedTypedSheet<std::string>::resize;
  using EscapedTypedSheet<std::string>::insertRow;

  CsvSheet() {
    tw = th = 0;
    valid = true;
    strict = true;
  }

  virtual SheetSchema *getSchema() const {
    return pSchema.getContent();
  }

  void setSchema(Poly<SheetSchema> pSchema) {
    this->pSchema = pSchema;
  }

  const SheetStyle& getStyle() {
    return style;
  }

  void setStyle(const SheetStyle& style) {
    this->style = style;
  }

  bool setStrict(bool strict) {
    this->strict = strict;
  }

  bool isStrict() {
    return strict;
  }

  // Deprecated, should migrate to deleteRow
  bool removeRow(int index) {
    s.arr.erase(s.arr.begin()+index);
    if (s.arr.size()<(size_t)s.h) {
      th = s.h = s.arr.size();
      return true;
    }
    return false;
  }

  // Deprecated
  bool insertRow(int index, int width = -1) {
    std::vector<pairCellType> rec;
    if (width == -1) { width = s.w; }
    for (int i=0; i<width; i++) {
      rec.push_back(pairCellType("",true));
    }
    if (index == -1 || index>=height()) {
      s.arr.push_back(rec);
    } else {
      s.arr.insert(s.arr.begin()+index,rec);
    }
    th = s.h = s.arr.size();
    tw = s.w = rec.size();
    return true;
  }

  void addField(const char *s, bool escaped);

  void addField(const coopy::store::SheetCell& c) {
    addField(c.text.c_str(),c.escaped);
  }

  void addField(const char *s, int len, bool escaped) {
    std::string str(s,len);
    str += "\0";
    addField(str.c_str(),escaped);
  }

  void addRecord() {
    s.arr.push_back(rec);
    rec.clear();
    if (s.w!=tw && s.w!=0) {
      valid = false;
    }
    if (tw>s.w) {
      s.w = tw;
    }
    tw = 0;
    th++;
    s.h = th;
  }

  void addRow(CsvSheet& alt, int row) {
    for (int i=0; i<alt.width(); i++) {
      const pairCellType& p = alt.pcell(i,row);
      addField(p.first.c_str(),p.second);
    }
    addRecord();
  }

  void clear() {
    tw = th = 0;
    s.w = s.h = 0;
    s.arr.clear();
    rec.clear();
    valid = true;
  }

  bool isValid() const {
    return valid;
  }

  virtual std::string cellString(int x, int y) const {
    if (valid) { return cell(x,y); }
    if (s.arr[y].size()>x) {
      return cell(x,y);
    }
    return "";
  }

  virtual std::string cellString(int x, int y, bool& escaped) const {
    if (valid) {
      const pairCellType& c = pcell(x,y);
      escaped = c.second;
      return c.first;
    }
    if (s.arr[y].size()>x) {
      const pairCellType& c = pcell(x,y);
      escaped = c.second;
      return c.first;
    }
    // for CSV-friendliness, return "" instead of NULL
    // for cells that did not get appended.  This "feature"
    // is only used by the patch generator.
    escaped = false;
    return ""; 
  }

  virtual bool cellString(int x, int y, const std::string& str) {
    cell(x,y) = str;
    return true;
  }

  virtual bool cellString(int x, int y, const std::string& str, 
			  bool escaped) {
    pairCellType& p = pcell(x,y);
    p.first = str;
    p.second = escaped;
    return true;
  }

  const CsvSheet& copy(const CsvSheet& alt) {
    s.arr = alt.s.arr;
    s.h = alt.s.h;
    s.w = alt.s.w;
    return *this;
  }

  const CsvSheet& copy(const DataSheet& alt) {
    clear();
    for (int i=0; i<alt.height(); i++) {
      for (int j=0; j<alt.width(); j++) {
	addField(alt.cellSummary(j,i));
      }
      addRecord();
    }
    return *this;
  }

  virtual bool canResize() { return true; }

  virtual bool resize(int w, int h) {
    clear();
    SheetCell cell;
    for (int i=0; i<h; i++) {
      for (int j=0; j<w; j++) {
	addField(cell);
      }
      addRecord();
    }
    s.w = w;
    s.h = h;
    return true;
  }

  bool setWidth(int w) {
    s.w = w;
    return true;
  }

  virtual std::string getDescription() const {
    return "csv";
  }
};


#endif
