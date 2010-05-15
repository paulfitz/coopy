#ifndef CSVSHEET_COMPARE_INC
#define CSVSHEET_COMPARE_INC

#include <coopy/CsvSheet.h>



class CsvCompare {
private:
  CsvSheet cmp;
public:
  /*
    If a diff is made, the result in cmp contains a description of 
    how to get from the local sheet to a sheet incorporating the
    changes made in the remote sheet, if possible.

    If a diff is not made, the result in cmp is an actual merged
    sheet.
   */
  int compare(CsvSheet& pivot, CsvSheet& local, CsvSheet& remote, 
	      bool makeDiff = false);

  const CsvSheet& get() { return cmp; }

  void setVerbose(bool verbose);
};

#endif
