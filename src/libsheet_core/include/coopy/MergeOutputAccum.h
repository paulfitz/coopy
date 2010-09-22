#ifndef COOPY_MERGEOUTPUTACCUM
#define COOPY_MERGEOUTPUTACCUM

#include <coopy/MergeOutput.h>
#include <coopy/CsvSheet.h>

namespace coopy {
  namespace cmp {
    class MergeOutputAccum;
  }
}

class coopy::cmp::MergeOutputAccum : public MergeOutput {
public:
  coopy::store::CsvSheet result;

  virtual bool wantDiff() { return false; }

  virtual bool addRow(const char *tag,
		      const std::vector<std::string>& row,
		      const std::string& blank);

  virtual bool stripMarkup();

  const coopy::store::CsvSheet& get() { return result; }
};

#endif
