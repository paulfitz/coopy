#ifndef COOPY_MERGER
#define COOPY_MERGER

#include <coopy/OrderMerge.h>
#include <coopy/OrderResult.h>

#include <vector>

class Merger {
public:
  OrderMerge row_merge;
  OrderMerge col_merge;
  int conflicts;
  std::vector<std::string> lastAddress;
  std::vector<std::string> lastAction;
  int current_row;
  int last_row;
  int addition;

  CsvSheet result;

  Merger() {
  }

  void merge(CsvSheet& pivot, CsvSheet& local, CsvSheet& remote,
	     const OrderResult& nrow_local,
	     const OrderResult& nrow_remote,
	     const OrderResult& ncol_local,
	     const OrderResult& ncol_remote);

  void diff(CsvSheet& pivot, CsvSheet& local, CsvSheet& remote,
	    const OrderResult& nrow_local,
	    const OrderResult& nrow_remote,
	    const OrderResult& ncol_local,
	    const OrderResult& ncol_remote);

  void mergeRow(CsvSheet& pivot, CsvSheet& local, CsvSheet& remote,
		MatchUnit& row_unit, bool diff);

  void addRow(const char *tag,
	      const std::vector<std::string>& row,
	      const std::string& blank);
};

#endif

