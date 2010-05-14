#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <list>

using namespace std;


////////////////////////////////////////////////////////////////////////
//
// Row/Column neutral code
//

#include <coopy/CsvCompare.h>
#include <coopy/CsvWrite.h>
#include <coopy/OrderResult.h>

#include <coopy/FVal.h>
#include <coopy/FMap.h>
#include <coopy/MeasurePass.h>
#include <coopy/Measure.h>
#include <coopy/MeasureMan.h>
#include <coopy/ColMan.h>
#include <coopy/RowMan.h>
#include <coopy/MatchUnit.h>
#include <coopy/OrderMerge.h>
#include <coopy/Merger.h>


void CsvCompare::compare(CsvSheet& local, CsvSheet& remote) {
  printf("Two-way compare no longer implemented\n");
  exit(1);
}

int CsvCompare3::compare(CsvSheet& pivot, CsvSheet& local, CsvSheet& remote,
			 bool makeDiff) {
  IdentityOrderResult id;

  //RowOrder order;
  //order.compare(pivot,local,remote);

  // Prepare for p2l row comparison
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

  // Compare p2l rows
  p2l_row_man.compare();


  // Prepare for p2r row comparison
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

  p2r_row_man.compare();

  // Now, column comparison

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

  p2l_col_man.compare();

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

  p2r_col_man.compare();


  OrderResult p2l_col_order = p2l_col_pass_local.getOrder();
  OrderResult p2r_col_order = p2r_col_pass_local.getOrder();

  Merger merger;
  if (makeDiff) {
    merger.diff(pivot,local,remote,
		p2l_row_order,
		p2r_row_order,
		p2l_col_order,
		p2r_col_order);
  } else {
    merger.merge(pivot,local,remote,
		 p2l_row_order,
		 p2r_row_order,
		 p2l_col_order,
		 p2r_col_order);
  }

  cmp = merger.result;

  return 0;
}


void CsvCompare3::setVerbose(bool verbose) {
  _csv_verbose = verbose;
}
