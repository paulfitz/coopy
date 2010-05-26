#include <coopy/OrderMerge.h>
#include <coopy/Dbg.h>


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
}

