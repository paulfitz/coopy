#ifndef COOPY_MERGEOUTPUTEDITLIST
#define COOPY_MERGEOUTPUTEDITLIST

#include <coopy/MergeOutput.h>
#include <coopy/CsvSheet.h>

#include <map>

namespace coopy {
  namespace cmp {
    class MergeOutputEditList;
  }
}

class coopy::cmp::MergeOutputEditList : public Patcher {
private:
  std::string sheet_name;
  std::map<std::string,int> col_map;
  int x;
  int y;

  bool outputField(const std::string& str) {
    return outputField(coopy::store::SheetCell(str,false));
  }

  bool makeSafe();
  bool outputField(const coopy::store::SheetCell& cell);
  bool outputRecord();
  
public:
  MergeOutputEditList() {
    sheet_name = coopy_get_default_table_name();
    x = y = -1;
  }

  virtual bool wantDiff() { return true; }

  virtual bool changeColumn(const OrderChange& change);
  virtual bool changeRow(const RowChange& change);

  virtual bool mergeStart() { return true; }
  virtual bool mergeDone() { return true; }

  virtual bool mergeAllDone() { return true; }

  virtual bool changeName(const NameChange& change);

  virtual bool outputStartsFromInput() {
    return true;
  }
  
  virtual bool needOutputBook() {
    return true;
  }

  virtual bool setSheet(const char *name);
};

#endif
