#ifndef COOPY_VITERBI_INC
#define COOPY_VITERBI_INC

#include <assert.h>
#include <coopy/SparseSheet.h>
#include <coopy/Dbg.h>

namespace coopy {
  namespace cmp {
    class Viterbi;
  }
}

class coopy::cmp::Viterbi {
public:
  int K;
  int T;
  int index;
  int mode;
  int path_valid;
  float best_cost;
  coopy::store::SparseFloatSheet cost;
  coopy::store::SparseIntSheet src;
  coopy::store::SparseIntSheet path;

  Viterbi() {
      K = T = 0;
      reset();
    }

  void setSize(int states, int sequence_length);

  void reset()
    {
      index = 0;
      mode = 0;
      path_valid = 0;
      best_cost = 0;
    }

  void assertMode(int n_mode);

  void addTransition(int s0, int s1, float c);

  void endTransitions() {
    path_valid = 0;
    assertMode(0);
  }

  void beginTransitions() {
    path_valid = 0;
    assertMode(1);
  }

  void calculatePath();

  void showPath();

  int length() {
    if (index>0) {
      calculatePath();
    }
    return index;
  }

  int getPath(int i) {
    calculatePath();
    COOPY_ASSERT(i<index);
    return path(0,i);
  }

  int operator() (int i) {
    return getPath(i);
  }

  float getCost() {
    calculatePath();
    return best_cost;
  }
};


#endif
