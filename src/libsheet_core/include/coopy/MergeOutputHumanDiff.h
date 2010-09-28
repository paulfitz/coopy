#ifndef COOPY_MERGEOUTPUTHUMANDIFF
#define COOPY_MERGEOUTPUTHUMANDIFF

#include <coopy/MergeOutput.h>

#include <string>

namespace coopy {
  namespace cmp {
    class MergeOutputHumanDiff;
  }
}

class coopy::cmp::MergeOutputHumanDiff : public MergeOutput {
private:
  std::string sheet_name;
  std::string pending_message;
  bool showed_initial_columns;

  void checkMessage();
public:
  MergeOutputHumanDiff();

  virtual bool wantDiff() { return true; }
  virtual bool changeColumn(const OrderChange& change);
  virtual bool changeRow(const RowChange& change);

  virtual bool mergeDone();

  virtual bool setSheet(const char *name) { 
    sheet_name = name;
    return true;
  }

  virtual bool declareNames(const std::vector<std::string>& names, bool final);

};

#endif
