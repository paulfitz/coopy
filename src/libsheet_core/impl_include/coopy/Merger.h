#ifndef COOPY_MERGER
#define COOPY_MERGER

#include <coopy/OrderMerge.h>
#include <coopy/OrderResult.h>
#include <coopy/TextSheet.h>
#include <coopy/MergeOutput.h>
#include <coopy/CompareFlags.h>

#include <vector>

namespace coopy {
  namespace cmp {
    class Merger;
  }
}

class coopy::cmp::Merger {

public:
  Merger() {
  }

  void merge(coopy::store::TextSheet& pivot, 
	     coopy::store::TextSheet& local, 
	     coopy::store::TextSheet& remote,
	     const OrderResult& nrow_local,
	     const OrderResult& nrow_remote,
	     const OrderResult& ncol_local,
	     const OrderResult& ncol_remote,
	     MergeOutput& output,
	     const CompareFlags& flags);

  void mergeRow(coopy::store::TextSheet& pivot, 
		coopy::store::TextSheet& local, 
		coopy::store::TextSheet& remote,
		MatchUnit& row_unit, MergeOutput& output,
		const CompareFlags& flags);

private:
  OrderMerge row_merge;
  OrderMerge col_merge;
  int conflicts;
  std::vector<std::string> lastAddress;
  std::vector<std::string> lastAction;
  std::vector<std::string> names;
  int current_row;
  int last_row;
  int addition;
  //SheetSchema defaultSheetSchema;
  //SheetSchema *pivotSheetSchema;
  //SheetSchema *localSheetSchema;
  //SheetSchema *remoteSheetSchema;

};

#endif

