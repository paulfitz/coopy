#ifndef COOPY_TYPEDSHEET
#define COOPY_TYPEDSHEET

#include <coopy/DataSheet.h>

#include <vector>
#include <string>

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

  // still need to define how T is serialized
  virtual std::string cellString(int x, int y) const = 0;
};


#endif
