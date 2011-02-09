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
    //dbg_printf("--- process %d %d / %d %d / %d %d\n", ilocal, iremote, 
    //	       base_local, base_remote, stop_local, stop_remote);
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



static bool summarize(int prev, int curr, int next) {
  if (prev!=-1) {
    if (prev==curr-1) {
      prev = 1;
    } else {
      prev = 0;
    }
  }
  if (next!=-1) {
    if (next==curr+1) {
      next = 1;
    } else {
      next = 0;
    }
  }
  return prev==1 || next==1;
}

static bool shunt(list<MatchUnit>& canon,
		  const MatchUnit& unit,
		  int prev,
		  int curr,
		  int next,
		  int side) {
  bool remote = (side==1);
  list<MatchUnit>::iterator subj = canon.end();
  list<MatchUnit>::iterator obj = canon.end();
  bool before = false; 
  int at_subj = -1;
  int at_obj = -1;
  int ct = 0;
  bool shunted = false;
  for (list<MatchUnit>::iterator it=canon.begin();
       it!=canon.end(); 
       it++) {
    MatchUnit& unit2 = *it;
    int r = remote?unit2.remoteUnit:unit2.localUnit;
    if (r!=-1) {
      if (r==prev||r==next) {
	dbg_printf("Shunt object found...\n");
	obj = it;
	at_obj = ct;
	if (r==next) {
	  before = true;
	} else {
	  before = false;
	}
      }
      if (r==curr) {
	dbg_printf("Shunt subject found...\n");
	subj = it;
	at_subj = ct;
      }
    }
    ct++;
  }
  if (obj!=canon.end() && subj!=canon.end()) {
    dbg_printf("Shunting... move %d %s %d\n", at_subj, before?"before":"after",
	       at_obj);
    if (before==false) {
      obj++;
      at_obj++;
    }
    if (at_obj==at_subj || at_obj==at_subj+1) {
      dbg_printf("  ... skipping shunt\n");
    } else {
      if (_csv_verbose) {
	dbg_printf("BEFORE ");
	for (list<MatchUnit>::iterator it=canon.begin();
	     it!=canon.end(); 
	     it++) {
	  dbg_printf("%d:%d:%d ", (*it).pivotUnit, (*it).localUnit, (*it).remoteUnit); 
	}
	dbg_printf("\n");
      }
      canon.splice(obj,canon,subj);
      shunted = true;
      if (_csv_verbose) {
	dbg_printf("AFTER  ");
	for (list<MatchUnit>::iterator it=canon.begin();
	     it!=canon.end(); 
	     it++) {
	  dbg_printf("%d:%d:%d ", (*it).pivotUnit, (*it).localUnit, (*it).remoteUnit); 
	}
	dbg_printf("\n");
      }
    }
  } else {
    dbg_printf("Shunt failed!\n");
  }
  return shunted;
}

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

  list<MatchUnit> canon = accum;
  int shunt_count = 0;

  // Prepare a list of constraints on order
  for (list<MatchUnit>::iterator it=accum.begin();
       it!=accum.end(); 
       it++) {
    MatchUnit& unit = *it;
    int pCol = unit.pivotUnit;
    int lCol = unit.localUnit;
    int rCol = unit.remoteUnit;
    bool deleted = unit.deleted;

    if (!deleted) {
      if (pCol!=-1&&lCol!=-1&&rCol!=-1) {
	//printf("Working on P/L/R %d %d %d\n", pCol, lCol, rCol);
	int lColNext = order_local.a2b(pCol+1);
	int rColNext = order_remote.a2b(pCol+1);
	int lColPrev = order_local.a2b(pCol-1);
	int rColPrev = order_remote.a2b(pCol-1);
	//lColNext = order_local.b2a(_lp);
	/*
	  printf(">> %d:%d:%d %d:%d:%d\n",
	  lColPrev, lCol, lColNext,
	  rColPrev, rCol, rColNext);
	*/
	
	bool lCont = summarize(lColPrev,lCol,lColNext);
	bool rCont = summarize(rColPrev,rCol,rColNext);
	
	if (lCont&&!rCont) {
	  dbg_printf("Remote constraint P/L/R %d/%d/%d\n", pCol, lCol, rCol);
	  if (shunt(canon,unit,rCol-1,rCol,rCol+1,1)) {
	    shunt_count++;
	  }
	}
	if (rCont&&!lCont) {
	  dbg_printf("Local constraint P/L/R %d/%d/%d\n", pCol, lCol, rCol);
	  if (shunt(canon,unit,lCol-1,lCol,lCol+1,-1)) {
	    shunt_count++;
	  }
	}
      } else if (pCol==-1) {
	if (rCol!=-1 && lCol==-1) {
	  int rColNext = order_remote.a2b(pCol+1);
	  int rColPrev = order_remote.a2b(pCol-1);
	  dbg_printf("Remote new col constraint P/L/R %d/%d/%d\n", pCol, lCol, rCol);
	  if (shunt(canon,unit,rCol-1,rCol,rCol+1,1)) {
	    shunt_count++;
	  }
	}
      }
    }
  }
  if (shunt_count>0) {
    dbg_printf("Copying result of shunt...\n");
    accum = canon;
  }
}

