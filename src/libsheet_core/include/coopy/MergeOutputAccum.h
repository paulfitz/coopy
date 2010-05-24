#ifndef COOPY_MERGEOUTPUTACCUM
#define COOPY_MERGEOUTPUTACCUM

#include <coopy/MergeOutput.h>
#include <coopy/CsvSheet.h>

class MergeOutputAccum : public MergeOutput {
public:
  CsvSheet result;

  virtual bool wantDiff() { return false; }

  virtual bool addRow(const char *tag,
		      const std::vector<std::string>& row,
		      const std::string& blank);

  virtual bool stripMarkup();

  const CsvSheet& get() { return result; }
};

#endif