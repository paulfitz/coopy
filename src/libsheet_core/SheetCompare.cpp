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

using namespace coopy::store;

class FastMatch {
public:
  MeasurePass& pass;

  FastMatch(MeasurePass& pass) : pass(pass) {
  }

  void match(bool rowLike) {
    // add matches for easy cases here
    if (pass.a.width()==pass.b.width() &&
	pass.a.height()==pass.b.height()) {
      bool fail = false;
      for (int r=0; r<pass.a.height() && !fail; r++) {
	for (int c=0; c<pass.a.width(); c++) {
	  if (pass.a.cell(c,r)!=pass.b.cell(c,r)) {
	    fail = true;
	    break;
	  }
	}
      }
      if (!fail) {
	// sheets are identical!
	dbg_printf("MATCH!\n");
	for (int i=0; i<pass.asel.height(); i++) {
	  pass.asel.cell(0,i) = i;
	  pass.bsel.cell(0,i) = i;
	}
      }
    }
  }
};

int SheetCompare::compare(TextSheet& pivot, TextSheet& local, TextSheet& remote,
			  MergeOutput& output, const CompareFlags& flags) {
  IdentityOrderResult id;

  /////////////////////////////////////////////////////////////////////////
  // PIVOT to LOCAL row mapping

  MeasurePass p2l_row_pass_local(pivot,local);
  MeasurePass p2l_row_pass_norm1(pivot,pivot);
  MeasurePass p2l_row_pass_norm2(local,local);

  RowMan p2l_row_local;
  RowMan p2l_row_norm1;
  RowMan p2l_row_norm2;

  MeasureMan p2l_row_man(p2l_row_local,p2l_row_pass_local,
			 p2l_row_norm1,p2l_row_pass_norm1,
			 p2l_row_norm2,p2l_row_pass_norm2,
			 1);

  p2l_row_man.setup();
  FastMatch p2l_row_fast_match(p2l_row_pass_local);
  p2l_row_fast_match.match(true);
  p2l_row_man.compare();


  /////////////////////////////////////////////////////////////////////////
  // PIVOT to REMOTE row mapping

  MeasurePass p2r_row_pass_local(pivot,remote);
  MeasurePass p2r_row_pass_norm1(pivot,pivot);
  MeasurePass p2r_row_pass_norm2(remote,remote);

  RowMan p2r_row_local;
  RowMan p2r_row_norm1;
  RowMan p2r_row_norm2;

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

  OrderResult p2l_col_order = p2l_col_pass_local.getOrder();
  OrderResult p2r_col_order = p2r_col_pass_local.getOrder();

  Merger merger;
  merger.merge(pivot,local,remote,
	       p2l_row_order,
	       p2r_row_order,
	       p2l_col_order,
	       p2r_col_order,
	       output,
	       flags);
  return 0;
}


void SheetCompare::setVerbose(bool verbose) {
  _csv_verbose = verbose;
}
