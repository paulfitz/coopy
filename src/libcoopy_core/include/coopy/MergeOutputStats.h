#ifndef COOPY_MERGEOUTPUTSTATS
#define COOPY_MERGEOUTPUTSTATS

#include <coopy/MergeOutput.h>

#include <string>
#include <vector>

namespace coopy {
  namespace cmp {
    class MergeOutputStats;
  }
}

class coopy::cmp::MergeOutputStats : public MergeOutput {
private:
  std::string sheet_name;
  std::vector<std::string> ids;
  coopy::store::PolySheet ops;
  bool active;
  int ct_col, ct_col_insert, ct_col_delete, ct_col_move, ct_col_rename;
  int ct_row, ct_row_insert, ct_row_delete, ct_row_move, ct_row_update;
public:
  MergeOutputStats() {
    active = false;
    flush();
  }

  virtual bool wantDiff() { return true; }

  virtual bool changeColumn(const OrderChange& change);
  virtual bool changeRow(const RowChange& change);

  virtual bool setSheet(const char *name) {
    sheet_name = name;
    active = true;
    return true;
  }

  virtual bool changeName(const NameChange& change) {
    active = true;
    return true;
  }

  bool flush();

  virtual bool mergeStart() {
    return flush();
  }

  virtual bool mergeAllDone() {
    return flush();
  }

  virtual bool needOutputBook() {
    return true;
  }
};

#endif
