#ifndef COOPY_STRINGER
#define COOPY_STRINGER

#ifdef WANT_MAP2STRING
#include <map>
#include <string>
template <class T>
std::string map2string(const std::map<T,T>& src) {
  std::string result = "";
  for (typename std::map<T,T>::const_iterator it=src.begin(); it!=src.end(); it++) {
    if (it!=src.begin()) {
      result += " ";
    }
    result += it->first;
    result += ":";
    result += it->second;
  }
  return result;
}
#endif

#ifdef WANT_VECTOR2STRING
#include <vector>
#include <string>
template <class T>
std::string vector2string(const std::vector<T>& src) {
  std::string result = "";
  for (typename std::vector<T>::const_iterator it=src.begin(); it!=src.end(); it++) {
    if (it!=src.begin()) {
      result += " ";
    }
    result += *it;
  }
  return result;
}
#endif

#endif
