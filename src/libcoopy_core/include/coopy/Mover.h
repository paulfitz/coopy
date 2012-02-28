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
private:
  int solutions;
  bool global_solved;
  std::vector<int> global_best_order;
public:
  bool move(const std::vector<int>& src, const std::vector<int>& dest, 
	    std::vector<int>& order) {
    std::vector<int> forbidden;
    return move(src,dest,order,forbidden,0);
  }

  bool move(const std::vector<int>& src, const std::vector<int>& dest, 
	    std::vector<int>& order, 
	    std::vector<int>& forbidden, 
	    int depth);
};


#endif

