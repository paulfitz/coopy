#ifndef COOPY_MERGEOUTPUT
#define COOPY_MERGEOUTPUT

#include <string>
#include <vector>


class MergeOutput {
public:
  virtual bool wantDiff() = 0;

  virtual bool addRow(const char *tag,
		      const std::vector<std::string>& row,
		      const std::string& blank) { return false; }

  virtual bool stripMarkup() { return false; }
};

#endif
