#ifndef CSVSHEET_COMPARE_INC
#define CSVSHEET_COMPARE_INC

#include <coopy/DataSheet.h>
#include <coopy/MergeOutput.h>
#include <coopy/CompareFlags.h>

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
};

#endif
