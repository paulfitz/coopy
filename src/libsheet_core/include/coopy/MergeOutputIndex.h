#ifndef COOPY_MERGEOUTPUTINDEX
#define COOPY_MERGEOUTPUTINDEX

#include <coopy/MergeOutput.h>

namespace coopy {
  namespace cmp {
    class MergeOutputIndex;
  }
}

class coopy::cmp::MergeOutputIndex : public MergeOutput {
public:
  virtual bool mergeStart() {
    return true;
  }

  virtual bool declareLink(const LinkDeclare& decl);

  virtual bool wantDiff() { return false; }

  virtual bool wantLinks() { return true; }
};

#endif
