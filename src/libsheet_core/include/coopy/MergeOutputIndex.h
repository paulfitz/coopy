#ifndef COOPY_MERGEOUTPUTINDEX
#define COOPY_MERGEOUTPUTINDEX

#include <coopy/MergeOutput.h>
#include <coopy/PolySheet.h>

namespace coopy {
  namespace cmp {
    class MergeOutputIndex;
  }
}

class coopy::cmp::MergeOutputIndex : public MergeOutput {
public:
  coopy::store::PolySheet links;

  virtual bool mergeStart();

  virtual bool mergeDone();

  virtual bool declareLink(const LinkDeclare& decl);

  virtual bool wantDiff() { return false; }

  virtual bool wantLinks() { return true; }
};

#endif
