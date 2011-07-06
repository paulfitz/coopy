#ifndef COOPY_FVAL
#define COOPY_FVAL

#include <set>

#include <coopy/SparseSheet.h>

namespace coopy {
  namespace cmp {
    class FSingleVal;
    class FMultiVal;
  }
}

class coopy::cmp::FSingleVal {
public:
  int index;

  FSingleVal() {
    index = -1;
  }

  void setIndex(int idx) {
    if (index==-1) {
      index = idx;
    } else {
      if (index!=idx) {
	index = -2;
      }
    }
  }
  
  void apply(coopy::store::SparseFloatSheet& match,int row) {
    if (index>=0) {
      match.cell(index,row)++;
    }
  }
};


class coopy::cmp::FMultiVal {
public:
  std::set<int> indices;

  FMultiVal() {
  }

  void setIndex(int idx) {
    if (indices.size()<50) {
      indices.insert(idx);
    }
  }

  void apply(coopy::store::SparseFloatSheet& match,int row) {
    size_t len = indices.size();
    if (len<=0) return;
    if (len>=50) return;
    float delta = 1.0/len;
    for (std::set<int>::const_iterator it=indices.begin();
	 it!=indices.end();
	 it++) {
      match.cell(*it,row)+=delta;
    }
  }
};


#endif
