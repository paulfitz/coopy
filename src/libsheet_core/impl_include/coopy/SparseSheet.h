#ifndef COOPY_SPARSESHEET
#define COOPY_SPARSESHEET

#include <coopy/CsvSheet.h>
#include <coopy/EfficientMap.h>

template <class T>
class SparseSheet : public DataSheet {
public:
  efficient_map<long long,T> data;
  int h, w;
  T zero;

  SparseSheet() {
    h = w = 0;
  }
  

  const SparseSheet& operator = (const SparseSheet& alt) {
    data = alt.data;
    h = alt.h;
    w = alt.w;
    zero = alt.zero;
    return *this;
  }

  void resize(int w, int h, const T& zero) {
    data.clear();
    this->zero = zero;
    this->h = h;
    this->w = w;
  }

  int width() const {
    return w;
  }

  int height() const {
    return h;
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
      result = zero;
      return result;
    }
    return it->second;
  }
};

class SparseFloatSheet : public SparseSheet<float> {
public:
  virtual std::string cellString(int x, int y) const {
    char buf[256];
    const float& v = cell(x,y);
    snprintf(buf,sizeof(buf),"%g");
    return buf;
  }

  Stat normalize(int first=-1, int last=-1, float sc=0.1, bool modify = true);
  
  void rescale(double factor) {
    for (efficient_map<long long,float>::iterator it=data.begin(); it!=data.end(); it++) {
      it->second *= factor;
    }
  }

  void findBest(IntSheet& bestIndex, FloatSheet& bestValue, FloatSheet& bestInc);
};


#endif
