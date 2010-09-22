#ifndef COOPY_BOOKCOMPARE
#define COOPY_BOOKCOMPARE

#include <coopy/TextSheet.h>
#include <coopy/MergeOutput.h>
#include <coopy/TextBook.h>
#include <coopy/CompareFlags.h>

class BookCompare {
public:
  int compare(coopy::store::TextBook& pivot, 
	      coopy::store::TextBook& local, 
	      coopy::store::TextBook& remote, 
	      MergeOutput& output, const CompareFlags& flags);
  
  void setVerbose(bool verbose);
};

#endif
