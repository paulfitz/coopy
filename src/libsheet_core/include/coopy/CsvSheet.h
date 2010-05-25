#ifndef SSFOSSIL_CSVSHEET
#define SSFOSSIL_CSVSHEET

#include <coopy/TextSheet.h>

#include <vector>
#include <string>


class CsvSheet : public TextSheet {
private:
  std::vector<std::string> rec;
  int th, tw;
  bool valid;
  SheetStyle style;
public:

  CsvSheet() {
    tw = th = 0;
    valid = true;
  }

  const SheetStyle& getStyle() {
    return style;
  }

  void setStyle(const SheetStyle& style) {
    this->style = style;
  }

  bool removeRow(int index) {
    arr.erase(arr.begin()+index);
    if (arr.size()<(size_t)h) {
      th = h = arr.size();
      return true;
    }
    return false;
  }

  bool insertRow(int index, int width = -1) {
    std::vector<std::string> rec;
    if (width == -1) { width = w; }
    for (int i=0; i<width; i++) {
      rec.push_back("");
    }
    if (index == -1 || index>=height()) {
      arr.push_back(rec);
    } else {
      arr.insert(arr.begin()+index,rec);
    }
    th = h = arr.size();
    tw = w = rec.size();
    return true;
  }

  void addField(const char *s) {
    rec.push_back(s);
    tw++;
  }

  void addField(const char *s, int len) {
    std::string str(s,len);
    str += "\0";
    addField(str.c_str());
  }

  void addRecord() {
    arr.push_back(rec);
    rec.clear();
    if (tw>w) {
      if (w!=tw && w!=0) {
	valid = false;
      }
      w = tw;
    }
    tw = 0;
    th++;
    h = th;
  }

  void addRow(CsvSheet& alt, int row) {
    for (int i=0; i<alt.width(); i++) {
      addField(alt.cell(i,row).c_str());
    }
    addRecord();
  }

  void clear() {
    tw = th = 0;
    w = h = 0;
    arr.clear();
    rec.clear();
    valid = true;
  }

  bool isValid() const {
    return valid;
  }
};


#endif
