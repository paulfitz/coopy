#ifndef COOPY_MERGEOUTPUTPATCH
#define COOPY_MERGEOUTPUTPATCH

#include <coopy/MergeOutput.h>
#include <coopy/CsvSheet.h>

namespace coopy {
  namespace cmp {
    class MergeOutputPatch;
  }
}

class coopy::cmp::MergeOutputPatch : public MergeOutput {
public:
  coopy::store::CsvSheet result;

  virtual bool wantDiff() { return true; }

  virtual bool addRow(const char *tag,
		      const std::vector<coopy::store::SheetCell>& row,
		      const std::string& blank);

  const coopy::store::CsvSheet& get() { return result; }

};

#endif
