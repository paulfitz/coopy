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

using namespace coopy::store;
using namespace coopy::cmp;

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

  FastMatch(MeasurePass& pass) : pass(pass) {
    local_names = remote_names = NULL;
  }

  void match(bool rowLike) {

    // We check if the two things being compared are identical.
    // If so, that's easy!

    // add matches for easy cases here
    if (pass.a.width()==pass.b.width() &&
	pass.a.height()==pass.b.height()) {
      bool fail = false;
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


int SheetCompare::compare(DataSheet& _pivot, DataSheet& _local, 
			  DataSheet& _remote,
			  MergeOutput& output, const CompareFlags& flags) {
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

  IdentityOrderResult id;

  /////////////////////////////////////////////////////////////////////////
  // PIVOT to LOCAL row mapping

  dbg_printf("SheetCompare::compare pivot <-> local rows\n");

  MeasurePass p2l_row_pass_local(pivot,local);
  MeasurePass p2l_row_pass_norm1(pivot,pivot);
  MeasurePass p2l_row_pass_norm2(local,local);

  CombinedRowMan p2l_row_local;
  CombinedRowMan p2l_row_norm1;
  CombinedRowMan p2l_row_norm2;

  MeasureMan p2l_row_man(p2l_row_local,p2l_row_pass_local,
			 p2l_row_norm1,p2l_row_pass_norm1,
			 p2l_row_norm2,p2l_row_pass_norm2,
			 1);

  p2l_row_man.setup();
  FastMatch p2l_row_fast_match(p2l_row_pass_local);
  p2l_row_fast_match.local_names = &local_names;
  p2l_row_fast_match.remote_names = &remote_names;
  p2l_row_fast_match.match(true);
  p2l_row_man.compare();


  /////////////////////////////////////////////////////////////////////////
  // PIVOT to REMOTE row mapping

  dbg_printf("SheetCompare::compare pivot <-> remote rows\n");

  MeasurePass p2r_row_pass_local(pivot,remote);
  MeasurePass p2r_row_pass_norm1(pivot,pivot);
  MeasurePass p2r_row_pass_norm2(remote,remote);

  CombinedRowMan p2r_row_local;
  CombinedRowMan p2r_row_norm1;
  CombinedRowMan p2r_row_norm2;

  MeasureMan p2r_row_man(p2r_row_local,p2r_row_pass_local,
			 p2r_row_norm1,p2r_row_pass_norm1,
			 p2r_row_norm2,p2r_row_pass_norm2,
			 1);

  p2r_row_man.setup();
  FastMatch p2r_row_fast_match(p2r_row_pass_local);
  p2r_row_fast_match.match(true);
  p2r_row_man.compare();


  /////////////////////////////////////////////////////////////////////////
  // PIVOT to LOCAL column mapping

  dbg_printf("SheetCompare::compare pivot <-> local columns\n");

  MeasurePass p2l_col_pass_local(pivot,local);
  MeasurePass p2l_col_pass_norm1(pivot,pivot);
  MeasurePass p2l_col_pass_norm2(local,local);

  OrderResult p2l_row_order = p2l_row_pass_local.getOrder();
  ColMan p2l_col_local(p2l_row_order);
  ColMan p2l_col_norm1(id);
  ColMan p2l_col_norm2(id);

  MeasureMan p2l_col_man(p2l_col_local,p2l_col_pass_local,
			 p2l_col_norm1,p2l_col_pass_norm1,
			 p2l_col_norm2,p2l_col_pass_norm2,
			 0);

  p2l_col_man.setup();
  FastMatch p2l_col_fast_match(p2l_col_pass_local);
  p2l_col_fast_match.match(false);
  p2l_col_man.compare();


  /////////////////////////////////////////////////////////////////////////
  // PIVOT to REMOTE column mapping

  dbg_printf("SheetCompare::compare pivot <-> remote columns\n");

  MeasurePass p2r_col_pass_local(pivot,remote);
  MeasurePass p2r_col_pass_norm1(pivot,pivot);
  MeasurePass p2r_col_pass_norm2(remote,remote);

  OrderResult p2r_row_order = p2r_row_pass_local.getOrder();
  ColMan p2r_col_local(p2r_row_order);
  ColMan p2r_col_norm1(id);
  ColMan p2r_col_norm2(id);

  MeasureMan p2r_col_man(p2r_col_local,p2r_col_pass_local,
			 p2r_col_norm1,p2r_col_pass_norm1,
			 p2r_col_norm2,p2r_col_pass_norm2,
			 0);

  p2r_col_man.setup();
  FastMatch p2r_col_fast_match(p2r_col_pass_local);
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
  merger.merge(state);

  dbg_printf("SheetCompare::compare done\n");

  return 0;
}


void SheetCompare::setVerbose(bool verbose) {
  _csv_verbose = verbose;
}
