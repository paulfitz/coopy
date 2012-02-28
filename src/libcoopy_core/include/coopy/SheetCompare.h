#ifndef CSVSHEET_COMPARE_INC
#define CSVSHEET_COMPARE_INC

#include <coopy/DataSheet.h>
#include <coopy/MergeOutput.h>
#include <coopy/CompareFlags.h>
#include <coopy/SheetView.h>
#include <coopy/OrderResult.h>

namespace coopy {
  namespace cmp {
    class SheetCompare;
  }
}

class coopy::cmp::SheetCompare {
public:
  int compare(coopy::store::DataSheet& pivot, 
	      coopy::store::DataSheet& local, 
	      coopy::store::DataSheet& remote, 
	      Patcher& output, const CompareFlags& flags,
	      const char *output_name = 0/*NULL*/);

  void setVerbose(bool verbose);

private:

  void doRowMapping(coopy::cmp::OrderResult& p2l_row_order,
		    coopy::cmp::OrderResult& p2r_row_order,
		    const coopy::cmp::OrderResult& p2l_col_order,
		    const coopy::cmp::OrderResult& p2r_col_order,
		    const coopy::cmp::CompareFlags& flags,
		    const coopy::cmp::CompareFlags& eflags,
		    coopy::store::SheetView& vpivot,
		    coopy::store::SheetView& vlocal,
		    coopy::store::SheetView& vremote);

  void doColMapping(const coopy::cmp::OrderResult& p2l_row_order,
		    const coopy::cmp::OrderResult& p2r_row_order,
		    coopy::cmp::OrderResult& p2l_col_order,
		    coopy::cmp::OrderResult& p2r_col_order,
		    const coopy::cmp::CompareFlags& flags,
		    const coopy::cmp::CompareFlags& eflags,
		    coopy::store::SheetView& vpivot,
		    coopy::store::SheetView& vlocal,
		    coopy::store::SheetView& vremote);
};

#endif
