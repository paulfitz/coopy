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

  coopy::store::PolySheet links;
  coopy::store::PolySheet links_column;
  coopy::store::SimpleSheetSchema links_column_schema;
  coopy::store::PolySheet identity;
  bool pending_row, sheet_set;
  coopy::store::PolySheet pivot, local, remote;
  std::vector<int> ipivot, ilocal, iremote;
  std::string name;

public:
  MergeOutputIndex();

  virtual ~MergeOutputIndex();

  virtual bool wantDiff() { return false; }

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
