#ifndef CSVSHEET_COMPARE_INC
#define CSVSHEET_COMPARE_INC

#include <coopy/TextSheet.h>
#include <coopy/MergeOutput.h>

class SheetCompare {
public:
  int compare(TextSheet& pivot, TextSheet& local, TextSheet& remote, 
	      MergeOutput& output);

  void setVerbose(bool verbose);
};

#endif
