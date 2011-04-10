#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <coopy/SheetCompare.h>
#include <coopy/CsvSheet.h>
#include <coopy/OrderResult.h>
#include <coopy/MeasurePass.h>
#include <coopy/RowMan.h>
#include <coopy/ColMan.h>
#include <coopy/MeasureMan.h>
#include <coopy/Merger.h>
#include <coopy/SchemaSniffer.h>

#include <string>
#include <map>

using namespace coopy::store;
using namespace coopy::cmp;

using namespace std;

namespace coopy {
  namespace cmp {
    class FastMatch;
    class RedundancyMatch;
  }
}

class coopy::cmp::FastMatch {
public:
  MeasurePass& pass;
  NameSniffer *local_names;
  NameSniffer *remote_names;
  std::string local_hash;
  std::string remote_hash;

  FastMatch(MeasurePass& pass) : pass(pass) {
    //local_names = remote_names = NULL;
    local_names = &pass.va.meta;
    remote_names = &pass.vb.meta;
    local_hash = pass.va.sha1;
    remote_hash = pass.vb.sha1;
  }

  void match(bool rowLike) {

    // We check if the two things being compared are identical.
    // If so, that's easy!

    // add matches for easy cases here
    if (pass.a.width()==pass.b.width() &&
	pass.a.height()==pass.b.height()) {
      bool fail = false;

      if (local_hash=="") {
	for (int r=0; r<pass.a.height() && !fail; r++) {
	  for (int c=0; c<pass.a.width(); c++) {
	    if (pass.a.cellSummary(c,r)!=pass.b.cellSummary(c,r)) {
	      dbg_printf("FastMatch::match mismatch at (%d,%d): [%s] vs [%s]\n",
			 c,r,
			 pass.a.cellSummary(c,r).toString().c_str(),
			 pass.b.cellSummary(c,r).toString().c_str());
	      fail = true;
	      break;
	    }
	  }
	}
      } else {
	fail = (local_hash!=remote_hash);
      }

      if (!fail) {
	// sheets are identical!
	dbg_printf("FastMatch::match identical sheets found\n");
	for (int i=0; i<pass.asel.height(); i++) {
	  pass.asel.cell(0,i) = i;
	  pass.bsel.cell(0,i) = i;
	}
	return;
      } else {
	dbg_printf("FastMatch::match sheets same size (%dx%d), but differ in content\n",
		   pass.a.width(),pass.a.height());
      }
    } else {
      dbg_printf("FastMatch::match sheets differ in size, %dx%d vs %dx%d\n",
		 pass.a.width(),pass.a.height(),
		 pass.b.width(),pass.b.height());
    }

    // Non identical eh?  Well, maybe we've been told to trust
    // some identifying columns.

    if (local_names!=NULL && remote_names!=NULL) {
      if (local_names->hasSubset()&&remote_names->hasSubset()) {
	// Great!  No need to do anything elaborate.  We've probably
	// already wasted too much time sucking data into memory,
	// oh well...
	
	// process subset here...
      }
    }
  }
};


static string encodeKey(DataSheet& sheet, int x, int y, int len) {
  string result = "";
  for (int i=x; i<x+len; i++) {
    if (i>x) {
    result += "__";
    }
    result += sheet.cellSummary(i,y).toString();
  }
  return result;
}

