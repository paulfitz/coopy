#include <coopy/Mover.h>
#include <coopy/Dbg.h>

#include <algorithm>

using namespace std;
using namespace coopy::cmp;

#include <stdio.h>

bool Mover::move(const std::vector<int>& src, const std::vector<int>& dest, 
		 std::vector<int>& order, 
		 std::vector<int>& forbidden, 
		 int depth) {
  if (depth==0) {
    global_solved = false;
    solutions = 0;
  }
  if (global_solved) {
    if (depth>(int)global_best_order.size()) {
      return false;
    }
  }
  if (solutions>100) {
    return false;
  }

  dbg_printf("* move in %d [%d] : %s / %s / %s\n", depth,
	     solutions,
	     vector2string(src).c_str(),
	     vector2string(dest).c_str(),
	     vector2string(order).c_str());
  vector<int> best_order;
  if (src==dest) {
    return true;
  }
  bool solved = false;

  for (size_t i=0; i<dest.size(); i++) {
    int x = dest[i];
    if (std::find(forbidden.begin(),forbidden.end(),x)!=forbidden.end()) {
      continue;
    }
    vector<int>::const_iterator it = std::find(src.begin(),
					       src.end(),
					       x);
    bool moving = false;
    bool toStart = false;
    int x2 = -1;
    if (it==src.begin()) {
      if (i!=0) {
	x2 = dest[i-1];
	moving = true;
      }
    } else if (i==0) {
      moving = true;
      toStart = true;
    } else if (*(it-1)!=dest[i-1]) {
      x2 = dest[i-1];
      moving = true;
    }
    if (!moving) continue;

    vector<int> norder = order;
    vector<int> nforbidden = forbidden;
    vector<int> nsrc = src;
    nsrc.erase(nsrc.begin()+(it-src.begin()));
    if (toStart) {
      nsrc.insert(nsrc.begin(),x);
    } else {
      vector<int>::iterator it2 = std::find(nsrc.begin(),
					    nsrc.end(),
					    x2);
      nsrc.insert(it2+1,x);
    }
    /*
    if (src[i]==dest[i]) {
      continue;
    }
    nsrc.erase(nsrc.begin()+i);
    nsrc.insert(nsrc.begin()+(it-dest.begin()),x);
    */
    norder.push_back(x);
    nforbidden.push_back(x);
    //if (x2!=-1) {
    //nforbidden.push_back(x2);
    //}
    bool ok = move(nsrc,dest,norder,nforbidden,depth+1);
    if (!ok) continue;
    if (norder.size()<best_order.size() || !solved) {
      best_order = norder;
      solved = true;
      solutions++;
      if (best_order.size()<global_best_order.size() || !global_solved) {
	global_best_order = norder;
	global_solved = true;
      }
    }
  }
  if (!solved) {
    return false;
  }
  order = global_best_order;
  dbg_printf("* move out %d : %s / %s / %s\n", depth,
	     vector2string(src).c_str(),
	     vector2string(dest).c_str(),
	     vector2string(order).c_str());
  return solved;
}
