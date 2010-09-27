#include <coopy/Mover.h>
#include <coopy/Dbg.h>

#include <algorithm>

using namespace std;
using namespace coopy::cmp;

#include <stdio.h>

void Mover::move(const std::vector<int>& src, const std::vector<int>& dest, 
		 std::vector<int>& order, int depth) {
  dbg_printf("* move in %d : %s / %s / %s\n", depth,
	     vector2string(src).c_str(),
	     vector2string(dest).c_str(),
	     vector2string(order).c_str());
  vector<int> best_order;
  if (src==dest) {
    return;
  }
  bool solved = false;

  for (size_t i=0; i<src.size(); i++) {
    if (src[i]==dest[i]) {
      continue;
    }
    vector<int> norder = order;
    vector<int> nsrc = src;
    int x = src[i];
    nsrc.erase(nsrc.begin()+i);
    vector<int>::const_iterator it = std::find(dest.begin(),
					       dest.end(),
					       x);
    nsrc.insert(nsrc.begin()+(it-dest.begin()),x);
    norder.push_back(i);
    move(nsrc,dest,norder,depth+1);
    if (norder.size()<best_order.size() || !solved) {
      best_order = norder;
      solved = true;
    }
  }
  order = best_order;
  dbg_printf("* move out %d : %s / %s / %s\n", depth,
	     vector2string(src).c_str(),
	     vector2string(dest).c_str(),
	     vector2string(order).c_str());
}
