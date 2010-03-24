#ifndef SSFOSSIL_CSVSHEET
#define SSFOSSIL_CSVSHEET

#include <vector>
#include <string>

class CsvSheet {
public:
  std::vector<std::string> rec;
  std::vector<std::vector<std::string> > arr;
  int h, w;
  int th, tw;

  CsvSheet() {
    w = h = 0;
    tw = th = 0;
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

  std::string encode() {
    std::string result = "";
    for (int y=0;y<height();y++) {
      for (int x=0;x<width();x++) {
	if (x>0) {
	  result += ",";
	}
	result += encode_cell(cell(x,y));
      }
      result += "\r\n"; // use Windows encoding, since UNIX is more forgiving
    }
    return result;
  }

  static std::string encode_cell(std::string str) {
    bool need_quote = false;
    for (int i=0; i<str.length(); i++) {
      char ch = str[i];
      if (ch=='"'||ch=='\''||ch==','||ch=='\r'||ch=='\n'||ch=='\t') {
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

#endif
