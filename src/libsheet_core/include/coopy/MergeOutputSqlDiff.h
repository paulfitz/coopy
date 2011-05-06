#ifndef COOPY_MERGEOUTPUTSQLDIFF
#define COOPY_MERGEOUTPUTSQLDIFF

#include <coopy/MergeOutput.h>

#include <string>

namespace coopy {
  namespace cmp {
    class MergeOutputSqlDiff;
  }
}

class coopy::cmp::MergeOutputSqlDiff : public MergeOutput {
private:
  std::string sheet_name;
public:
  MergeOutputSqlDiff() {
    sheet_name = "sheet";
  }

  virtual bool wantDiff() { return true; }
  virtual bool changeColumn(const OrderChange& change);
  virtual bool changeRow(const RowChange& change);

  virtual bool setSheet(const char *name) { 
    sheet_name = name;
    return true;
  }
};

#endif
