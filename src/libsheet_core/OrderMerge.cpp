#include <coopy/OrderMerge.h>
#include <coopy/Dbg.h>

#include <list>
#include <map>

using namespace std;
using namespace coopy::cmp;

/*

  Order is currently not quite right.  Inserts and deletes are ok,
  but no analysis is done yet to determine which of local vs remote
  orders should win.

 */

void OrderMerge::process(int ilocal, int iremote,
			 int& base_local, int& base_remote,
			 int stop_local, int stop_remote) {
  //dbg_printf("process %d %d / %d %d / %d %d\n", ilocal, iremote, 
  //base_local, base_remote, stop_local, stop_remote);
  while (true) {
    dbg_printf("--- process %d %d / %d %d / %d %d\n", ilocal, iremote, 
	       base_local, base_remote, stop_local, stop_remote);
    if (ilocal>=stop_local &&
	iremote>=stop_remote) {
      break;
    }
    int _l = ilocal;
    if (ilocal<stop_local) {
      if (_l<xlocal.height()) {
	if (xlocal.cell(0,_l)==0) {
	  int _lp = order_local.b2a(_l);
	  if (_lp!=-1) {
	    int _lpr = order_remote.a2b(_lp);
	    if (_lpr!=-1) {
	      process(base_local,base_remote,base_local,base_remote,ilocal,_lpr);
	      dbg_printf("Local unit %d exists in pivot at %d and in remote at %d\n", _l, _lp, _lpr);
	      accum.push_back(MatchUnit(_lp,_l,_lpr,false,MATCH_UNIT_PRESERVE));
	      if (_lpr>=0) {
		xremote.cell(0,_lpr) = 1;
	      }
	      if (_l>=0) {
		xlocal.cell(0,_l) = 1;
	      }
	    } else {
	      dbg_printf("Local unit %d exists in pivot at %d, but not in remote - [DELETE]\n", _l, _lp);
	      accum.push_back(MatchUnit(_lp,_l,-1,true,MATCH_UNIT_DELETE));
	      if (_l>=0) {
		xlocal.cell(0,_l) = 1;
	      }
	    }
	  } else {
	    dbg_printf("Local unit %d not in pivot - [ADD]\n", _l);
	    accum.push_back(MatchUnit(-1,_l,-1,false,MATCH_UNIT_PRESERVE));
	    if (_l>=0) {
	      xlocal.cell(0,_l) = 1;
	    }
	  }
	}
      }
    } else {
      int _r = iremote;
      if (_r<xremote.height()) {
	if (xremote.cell(0,_r)==0) {
	  int _rp = order_remote.b2a(_r);
	  if (_rp!=-1) {
	    int _rpl = order_local.a2b(_rp);
	    if (_rpl!=-1) {
	      // we will get this (assuming no collisions), skip...
	    } else {
	      // deleted locally
	    }
	  } else {
	    dbg_printf("Remote unit %d not in pivot - [ADD]\n", _r);
	    accum.push_back(MatchUnit(-1,-1,_r,false,MATCH_UNIT_INSERT));
	    if (_r>=0) {
	      xremote.cell(0,_r) = 1;
	    }
	  }
	}
      }
    }
    if (ilocal<stop_local) {
      ilocal++;
      base_local++;
    } else {
      iremote++;
      base_remote++;
    }
  }
}


class RematchUnit {
public:
  bool changeLocal;
  bool changeRemote;

  RematchUnit() {
    changeLocal = changeRemote = false;
  }
};

/*
MatchUnit getIndex(map<int,MatchUnit>& ref,
		   const map<int,int> m, int v, int offset) {
  if (v==-1) return MatchUnit();
  v += offset;
  map<int,int>::const_iterator it = m.find(v);
  if (it!=m.end()) {
    return ref[it->second];
  }
  return MatchUnit();
}
*/

void OrderMerge::merge(const OrderResult& nlocal,
		       const OrderResult& nremote,
		       const CompareFlags& flags) {
  this->flags = flags;
  order_local = nlocal;
  order_remote = nremote;
  if (flags.head_trimmed) {
    order_local.trimHead(-1,-2);
    order_remote.trimHead(-1,-2);
  }
  if (flags.tail_trimmed) {
    order_local.trimTail(-1,-2);
    order_remote.trimTail(-1,-2);
  }
  xlocal.resize(1,order_local.blen(),0);
  xremote.resize(1,order_remote.blen(),0);
  start_local = 0;
  start_remote = 0;
  int base_local = 0;
  int base_remote = 0;
  process(0,0,base_local,base_remote,order_local.blen(),order_remote.blen());

  /*
    For unit that exists in pivot, local, and remote, with
    indices P, L, and R:
    - Try to determine if there has been a change of order from P->L or P->R
    - If so, try to implement that order.

    - Only deal with clear-cut cases for now.

   */

  // Prepare a reference order
  list<MatchUnit> canon = accum;
  /*
  map<int,MatchUnit> ref;
  map<int,int> p2canon;
  map<int,int> l2canon;
  map<int,int> r2canon;
  int at = 0;
  for (list<MatchUnit>::iterator it=accum.begin();
       it!=accum.end(); 
       it++) {
      MatchUnit& unit = *it;
      int pCol = unit.localUnit;
      int lCol = unit.pivotUnit;
      int rCol = unit.remoteUnit;
      bool deleted = unit.deleted;
      if (!deleted) {
	int idx = at;
	if (pCol!=-1) p2canon[pCol] = idx;
	if (lCol!=-1) l2canon[lCol] = idx;
	if (rCol!=-1) r2canon[rCol] = idx;
	ref[idx] = unit;
      }
      at++;
  }
  */

  // Prepare a list of constraints on order
  for (list<MatchUnit>::iterator it=accum.begin();
       it!=accum.end(); 
       it++) {
    MatchUnit& unit = *it;
    int pCol = unit.localUnit;
    int lCol = unit.pivotUnit;
    int rCol = unit.remoteUnit;
    bool deleted = unit.deleted;
    
    if (pCol!=-1&&lCol!=-1&&rCol!=-1&&!deleted) {
      /*
      int lColNext = getIndex(ref,p2canon,pCol,1).localUnit;
      int rColNext = getIndex(ref,p2canon,pCol,1).remoteUnit;
      int lColPrev = getIndex(ref,p2canon,pCol,-1).localUnit;
      int rColPrev = getIndex(ref,p2canon,pCol,-1).remoteUnit;
      */
      int lColNext = order_local.a2b(pCol+1);
      int rColNext = order_remote.a2b(pCol+1);
      int lColPrev = order_local.a2b(pCol-1);
      int rColPrev = order_remote.a2b(pCol-1);
      //lColNext = order_local.b2a(_lp);
      printf(">> %d:%d:%d %d:%d:%d\n",
	     lColPrev, lCol, lColNext,
	     rColPrev, rCol, rColNext);
      int ldiff = lColNext-lCol;
      int rdiff = rColNext-rCol;
      if (ldiff!=1&&rdiff!=1) { 
	ldiff = lCol-lColPrev; 
	rdiff = rCol-rColPrev; 
      }
      if (ldiff==1) {
	if (rdiff!=1) {
	  printf("Remote constraint P/L/R %d/%d/%d\n", pCol, lCol, rCol);
	}
      }
      if (rdiff==1) {
	if (ldiff!=1) {
	  printf("Local constraint P/L/R %d/%d/%d\n", pCol, lCol, rCol);
	}
      }
    }
  }
}

