#ifndef COOPY_MERGEOUTPUTINDEX
#define COOPY_MERGEOUTPUTINDEX

#include <coopy/MergeOutput.h>
#include <coopy/PolySheet.h>
#include <coopy/SheetSchema.h>

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
  coopy::store::PolySheet links_column;
  coopy::store::SimpleSheetSchema links_column_schema;
  coopy::store::PolySheet identity;

  virtual bool wantDiff() { return true; }

  virtual bool mergeStart();

  virtual bool mergeDone();

  virtual bool setSheet(const char *name);

  virtual bool declareLink(const LinkDeclare& decl);

  virtual bool wantLinks() { return true; }

  virtual bool needOutputBook() {
    return true;
  }
};

#endif
