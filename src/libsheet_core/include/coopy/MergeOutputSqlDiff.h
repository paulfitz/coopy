#ifndef COOPY_MERGEOUTPUTSQLDIFF
#define COOPY_MERGEOUTPUTSQLDIFF

#include <coopy/MergeOutput.h>

class MergeOutputSqlDiff : public MergeOutput {
public:
  virtual bool wantDiff() { return true; }
  virtual bool changeColumn(const OrderChange& change);
  virtual bool changeRow(const RowChange& change);
};

#endif
