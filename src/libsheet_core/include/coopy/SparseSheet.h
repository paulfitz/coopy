#ifndef COOPY_SPARSESHEET
#define COOPY_SPARSESHEET

#include <coopy/EfficientMap.h>
#include <coopy/DataSheet.h>
#include <coopy/IntSheet.h>
#include <coopy/FloatSheet.h>
#include <coopy/Stat.h>

#include <set>

namespace coopy {
  namespace store {
    template <class T> class SparseSheet;
    class SparseFloatSheet;
    class SparseIntSheet;
    class SparseByteSheet;
    class SparseStringSheet;
  }
}

template <class T>
class coopy::store::SparseSheet : public DataSheet {
 private:
  std::set<int> empty_set;
  T zero;
public:
  efficient_map<long long,T> data;
  efficient_map<long, std::set<int> > row;
  int h, w;

  SparseSheet() {
    h = w = 0;
  }
  

  const SparseSheet& operator = (const SparseSheet& alt) {
    data = alt.data;
    row = alt.row;
    h = alt.h;
    w = alt.w;
    zero = alt.zero;
    return *this;
  }

  bool resize(int w, int h, const T& zero) {
    data.clear();
    row.clear();
    this->zero = zero;
    this->h = h;
    this->w = w;
    return true;
  }

  void reheight(int h) {
    if (h>=this->h) {
      this->h = h;
    }
  }

  void clear() {
    data.clear();
    row.clear();
  }

  int width() const {
    return w;
  }

  int height() const {
    return h;
  }

  const T *pcell_const(int x, int y) const {
    typename efficient_map<long long,T>::const_iterator it = data.find(((long long)y)*w+x);
    if (it==data.end()) {
      return 0/*NULL*/;
    }
    return &(it->second);
  }

  T *pcell(int x, int y) {
    typename efficient_map<long long,T>::const_iterator it = data.find(((long long)y)*w+x);
    if (it==data.end()) {
      return 0/*NULL*/;
    }
    return &(it->second);
  }

  const T& cell(int x, int y) const {
    typename efficient_map<long long,T>::const_iterator it = data.find(((long long)y)*w+x);
    if (it==data.end()) {
      return zero;
    }
    return it->second;
  }

  const T& cell_const(int x, int y) const {
    typename efficient_map<long long,T>::const_iterator it = data.find(((long long)y)*w+x);
    if (it==data.end()) {
      return zero;
    }
    return it->second;
  }

  T& cell(int x, int y) {
    typename efficient_map<long long,T>::iterator it = data.find(((long long)y)*w+x);
    if (it==data.end()) {
      T& result = data[((long long)y)*w+x];
      row[y].insert(x);
      result = zero;
      return result;
    }
    return it->second;
  }

  bool remove(int x, int y) {
    typename efficient_map<long long,T>::iterator it = data.find(((long long)y)*w+x);
    if (it!=data.end()) {
      data.erase(it);
      return true;
    }
    return false;
  }

  const std::set<int>& getCellsInRow(int y) const {
    typename efficient_map<long, std::set<int> >::const_iterator it = 
      row.find(y);
    if (it==row.end()) {
      return empty_set;
    }
    return it->second;
  }

  virtual std::string getDescription() const {
    return "sparse";
  }
};


class coopy::store::SparseStringSheet : public SparseSheet<std::string> {
public:
  using SparseSheet<std::string>::resize;

  virtual std::string cellString(int x, int y) const {
    return cell(x,y);
  }
};

class coopy::store::SparseFloatSheet : public SparseSheet<float> {
public:
  using SparseSheet<float>::resize;

  virtual std::string cellString(int x, int y) const {
    char buf[256];
    const float& v = cell(x,y);
    snprintf(buf,sizeof(buf),"%g",v);
    return buf;
  }

  Stat normalize(int first=-1, int last=-1, float sc=0.1, bool modify = true);
  
  void rescale(double factor) {
    for (efficient_map<long long,float>::iterator it=data.begin(); it!=data.end(); it++) {
      it->second *= factor;
    }
  }

  void findBest(IntSheet& bestIndex, FloatSheet& bestValue, FloatSheet& bestInc);

  bool resize(int w, int h) {
    float zero = 0;
    return resize(w,h,zero);
  }

  bool canResize() {
    return true;
  }

  const float& operator()(int x, int y) const {
    return cell(x,y);
  }

  float& operator()(int x, int y) {
    return cell(x,y);
  }
};

class coopy::store::SparseIntSheet : public SparseSheet<int> {
public:
  using SparseSheet<int>::resize;

  virtual std::string cellString(int x, int y) const {
    const int& v = cell(x,y);
    return IntSheet::int2string(v);
  }

  bool resize(int w, int h) {
    int zero = 0;
    return resize(w,h,zero);
  }

  bool canResize() {
    return true;
  }

  const int& operator()(int x, int y) const {
    return cell(x,y);
  }

  int& operator()(int x, int y) {
    return cell(x,y);
  }
};


class coopy::store::SparseByteSheet : public SparseSheet<unsigned char> {
public:
  using SparseSheet<unsigned char>::resize;

  virtual std::string cellString(int x, int y) const {
    const unsigned char& v = cell(x,y);
    return IntSheet::int2string(v);
  }

  bool resize(int w, int h) {
    unsigned char zero = 0;
    return resize(w,h,zero);
  }

  bool canResize() {
    return true;
  }

  const unsigned char& operator()(int x, int y) const {
    return cell(x,y);
  }

  unsigned char& operator()(int x, int y) {
    return cell(x,y);
  }
};

#endif
