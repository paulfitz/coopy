#ifndef COOPY_ORDERMERGE
#define COOPY_ORDERMERGE

#include <coopy/OrderResult.h>
#include <coopy/CsvSheet.h>
#include <coopy/OrderResult.h>
#include <coopy/MatchUnit.h>

#include <list>

class OrderMerge {
public:
  OrderResult order_local, order_remote;
  std::list<MatchUnit> accum;
  IntSheet xlocal, xremote;
  int start_local;
  int start_remote;

  void process(int ilocal, int iremote,
	       int stop_local, int stop_remote);

  void merge(const OrderResult& nlocal,
	     const OrderResult& nremote) {
    order_local = nlocal;
    order_remote = nremote;
    xlocal.resize(1,order_local.blen(),0);
    xremote.resize(1,order_remote.blen(),0);
    start_local = 0;
    start_remote = 0;
    process(0,0,order_local.blen(),order_remote.blen());
  }
};


#endif
