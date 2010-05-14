#include "CsvSheet.h"

using namespace std;

Stat FloatSheet::normalize(int first, int last, float sc, bool modify) {
  Stat s;
  int hh = height();
  int ww = width();
  if (first==-1) first = 0;
  if (last==-1) last = ww-1;
  float tot = 0;
  float tot2 = 0;
  int ct = 0;
  for (int i=0; i<hh; i++) {
    for (int j=first; j<=last; j++) {
      float r = cell(j,i);
      tot += r;
      tot2 += r*r;
      ct++;
    }
  }
  if (ct==0) return s;
  float mean = tot;
  float dev = 1;
  mean /= ct;
  dev = tot2 / ct - mean*mean;
  if (dev<0) { 
    dev = 0; 
  } else {
    dev = sqrt(dev);
  }
  //printf("mean %g, dev %g\n", mean, dev);
  if (dev<sc) dev = sc;
  if (modify) {
    for (int i=0; i<hh; i++) {
      for (int j=first; j<=last; j++) {
	float r = cell(j,i);
	r = (r-mean)/dev;
	cell(j,i) = r;
      }
    }
  }
  s.mean = mean;
  s.stddev = dev;
  s.valid = true;
  return s;
}


std::string DataSheet::encode(const std::string& delim) const {
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

std::string DataSheet::encodeCell(const std::string& str, 
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
