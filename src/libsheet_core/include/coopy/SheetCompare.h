#ifndef CSVSHEET_COMPARE_INC
#define CSVSHEET_COMPARE_INC

#include <coopy/TextSheet.h>
#include <coopy/MergeOutput.h>
#include <coopy/CompareFlags.h>

class SheetCompare {
public:
  int compare(coopy::store::TextSheet& pivot, 
	      coopy::store::TextSheet& local, 
	      coopy::store::TextSheet& remote, 
	      MergeOutput& output, const CompareFlags& flags);

  void setVerbose(bool verbose);
};

#endif
