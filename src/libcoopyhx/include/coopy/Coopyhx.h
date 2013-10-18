#ifndef COOPY_COOPYHX_INC
#define COOPY_COOPYHX_INC

#include <coopy/Compare.h>
#include <coopy/IntSheet.h>
#include <coopy/CoopyhxLink.h>

namespace coopy {
  namespace cmp {
    class CoopyhxCompare;
  }
}

class coopy::cmp::CoopyhxCompare : public Compare, public CoopyhxResult {
private:
  CoopyhxLink link;
  coopy::store::IntSheet p2l_row, p2l_col, p2r_row, p2r_col;
  coopy::store::IntSheet l2p_row, l2p_col, r2p_row, r2p_col;
public:
  void init() {
    link.init();
  }

  virtual int compare(coopy::store::DataSheet& pivot, 
		      coopy::store::DataSheet& local, 
		      coopy::store::DataSheet& remote, 
		      coopy::cmp::OrderResult& p2l_row_order, 
		      coopy::cmp::OrderResult& p2r_row_order, 
		      coopy::cmp::OrderResult& p2l_col_order, 
		      coopy::cmp::OrderResult& p2r_col_order, 
		      const CompareFlags& flags);

  virtual void order(int a, int b, bool row, bool p2l);
};


#endif
