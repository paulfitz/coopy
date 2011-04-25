#include <coopy/OrderMerge.h>
#include <coopy/Dbg.h>
#include <coopy/Viterbi.h>
#include <coopy/EfficientMap.h>

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
	      //dbg_printf("Local unit %d exists in pivot at %d and in remote at %d\n", _l, _lp, _lpr);
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

/*
static bool shunt(list<MatchUnit>& canon,
		  list<MatchUnit>& accum,
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
  dbg_printf("Asked to shunt:\n");
  if (_csv_verbose) {
    dbg_printf("  BEFORE ");
    for (list<MatchUnit>::iterator it=canon.begin();
	 it!=canon.end(); 
	 it++) {
      dbg_printf("%d:%d:%d ", (*it).pivotUnit, (*it).localUnit, (*it).remoteUnit); 
    }
    dbg_printf("\n");
    dbg_printf("  for P/L/R %d:%d:%d\n", unit.pivotUnit, unit.localUnit, unit.remoteUnit); 
    dbg_printf("  side %s prev %d curr %d next %d\n", remote?"remote":"local",
	       prev, curr, next);
  }
  bool unneeded = false;
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
	  if (at_subj!=-1) {
	    if (at_obj==at_subj+1) unneeded = true;
	  }
	} else {
	  before = false;
	}
      }
      if (r==curr) {
	dbg_printf("Shunt subject found...\n");
	subj = it;
	at_subj = ct;
	if (at_obj!=-1) {
	  if (at_subj==at_obj+1) unneeded = true;
	}
      }
    }
    ct++;
  }
  if (obj!=canon.end() && subj!=canon.end()) {
    dbg_printf("Shunting... move %d %s %d\n", at_subj, before?"before":"after",
	       at_obj);
    //bool unneeded = before?(at_subj<at_obj):(at_subj>at_obj);
    if (before==false) {
      obj++;
      at_obj++;
    }
    if (_csv_verbose) {
      dbg_printf("BEFORE ");
      for (list<MatchUnit>::iterator it=canon.begin();
	   it!=canon.end(); 
	   it++) {
	dbg_printf("%d:%d:%d ", (*it).pivotUnit, (*it).localUnit, (*it).remoteUnit); 
      }
      dbg_printf("\n");
    }
    if ((at_obj==at_subj || at_obj==at_subj+1)||unneeded) {
      dbg_printf("  ... skipping shunt\n");
    } else {
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
*/

/*
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
  bool shunted = false;

  // Prepare a list of constraints on order
  do {
    shunted = false;
    if (coopy_is_verbose()) {
      for (list<MatchUnit>::iterator it=canon.begin();
	   it!=canon.end(); 
	   it++) {
	MatchUnit& unit = *it;
	int pCol = unit.pivotUnit;
	int lCol = unit.localUnit;
	int rCol = unit.remoteUnit;
	bool deleted = unit.deleted;
	dbg_printf("match P/L/R %d %d %d %s\n", pCol, lCol, rCol,
		   deleted?"(deleted)":"");
      }
    }
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
	  //dbg_printf("Working on P/L/R %d %d %d\n", pCol, lCol, rCol);
	  int lColNext = order_local.a2b(pCol+1);
	  int rColNext = order_remote.a2b(pCol+1);
	  int lColPrev = order_local.a2b(pCol-1);
	  int rColPrev = order_remote.a2b(pCol-1);
	  //lColNext = order_local.b2a(_lp);
	  
	  bool lCont = summarize(lColPrev,lCol,lColNext);
	  bool rCont = summarize(rColPrev,rCol,rColNext);
	  
	  if (lCont&&!rCont) {
	    dbg_printf("Remote constraint P/L/R %d/%d/%d\n", pCol, lCol, rCol);
	    if (shunt(canon,accum,unit,rCol-1,rCol,rCol+1,1)) {
	      shunt_count++;
	      shunted = true;
	    }
	  }
	  if (rCont&&!lCont) {
	    dbg_printf("Local constraint P/L/R %d/%d/%d\n", pCol, lCol, rCol);
	    if (shunt(canon,accum,unit,lCol-1,lCol,lCol+1,-1)) {
	      shunt_count++;
	      shunted = true;
	    }
	  }
	} else if (pCol==-1) {
	  if (rCol!=-1 && lCol==-1) {
	    int rColNext = order_remote.a2b(pCol+1);
	    int rColPrev = order_remote.a2b(pCol-1);
	    dbg_printf("Remote new col constraint P/L/R %d/%d/%d\n", pCol, lCol, rCol);
	    if (shunt(canon,accum,unit,rCol-1,rCol,rCol+1,1)) {
	      shunt_count++;
	      shunted = true;
	    }
	  }
	}
      }
    }
  } while (shunted);
  if (coopy_is_verbose()) {
    for (list<MatchUnit>::iterator it=canon.begin();
	 it!=canon.end(); 
	 it++) {
      MatchUnit& unit = *it;
      int pCol = unit.pivotUnit;
      int lCol = unit.localUnit;
      int rCol = unit.remoteUnit;
      bool deleted = unit.deleted;
      dbg_printf("final match P/L/R %d %d %d %s\n", pCol, lCol, rCol,
		 deleted?"(deleted)":"");
    }
  }
  if (shunt_count>0) {
    dbg_printf("Copying result of shunt...\n");
    accum = canon;
  }
}
*/


