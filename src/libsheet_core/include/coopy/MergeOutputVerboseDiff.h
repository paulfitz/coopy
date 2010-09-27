#ifndef COOPY_MERGEOUTPUTVERBOSEDIFF
#define COOPY_MERGEOUTPUTVERBOSEDIFF

#include <coopy/MergeOutput.h>

#include <string>

namespace coopy {
  namespace cmp {
    class MergeOutputVerboseDiff;
  }
}

class coopy::cmp::MergeOutputVerboseDiff : public MergeOutput {
public:
  virtual bool wantDiff() { return true; }
  virtual bool changeColumn(const OrderChange& change);
  virtual bool changeRow(const RowChange& change);

  virtual bool setSheet(const char *name);

  virtual bool declareNames(const std::vector<std::string>& names, bool final);

};

#endif
