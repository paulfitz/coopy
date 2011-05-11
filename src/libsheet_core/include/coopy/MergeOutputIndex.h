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
private:
  void *implementation;
public:
  MergeOutputIndex();

  virtual ~MergeOutputIndex();

  coopy::store::PolySheet links;
  coopy::store::PolySheet identity;

  virtual bool mergeStart();

  virtual bool mergeDone();

  virtual bool declareLink(const LinkDeclare& decl);

  virtual bool wantDiff() { return false; }

  virtual bool wantLinks() { return true; }

  virtual bool needOutputBook() {
    return true;
  }
};

#endif
