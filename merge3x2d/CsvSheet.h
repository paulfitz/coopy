#ifndef SSFOSSIL_CSVSHEET
#define SSFOSSIL_CSVSHEET

#include <vector>
#include <string>

#include <math.h>

class DataSheet {
public:
  virtual int width() = 0;
  virtual int height() = 0;

  virtual std::string cellString(int x, int y) = 0;

  std::string encode() {
    std::string result = "";
    for (int y=0;y<height();y++) {
      for (int x=0;x<width();x++) {
	if (x>0) {
	  result += ",";
	}
	result += encodeCell(cellString(x,y));
      }
      result += "\r\n"; // use Windows encoding, since UNIX is more forgiving
    }
    return result;
  }

  static std::string encodeCell(std::string str) {
    bool need_quote = false;
    for (int i=0; i<str.length(); i++) {
      char ch = str[i];
      if (ch=='"'||ch=='\''||ch==','||ch=='\r'||ch=='\n'||ch=='\t'||ch==' ') {
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

  int width() {
    return w;
  }

  int height() {
    return h;
  }

  T& cell(int x, int y) {
    return arr[y][x];
  }
};

class FloatSheet : public TypedSheet<float> {
public:
  virtual std::string cellString(int x, int y) {
    char buf[256];
    snprintf(buf,sizeof(buf),"%g",cell(x,y));
    return buf;
  }

  void normalize(int first=-1, int last=-1, float sc=0.1);

};

class CsvSheet : public DataSheet {
public:
  std::vector<std::string> rec;
  std::vector<std::vector<std::string> > arr;
  int h, w;
  int th, tw;

  CsvSheet() {
    w = h = 0;
    tw = th = 0;
  }

  bool removeRow(int index) {
    arr.erase(arr.begin()+index);
    if (arr.size()<h) {
      th = h = arr.size();
      return true;
    }
    return false;
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
      w = tw;
    }
    tw = 0;
    th++;
    h = th;
  }

  void clear() {
    tw = th = 0;
    w = h = 0;
    arr.clear();
    rec.clear();
  }

  std::string cell(int x, int y) {
    return arr[y][x];
  }

  int width() {
    return w;
  }

  int height() {
    return h;
  }

  std::string cellString(int x, int y) {
    return arr[y][x];
  }

};

#endif
