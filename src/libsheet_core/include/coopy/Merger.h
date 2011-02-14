#ifndef COOPY_MERGER
#define COOPY_MERGER

#include <coopy/OrderMerge.h>
#include <coopy/OrderResult.h>
#include <coopy/DataSheet.h>
#include <coopy/MergeOutput.h>
#include <coopy/CompareFlags.h>
#include <coopy/NameSniffer.h>

#include <vector>

namespace coopy {
  namespace cmp {
    class Merger;
    class MergerState;
  }
}

class coopy::cmp::MergerState {
public:
  coopy::store::DataSheet& pivot;
  coopy::store::DataSheet& local;
  coopy::store::DataSheet& remote;
  const OrderResult& nrow_local;
  const OrderResult& nrow_remote;
  const OrderResult& ncol_local;
  const OrderResult& ncol_remote;
  Patcher& output;
  const CompareFlags& flags;
  coopy::store::NameSniffer& local_names;
  coopy::store::NameSniffer& remote_names;

  MergerState(coopy::store::DataSheet& pivot,
	      coopy::store::DataSheet& local,
	      coopy::store::DataSheet& remote,
	      const OrderResult& nrow_local,
	      const OrderResult& nrow_remote,
	      const OrderResult& ncol_local,
	      const OrderResult& ncol_remote,
	      Patcher& output,
	      const CompareFlags& flags,
	      coopy::store::NameSniffer& local_names,
	      coopy::store::NameSniffer& remote_names) :
    pivot(pivot), local(local), remote(remote),
    nrow_local(nrow_local), nrow_remote(nrow_remote),
    ncol_local(ncol_local), ncol_remote(ncol_remote),
    output(output),
    flags(flags),
    local_names(local_names), remote_names(remote_names) {
  }
};

class coopy::cmp::Merger {

public:
  Merger() {
  }

  bool merge(MergerState& state);

  bool mergeRow(coopy::store::DataSheet& pivot, 
		coopy::store::DataSheet& local, 
		coopy::store::DataSheet& remote,
		MatchUnit& row_unit, Patcher& output,
		const CompareFlags& flags,
		std::vector<coopy::cmp::RowChange>& rc);

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

