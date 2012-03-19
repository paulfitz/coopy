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
  int index2;

  FSingleVal() {
    index = -1;
    index2 = -1;
  }

  void setIndex(int idx, bool alt) {
    if (alt) {
      if (index2==-1) {
	index2 = idx;
      } else {
	if (index2!=idx) {
	  index2 = -2;
	  index = -2;
	}
      }
    } else {
      if (index==-1) {
	index = idx;
      } else {
	if (index!=idx) {
	  index = -2;
	  index2 = -2;
	}
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
  std::set<int> indices, indices2;
  //size_t len2;

  FMultiVal() {
    //len2 = 0;
  }

  void setIndex(int idx, bool alt) {
    if (alt) {
      if (indices2.size()<50) {
	indices2.insert(idx);
      }
      //if (len2<50) len2++;
    } else {
      if (indices.size()<50) {
	indices.insert(idx);
      }
    }
  }

  void apply(coopy::store::SparseFloatSheet& match,int row) {
    size_t len = indices.size();
    size_t len2 = indices2.size();
    if (len2>len) len = len2;
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
