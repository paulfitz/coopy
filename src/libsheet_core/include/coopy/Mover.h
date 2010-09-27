#ifndef COOPY_MOVER
#define COOPY_MOVER

#define WANT_MAP2STRING
#define WANT_VECTOR2STRING
#include <coopy/Stringer.h>

namespace coopy {
  namespace cmp {
    class Mover;
  }
}

class coopy::cmp::Mover {
public:
  void move(const std::vector<int>& src, const std::vector<int>& dest, 
	    std::vector<int>& order, int depth);
};


#endif

