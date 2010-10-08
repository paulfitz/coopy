#ifndef COOPY_TYPEDSHEET
#define COOPY_TYPEDSHEET

#include <coopy/DataSheet.h>

#include <vector>
#include <string>

namespace coopy {
  namespace store {
    template <class T> class TypedSheet;
  }
}

template <class T>
class coopy::store::TypedSheet : public DataSheet {
public:
  std::vector<std::vector<T> > arr;
  int h, w;

  TypedSheet() {
    h = w = 0;
  }

  void clear() {
    arr.clear();
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

  virtual bool deleteColumn(const ColumnRef& column) {
    int offset = column.getIndex();
    if (offset<0||offset>=(int)arr.size()) return false;
    for (int i=0; i<(int)arr.size(); i++) {
      arr[i].erase(arr[i].begin()+offset);
    }
    w--;
    return true;
  }

  virtual ColumnRef insertColumn(const ColumnRef& base) {
    int offset = base.getIndex();
    if (offset>=(int)arr.size()) return ColumnRef();
    T t;
    for (int i=0; i<(int)arr.size(); i++) {
      if (offset>=0) {
	arr[i].insert(arr[i].begin()+offset,t);
      } else {
	arr[i].push_back(t);
      }
    }
    w++;
    return ColumnRef((offset>=0)?offset:ColumnRef(w-1));
  }

  virtual ColumnRef moveColumn(const ColumnRef& src, const ColumnRef& base) {
    int offset = base.getIndex();
    if (offset>=(int)arr.size()) return ColumnRef();
    int offset_del = src.getIndex();
    if (offset_del<0||offset_del>=(int)arr.size()) return ColumnRef();
    int final = offset;
    if (offset<=offset_del) {
      offset_del++;
    } else {
      final--;
    }
    T t;
    for (int i=0; i<(int)arr.size(); i++) {
      if (offset>=0) {
	arr[i].insert(arr[i].begin()+offset,t);
      } else {
	arr[i].push_back(t);
      }
      arr[i].erase(arr[i].begin()+offset_del);
    }
    if (offset<0) {
      return ColumnRef(w-1);
    }
    return ColumnRef(final);
  }
};


#endif