int SheetCompare::compare(DataSheet& _pivot, DataSheet& _local, 
			  DataSheet& _remote,
			  Patcher& output, const CompareFlags& flags) {
  DataSheet *ppivot = &_pivot;
  DataSheet *plocal = &_local;
  DataSheet *premote = &_remote;

  SchemaSniffer spivot(_pivot,NULL,true);
  SchemaSniffer slocal(_local,NULL,true);
  SchemaSniffer sremote(_remote,NULL,true);
  PolySheet dpivot, dlocal, dremote;
  bool appleOrange = false;
  if (_local.hasExternalColumnNames()!=_remote.hasExternalColumnNames() ||
      _local.hasExternalColumnNames()!=_pivot.hasExternalColumnNames()) {
    appleOrange = true;
    spivot.sniff();
    slocal.sniff();
    sremote.sniff();
    if (!_local.hasExternalColumnNames()) {
      dbg_printf("SheetCompare::compare wrap local sheet\n");
      dlocal = PolySheet(&_local,false);
      dlocal.setRowOffset(1);
      dlocal.setSchema(slocal.suggestSchema(),false);
      plocal = &dlocal;
    }
    if (!_remote.hasExternalColumnNames()) {
      dbg_printf("SheetCompare::compare wrap remote sheet\n");
      dremote = PolySheet(&_remote,false);
      dremote.setRowOffset(1);
      dremote.setSchema(sremote.suggestSchema(),false);
      premote = &dremote;
    }
    if (!_pivot.hasExternalColumnNames()) {
      dbg_printf("SheetCompare::compare wrap pivot sheet\n");
      dpivot = PolySheet(&_pivot,false);
      dpivot.setRowOffset(1);
      dpivot.setSchema(spivot.suggestSchema(),false);
      ppivot = &dpivot;
    }
  }

  DataSheet& pivot = *ppivot;
  DataSheet& local = *plocal;
  DataSheet& remote = *premote;

  NameSniffer pivot_names(pivot,false);
  NameSniffer local_names(local,false);
  NameSniffer remote_names(remote,false);

  CompareFlags eflags = flags;

  if (eflags.trust_ids) {
    local_names.sniff();
    remote_names.sniff();
    pivot_names.sniff();
  }

  if (eflags.trust_ids) {
    dbg_printf("Checking IDs\n");
    bool ok = local_names.subset(eflags.ids);
    ok = ok && remote_names.subset(eflags.ids);
    ok = ok && pivot_names.subset(eflags.ids);
    if (!ok) {
      fprintf(stderr,"Not all ID columns found\n");
      return -1;
    }
  }

  std::string local_hash = local.getHash(true);
  std::string remote_hash = remote.getHash(true);
  std::string pivot_hash = pivot.getHash(true);

  SheetView vpivot(pivot,pivot_names,pivot_hash);
  SheetView vlocal(local,local_names,local_hash);
  SheetView vremote(remote,remote_names,remote_hash);

  IdentityOrderResult id;

  /////////////////////////////////////////////////////////////////////////
  // PIVOT to LOCAL row mapping

  dbg_printf("SheetCompare::compare pivot <-> local rows\n");

  OrderResult p2l_row_order;
  OrderResult p2r_row_order;

  bool valueBasedPivot = (flags.mapping==NULL);

  if (valueBasedPivot) {
    MeasurePass p2l_row_pass_local(vpivot,vlocal);
    MeasurePass p2l_row_pass_norm1(vpivot,vpivot);
    MeasurePass p2l_row_pass_norm2(vlocal,vlocal);
    
    CombinedRowMan p2l_row_local;
    CombinedRowMan p2l_row_norm1;
    CombinedRowMan p2l_row_norm2;
    
    MeasureMan p2l_row_man(p2l_row_local,p2l_row_pass_local,
			   p2l_row_norm1,p2l_row_pass_norm1,
			   p2l_row_norm2,p2l_row_pass_norm2,
			   1);
    
    p2l_row_man.setup();
    FastMatch p2l_row_fast_match(p2l_row_pass_local);
    //p2l_row_fast_match.local_names = &pivot_names;
    //p2l_row_fast_match.remote_names = &local_names;
    //p2l_row_fast_match.local_hash = pivot_hash;
    //p2l_row_fast_match.remote_hash = local_hash;
    p2l_row_fast_match.match(true);
    p2l_row_man.compare();
    
    /////////////////////////////////////////////////////////////////////////
    // PIVOT to REMOTE row mapping
    
    dbg_printf("SheetCompare::compare pivot <-> remote rows\n");
    
    MeasurePass p2r_row_pass_local(vpivot,vremote);
    MeasurePass p2r_row_pass_norm1(vpivot,vpivot);
    MeasurePass p2r_row_pass_norm2(vremote,vremote);
    
    CombinedRowMan p2r_row_local;
    CombinedRowMan p2r_row_norm1;
    CombinedRowMan p2r_row_norm2;
    
    MeasureMan p2r_row_man(p2r_row_local,p2r_row_pass_local,
			   p2r_row_norm1,p2r_row_pass_norm1,
			   p2r_row_norm2,p2r_row_pass_norm2,
			   1);
    
    p2r_row_man.setup();
    FastMatch p2r_row_fast_match(p2r_row_pass_local);
    //p2r_row_fast_match.local_names = &pivot_names;
    //p2r_row_fast_match.remote_names = &remote_names;
    //p2r_row_fast_match.local_hash = pivot_hash;
    //p2r_row_fast_match.remote_hash = remote_hash;
    p2r_row_fast_match.match(true);
    p2r_row_man.compare();
    
    p2l_row_order = p2l_row_pass_local.getOrder();
    p2r_row_order = p2r_row_pass_local.getOrder();
  } else {

    // set up links using mapping

    bool local_pivot  = flags.pivot_sides_with_local;

    DataSheet& mapping = *(flags.mapping);
    int n = mapping.width()/2;
    map<string,int> local_index, remote_index, pivot_index;
    IntSheet l2p, r2p, p2l, p2r;
    l2p.resize(1,local.height(),-1);
    r2p.resize(1,remote.height(),-1);
    p2l.resize(1,pivot.height(),-1);
    p2r.resize(1,pivot.height(),-1);
    for (int i=0; i<local.height(); i++) {
      // assume 0 offsetting - not true in general, need to fix
      string k = encodeKey(local,0,i,n);
      local_index[k] = i;
    }
    for (int i=0; i<remote.height(); i++) {
      string k = encodeKey(remote,0,i,n);
      remote_index[k] = i;
    }
    for (int i=0; i<pivot.height(); i++) {
      string k = encodeKey(pivot,0,i,n);
      pivot_index[k] = i;
    }

    for (int i=0; i<mapping.height(); i++) {
      string klocal = encodeKey(mapping,0,i,n);
      string kremote = encodeKey(mapping,n,i,n);
      int l = -1;
      int r = -1;
      int p = -1;
      map<string,int>::iterator it = local_index.find(klocal);
      if (it!=local_index.end()) {
	l = it->second;
      }
      it = remote_index.find(kremote);
      if (it!=remote_index.end()) {
	r = it->second;
      }
      it = pivot_index.find(local_pivot?klocal:kremote);
      if (it!=pivot_index.end()) {
	p = it->second;
      }
      if (p!=-1) {
	if (l!=-1) {
	  l2p.cell(0,l) = p;
	  p2l.cell(0,p) = l;
	}
	if (r!=-1) {
	  r2p.cell(0,r) = p;
	  p2r.cell(0,p) = r;
	}
      }
    }
    p2l_row_order.setup(p2l,l2p);
    p2r_row_order.setup(p2r,r2p);
  }

  /////////////////////////////////////////////////////////////////////////
  // PIVOT to LOCAL column mapping

  dbg_printf("SheetCompare::compare pivot <-> local columns\n");

  MeasurePass p2l_col_pass_local(vpivot,vlocal);
  MeasurePass p2l_col_pass_norm1(vpivot,vpivot);
  MeasurePass p2l_col_pass_norm2(vlocal,vlocal);

  ColMan p2l_col_local(p2l_row_order);
  ColMan p2l_col_norm1(id);
  ColMan p2l_col_norm2(id);

  MeasureMan p2l_col_man(p2l_col_local,p2l_col_pass_local,
			 p2l_col_norm1,p2l_col_pass_norm1,
			 p2l_col_norm2,p2l_col_pass_norm2,
			 0);

  p2l_col_man.setup();
  FastMatch p2l_col_fast_match(p2l_col_pass_local);
  //p2l_col_fast_match.local_hash = pivot_hash;
  //p2l_col_fast_match.remote_hash = local_hash;
  p2l_col_fast_match.match(false);
  p2l_col_man.compare();


  /////////////////////////////////////////////////////////////////////////
  // PIVOT to REMOTE column mapping

  dbg_printf("SheetCompare::compare pivot <-> remote columns\n");

  MeasurePass p2r_col_pass_local(vpivot,vremote);
  MeasurePass p2r_col_pass_norm1(vpivot,vpivot);
  MeasurePass p2r_col_pass_norm2(vremote,vremote);

  ColMan p2r_col_local(p2r_row_order);
  ColMan p2r_col_norm1(id);
  ColMan p2r_col_norm2(id);

  MeasureMan p2r_col_man(p2r_col_local,p2r_col_pass_local,
			 p2r_col_norm1,p2r_col_pass_norm1,
			 p2r_col_norm2,p2r_col_pass_norm2,
			 0);

  p2r_col_man.setup();
  FastMatch p2r_col_fast_match(p2r_col_pass_local);
  //p2r_col_fast_match.local_hash = pivot_hash;
  //p2r_col_fast_match.remote_hash = remote_hash;
  p2r_col_fast_match.match(false);
  p2r_col_man.compare();


  /////////////////////////////////////////////////////////////////////////
  // Integrate results

  dbg_printf("SheetCompare::compare integrate\n");

  OrderResult p2l_col_order = p2l_col_pass_local.getOrder();
  OrderResult p2r_col_order = p2r_col_pass_local.getOrder();

  Merger merger;
  MergerState state(pivot,local,remote,
		    p2l_row_order,
		    p2r_row_order,
		    p2l_col_order,
		    p2r_col_order,
		    output,
		    eflags,
		    local_names,
		    remote_names);
  state.allIdentical = (pivot_hash == local_hash) && 
    (pivot_hash == remote_hash) &&
    (pivot_hash != "");
  bool ok = merger.merge(state);

  dbg_printf("SheetCompare::compare done\n");

  return ok?0:-1;
}


void SheetCompare::setVerbose(bool verbose) {
  _csv_verbose = verbose;
}
