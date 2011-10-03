#ifndef COOPY_ORDERMERGE
#define COOPY_ORDERMERGE

#include <coopy/OrderResult.h>
#include <coopy/CsvSheet.h>
#include <coopy/OrderResult.h>
#include <coopy/MatchUnit.h>
#include <coopy/CompareFlags.h>

#include <list>

namespace coopy {
  namespace cmp {
    class OrderMerge;
  }
}

class coopy::cmp::OrderMerge {
public:
  OrderResult order_local, order_remote;
  std::list<MatchUnit> accum;
  int overlap;
  coopy::store::IntSheet xlocal, xremote;
  int start_local;
  int start_remote;
  CompareFlags flags;

  void process(int ilocal, int iremote,
	       int& base_local, int& base_remote,
	       int stop_local, int stop_remote);

  void merge(const OrderResult& nlocal,
	     const OrderResult& nremote,
	     const CompareFlags& flags); 

  void merge_by_id(const OrderResult& nlocal,
		   const OrderResult& nremote,
		   const CompareFlags& flags); 
};


#endif
