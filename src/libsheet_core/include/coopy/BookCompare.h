#ifndef COOPY_BOOKCOMPARE
#define COOPY_BOOKCOMPARE

#include <coopy/TextSheet.h>
#include <coopy/MergeOutput.h>
#include <coopy/TextBook.h>

class BookCompare {
public:
  int compare(TextBook& pivot, TextBook& local, TextBook& remote, 
	      MergeOutput& output);
  
  void setVerbose(bool verbose);
};

#endif