static void evaluate(const OrderResult& order_local,
		     const OrderResult& order_remote,
		     int lCol,
		     int rCol,
		     int lastWasL,
		     float& lCost,
		     float& rCost) {
  int plCol = order_local.b2a(lCol);
  int prCol = order_remote.b2a(rCol);
  int lprCol = -1;
  int rplCol = -1;

  if (prCol>=0) {
    lprCol = order_local.a2b(prCol);
    if (lprCol>=0 && lprCol<lCol) {
      // already consumed, draw for free.
      rCost = 0;
      return;
    }
  }

  if (plCol>=0) {
    rplCol = order_remote.a2b(plCol);
    if (rplCol>=0 && rplCol<rCol) {
      // already consumed, draw for free.
      lCost = 0;
      return;
    }
  }

  int lColNext = order_local.a2b(plCol+1);
  int lColPrev = order_local.a2b(plCol-1);
  int rColNext = order_remote.a2b(prCol+1);
  int rColPrev = order_remote.a2b(prCol-1);
  bool lCont = summarize(lColPrev,lCol,lColNext);
  bool rCont = summarize(rColPrev,rCol,rColNext);
}


int safe_next(const efficient_map<int,int>& m, int val, int def) {
  efficient_map<int,int>::const_iterator it = m.find(val);
  if (it==m.end()) return def;
  return it->second;
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

  bool good = false;
  bool more = true;
  list<MatchUnit> canon;
  list<MatchUnit> canon_rem1;
  list<MatchUnit> canon_rem2;
  list<MatchUnit> *canon_src = &accum;

  while (more) {

    int ct = 0;
    efficient_map<int,int> l2k, r2k, p2k;
    efficient_map<int,MatchUnit *> mu;
    
    int lowestL = -2;
    int lowestR = -2;
    for (list<MatchUnit>::iterator it=canon_src->begin();
	 it!=canon_src->end(); 
	 it++) {
      MatchUnit& unit = *it;
      int pCol = unit.pivotUnit;
      int lCol = unit.localUnit;
      int rCol = unit.remoteUnit;
      bool deleted = unit.deleted;
      if ((lCol<lowestL&&lCol>=0)||lowestL==-2) {
	lowestL = lCol;
      }
      if ((rCol<lowestR&&rCol>=0)||lowestR==-2) {
	lowestR = rCol;
      }
      dbg_printf("match %d: P/L/R %d %d %d %s\n", ct, pCol, lCol, rCol,
		 deleted?"(deleted)":"");
      if (pCol>=0) {
	p2k[pCol] = ct;
      }
      if (lCol>=0) {
	l2k[lCol] = ct;
      }
      if (rCol>=0) {
	r2k[rCol] = ct;
      }
      mu[ct] = &unit;
      ct++;
    }
    int units = ct;
    
    Viterbi v;
    
    int dud = units;
    v.setSize(units+1,units+2);
    set<int> idx;
    
    v.beginTransitions();
    if (lowestL>=0) {
      v.addTransition(l2k[lowestL],l2k[lowestL],0);  
      idx.insert(l2k[lowestL]);
    }
    if (lowestR>=0) {
      v.addTransition(r2k[lowestR],r2k[lowestR],0);  
      idx.insert(r2k[lowestR]);
    }
    v.addTransition(dud,dud,1);  
    v.endTransitions();
    
    for (int draw=0; draw<units-1; draw++) {
      dbg_printf("DRAW %d\n", draw);
      v.beginTransitions();

      set<int> idx2;
      for (set<int>::const_iterator it1=idx.begin(); it1!=idx.end(); it1++) {
	int k = (*it1);
	dbg_printf("  draw %d k %d\n", draw, k);
	if (k==dud) continue;

	MatchUnit& unit = *mu[k];
	int pCol = unit.pivotUnit;
	int lCol = unit.localUnit;
	int rCol = unit.remoteUnit;
	bool deleted = unit.deleted;

	dbg_printf("    match P/L/R %d %d %d %s\n", pCol, lCol, rCol,
		   deleted?"(deleted)":"");

	if (lCol==-1 && rCol==-1) {
	  int next = safe_next(p2k,pCol+1,dud);
	  v.addTransition(k,next,0);
	  dbg_printf("    transition %d %d\n", k, next);
	  idx2.insert(next);
	  continue;
	}
	if (rCol==-1) {
	  int next = safe_next(l2k,lCol+1,dud);
	  v.addTransition(k,next,0);
	  dbg_printf("    transition %d %d\n", k, next);
	  idx2.insert(next);
	  continue;
	}
	if (lCol==-1) {
	  int next = safe_next(r2k,rCol+1,dud);
	  v.addTransition(k,next,0);
	  dbg_printf("    transition %d %d\n", k, next);
	  idx2.insert(next);
	  continue;
	}
	int lnext = safe_next(l2k,lCol+1,dud);
	int rnext = safe_next(r2k,rCol+1,dud);
	/*
	  if (lnext==dud||rnext==dud) {
	  v.addTransition(k,lnext,0);
	  dbg_printf("    transition %d %d\n", k, lnext);
	  idx2.insert(lnext);
	  v.addTransition(k,rnext,0);
	  dbg_printf("    transition %d %d\n", k, rnext);
	  idx2.insert(rnext);
	  continue;
	  }
	*/
	int pnext = safe_next(p2k,pCol+1,dud);
	if (lnext!=pnext) {
	  v.addTransition(k,lnext,0);
	  dbg_printf("    transition %d %d\n", k, lnext);
	  idx2.insert(lnext);
	}
	if (rnext!=pnext) {
	  v.addTransition(k,rnext,0);
	  dbg_printf("    transition %d %d\n", k, rnext);
	  idx2.insert(rnext);
	}
	if (rnext==pnext&&lnext==pnext) {
	  v.addTransition(k,pnext,0);
	  dbg_printf("    transition %d %d\n", k, pnext);
	  idx2.insert(pnext);
	}
      }
      idx = idx2;
      v.addTransition(dud,dud,1);  
      v.endTransitions();
    }

    v.beginTransitions();
    for (int draw=0; draw<=units; draw++) {
      v.addTransition(draw,draw,(draw==units));
    }
    v.endTransitions();
    int qo = 1;

    if (_csv_verbose) {
      dbg_printf("Viterbi calculation:\n");
      int q = v.length()-qo;
      for (int i=0; i<q; i++) {
	int k = v.getPath(i);
	dbg_printf("*** %d\n", k);
      }
    }

    int q = v.length()-qo;
    efficient_map<int,int> dups;
    dups[dud] = 1;
    good = true;
    //if (v.getPath(q)!=dud) {
    for (int i=0; i<q; i++) {
      int k = v.getPath(i);
      int k2 = k;
      if (i<q-1) {
	k2 = v.getPath(i+1);
      }
      if (dups.find(k)==dups.end() && dups.find(k2)==dups.end()) {
	canon.push_back(*mu[k]);
	dups[k] = 1;
      } else {
	good = false;
	break;
      }
    }
    //} else {
    //      good = false;
    //}

    more = false;
    if (!good) {
      more = true;
      dbg_printf("Going back for more after getting to length %d of %d...\n",
		 canon.size(), accum.size());
      list<MatchUnit> *canon_old_src = canon_src;
      if (canon_src != &canon_rem1) {
	canon_src = &canon_rem1;
      } else {
	canon_src = &canon_rem2;
      }
      canon_src->clear();
      int k = 0;
      int omit = 0;
      for (list<MatchUnit>::iterator it=canon_old_src->begin();
	   it!=canon_old_src->end(); 
	   it++) {
	if (dups.find(k)!=dups.end()) {
	  omit++;
	} else if (it->deleted) {
	  canon.push_back(*it);
	  omit++;
	} else {
	  canon_src->push_back(*it);
	}
	k++;
      }
      if (omit==0) {
	dbg_printf("Actually, no point going back for more, no progress made\n");
	more = false;
      }
    }
    dbg_printf("STATUS %d %d / %d %d\n", canon.size(), accum.size(), good, more);
  }

  if (good) {
    accum = canon;
    for (list<MatchUnit>::iterator it=accum.begin();
	 it!=accum.end(); 
	 it++) {
      MatchUnit& unit = *it;
      int pCol = unit.pivotUnit;
      int lCol = unit.localUnit;
      int rCol = unit.remoteUnit;
      bool deleted = unit.deleted;
      dbg_printf("final P/L/R %d %d %d %s\n", pCol, lCol, rCol,
		 deleted?"(deleted)":"");
    }
  } else {
    fprintf(stderr, "No consistent order merge was possible.\n");
    dbg_printf("No consistent order merge was possible.\n");
  }
}


