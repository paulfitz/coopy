#ifndef COOPY_MERGER
#define COOPY_MERGER

#include <coopy/OrderMerge.h>
#include <coopy/OrderResult.h>
#include <coopy/TextSheet.h>
#include <coopy/MergeOutput.h>

#include <vector>

class Merger {

public:
  Merger() {
  }

  void merge(TextSheet& pivot, TextSheet& local, TextSheet& remote,
	     const OrderResult& nrow_local,
	     const OrderResult& nrow_remote,
	     const OrderResult& ncol_local,
	     const OrderResult& ncol_remote,
	     MergeOutput& output);

  void mergeRow(TextSheet& pivot, TextSheet& local, TextSheet& remote,
		MatchUnit& row_unit, MergeOutput& output);

private:
  OrderMerge row_merge;
  OrderMerge col_merge;
  int conflicts;
  std::vector<std::string> lastAddress;
  std::vector<std::string> lastAction;
  int current_row;
  int last_row;
  int addition;
  //SheetSchema defaultSheetSchema;
  //SheetSchema *pivotSheetSchema;
  //SheetSchema *localSheetSchema;
  //SheetSchema *remoteSheetSchema;

};

#endif

