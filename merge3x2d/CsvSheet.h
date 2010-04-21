#ifndef SSFOSSIL_CSVSHEET
#define SSFOSSIL_CSVSHEET

#include <vector>
#include <string>

#include <math.h>

#include "CsvStyle.h"

class DataSheet {
public:
  virtual int width() const = 0;
  virtual int height() const = 0;

  virtual std::string cellString(int x, int y) const = 0;

  std::string encode(const std::string& delim = ",") const {
    std::string result = "";
    for (int y=0;y<height();y++) {
      for (int x=0;x<width();x++) {
	if (x>0) {
	  result += delim;
	}
	result += encodeCell(cellString(x,y),delim);
      }
      result += "\r\n"; // use Windows encoding, since UNIX is more forgiving
    }
    return result;
  }

  static std::string encodeCell(const std::string& str, 
				const std::string& delim) {
    bool need_quote = false;
    for (int i=0; i<str.length(); i++) {
      char ch = str[i];
      if (ch=='"'||ch=='\''||ch==delim[0]||ch=='\r'||ch=='\n'||ch=='\t'||ch==' ') {
	need_quote = true;
	break;
      }
    }
    std::string result = "";
    if (need_quote) { result += '"'; }
    for (int i=0; i<str.length(); i++) {
      char ch = str[i];
      if (ch=='"') {
	result += '"';
      }
      result += ch;
    }
    if (need_quote) { result += '"'; }
    return result;
  }
};

template <class T>
class TypedSheet : public DataSheet {
public:
  std::vector<std::vector<T> > arr;
  int h, w;

  TypedSheet() {
    h = w = 0;
  }
  
  void resize(int w, int h, const T& zero) {
    arr.clear();
    for (int i=0; i<h; i++) {
      arr.push_back(std::vector<T>());
      std::vector<T>& lst = arr.back();
      for (int j=0; j<w; j++) {
	lst.push_back(zero);
      }
    }
    this->h = h;
    this->w = w;
  }

  int width() const {
    return w;
  }

  int height() const {
    return h;
  }

  T& cell(int x, int y) {
    return arr[y][x];
  }

  const T& cell(int x, int y) const {
    return arr[y][x];
  }
};

class Stat {
public:
  double mean;
  double stddev;
  bool valid;

  Stat() {
    mean = stddev = 0;
    valid = false;
  }

  bool isValid() { 
    return valid;
  }

  double evaluate(double x) {
    if (stddev<0.001) {
      return 1;
    }
    return (x-mean)/stddev;
  }

  double rescale(double factor) {
    mean *= factor;
    stddev *= factor;
  }
};

class FloatSheet : public TypedSheet<float> {
public:
  virtual std::string cellString(int x, int y) const {
    char buf[256];
    snprintf(buf,sizeof(buf),"%g",cell(x,y));
    return buf;
  }

  Stat normalize(int first=-1, int last=-1, float sc=0.1, bool modify = true);
  
  void rescale(double factor) {
    int w = width();
    int h = height();
    for (int x=0; x<w; x++) {
      for (int y=0; y<h; y++) {
	cell(x,y) *= factor;
      }
    }
  }
};

class IntSheet : public TypedSheet<int> {
public:
  virtual std::string cellString(int x, int y) const {
    char buf[256];
    snprintf(buf,sizeof(buf),"%d",cell(x,y));
    return buf;
  }
};

class CsvSheet : public DataSheet {
private:
  std::vector<std::string> rec;
  std::vector<std::vector<std::string> > arr;
  int h, w;
  int th, tw;
  bool valid;
  CsvStyle style;
 public:

  CsvSheet() {
    w = h = 0;
    tw = th = 0;
    valid = true;
  }

  const CsvStyle& getStyle() {
    return style;
  }

  void setStyle(const CsvStyle& style) {
    this->style = style;
  }

  bool removeRow(int index) {
    arr.erase(arr.begin()+index);
    if (arr.size()<h) {
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
    //printf("Add record\n");
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

  std::string& cell(int x, int y) {
    return arr[y][x];
  }

  const std::string& cell(int x, int y) const {
    return arr[y][x];
  }

  int width() const {
    return w;
  }

  int height() const {
    return h;
  }

  std::string cellString(int x, int y) const {
    return arr[y][x];
  }

  bool isValid() const {
    return valid;
  }
};

#endif
