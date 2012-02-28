#ifndef COOPY_MERGEOUTPUT
#define COOPY_MERGEOUTPUT

#include <coopy/Patcher.h>
#include <coopy/CompareFlags.h>
#include <coopy/SheetCell.h>
#include <coopy/PolySheet.h>
#include <coopy/TextBook.h>

namespace coopy {
  namespace cmp {
    class MergeOutput;
  }
}

/**
 *
 * A generator of descriptions of a merge.  It contains a collection
 * of callbacks, called at various stages during a merge.  Callbacks
 * can be ignored if not relevant to the description being generated.
 *
 */
class coopy::cmp::MergeOutput : public Patcher {
public:
  virtual bool wantDiff() { return false; }
};

#endif

