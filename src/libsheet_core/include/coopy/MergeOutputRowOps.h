#ifndef COOPY_MERGEOUTPUTROWOPS
#define COOPY_MERGEOUTPUTROWOPS

#include <coopy/MergeOutput.h>

#include <string>
#include <vector>

namespace coopy {
  namespace cmp {
    class MergeOutputRowOps;
  }
}

class coopy::cmp::MergeOutputRowOps : public MergeOutput {
private:
  std::string sheet_name;
  std::vector<std::string> ids;
  coopy::store::PolySheet ops;
public:
  virtual bool wantDiff() { return true; }
  virtual bool changeRow(const RowChange& change);

  virtual bool setSheet(const char *name) {
    sheet_name = name;
    ops.clear();
  }

  virtual bool needOutputBook() {
    return true;
  }
};

#endif
