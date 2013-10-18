#ifndef COOPY_COMPARE_INC
#define COOPY_COMPARE_INC

#include <coopy/DataSheet.h>
#include <coopy/Patcher.h>
#include <coopy/CompareFlags.h>
#include <coopy/OrderResult.h>

namespace coopy {
  namespace cmp {
    class Compare;
  }
}

class coopy::cmp::Compare {
public:
 virtual int compare(coopy::store::DataSheet& pivot, 
		     coopy::store::DataSheet& local, 
		     coopy::store::DataSheet& remote, 
		     Patcher& output, const CompareFlags& flags) {
   return -1;
 }

 virtual int compare(coopy::store::DataSheet& pivot, 
		     coopy::store::DataSheet& local, 
		     coopy::store::DataSheet& remote, 
		     coopy::cmp::OrderResult& p2l_row_order, 
		     coopy::cmp::OrderResult& p2r_row_order, 
		     coopy::cmp::OrderResult& p2l_col_order, 
		     coopy::cmp::OrderResult& p2r_col_order, 
		     const CompareFlags& flags) {
   return -1;
 }
};

#endif
