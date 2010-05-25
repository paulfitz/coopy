#ifndef COOPY_MERGEOUTPUTVERBOSEDIFF
#define COOPY_MERGEOUTPUTVERBOSEDIFF

#include <coopy/MergeOutput.h>

#include <string>

class MergeOutputVerboseDiff : public MergeOutput {
private:
  std::string sheet_name;
  std::string pending_message;

  void checkMessage();
public:
  virtual bool wantDiff() { return true; }
  virtual bool changeColumn(const OrderChange& change);
  virtual bool changeRow(const RowChange& change);

  virtual bool setSheet(const char *name) { 
    sheet_name = name;
    return true;
  }

  virtual bool declareNames(const std::vector<std::string>& names, bool final);

};

#endif
