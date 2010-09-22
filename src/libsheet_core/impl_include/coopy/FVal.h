#ifndef COOPY_FVAL
#define COOPY_FVAL

namespace coopy {
  namespace cmp {
    class FVal;
  }
}

class coopy::cmp::FVal {
public:
  int ct;
  int index;

  FVal() {
    ct = 0;
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
};


#endif
