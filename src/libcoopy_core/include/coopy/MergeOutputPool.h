#ifndef COOPY_MERGEOUTPUTPOOL
#define COOPY_MERGEOUTPUTPOOL

#include <coopy/MergeOutput.h>

#include <string>
#include <vector>

namespace coopy {
  namespace cmp {
    class MergeOutputPool;
  }
}

class coopy::cmp::MergeOutputPool : public MergeOutput {
private:
  std::string sheet_name;
public:
  virtual bool wantDiff() { return true; }

  virtual bool setSheet(const char *name) {
    sheet_name = name;
    return true;
  }

  virtual bool needOutputBook() {
    return true;
  }

  virtual bool changePool(const PoolChange& change) {
    return applyPool(change);
  }
};

#endif
