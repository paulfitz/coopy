#ifndef COOPY_COMPARE_INC
#define COOPY_COMPARE_INC

#include <coopy/DataSheet.h>
#include <coopy/Patcher.h>
#include <coopy/CompareFlags.h>

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
		     Patcher& output, const CompareFlags& flags) = 0;
};

#endif
