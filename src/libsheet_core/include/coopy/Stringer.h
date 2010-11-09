#ifndef COOPY_STRINGER
#define COOPY_STRINGER

#include <string>
#include <list>
#include <coopy/SheetCell.h>

#ifdef WANT_MAP2STRING
#include <map>
#include <string>
std::string stringify(const std::string& x);
std::string stringify(const coopy::store::SheetCell& x);
template <class T1, class T2>
std::string map2string(const std::map<T1,T2>& src) {
  std::string result = "";
  for (typename std::map<T1,T2>::const_iterator it=src.begin(); it!=src.end(); it++) {
    if (it!=src.begin()) {
      result += " ";
    }
    result += it->first;
    result += ":";
    result += stringify(it->second);
  }
  return result;
}
#endif

#ifdef WANT_VECTOR2STRING
#include <vector>
#include <string>
std::string stringer_encoder(bool x);
std::string stringer_encoder(int x);
std::string stringer_encoder(const std::string& x);
std::string stringer_encoder(const coopy::store::SheetCell& x);
template <class T>
std::string vector2string(const std::vector<T>& src) {
  std::string result = "";
  for (typename std::vector<T>::const_iterator it=src.begin(); it!=src.end(); it++) {
    if (it!=src.begin()) {
      result += " ";
    }
    result += stringer_encoder(*it);
  }
  return result;
}
#endif


class Stringer {
public:
  static void split(const std::string& str, 
		    const std::string& delimiters, 
		    std::list<std::string>& tokens) {
    size_t lastPos = str.find_first_not_of(delimiters, 0);
    size_t pos = str.find_first_of(delimiters, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos) {
      tokens.push_back(str.substr(lastPos, pos - lastPos));
      lastPos = str.find_first_not_of(delimiters, pos);
      pos = str.find_first_of(delimiters, lastPos);
    }
  }


  static void replace(std::string& str, const std::string& old, 
		      const std::string& rep) {
    size_t pos = 0;
    while((pos = str.find(old, pos)) != std::string::npos) {
      str.replace(pos, old.length(), rep);
      pos += rep.length();
    }
  }
};

#endif
