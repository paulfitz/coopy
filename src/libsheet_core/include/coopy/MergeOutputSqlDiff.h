#ifndef COOPY_MERGEOUTPUTSQLDIFF
#define COOPY_MERGEOUTPUTSQLDIFF

#include <coopy/MergeOutput.h>
#include <coopy/Dbg.h>

#include <string>

namespace coopy {
  namespace cmp {
    class SqlText;
    class MergeOutputSqlDiff;
  }
}

class coopy::cmp::SqlText {
public:
  std::string name;
  std::string vals;
  std::string conds;
  std::string val_columns;
  std::string val_values;
};

class coopy::cmp::MergeOutputSqlDiff : public MergeOutput {
private:
  std::string sheet_name;
public:
  MergeOutputSqlDiff() {
    sheet_name = coopy_get_default_table_name();
  }

  virtual bool wantDiff() { return true; }
  virtual bool changeColumn(const OrderChange& change);
  virtual bool changeRow(const RowChange& change);

  virtual bool setSheet(const char *name) { 
    sheet_name = name;
    return true;
  }

  static SqlText getText(const RowChange& change, const char *sheet_name,
			 const CompareFlags *flags = NULL);
};

#endif
