#ifndef COOPY_MERGEOUTPUTTDIFF
#define COOPY_MERGEOUTPUTTDIFF

#include <coopy/MergeOutput.h>
#include <coopy/Viterbi.h>

#include <map>

namespace coopy {
  namespace cmp {
    class MergeOutputTdiff;
  }
}

class coopy::cmp::MergeOutputTdiff : public MergeOutput {
public:
  std::vector<std::string> ops;
  std::vector<std::string> nops;
  std::map<std::string,bool> activeColumn;
  std::map<std::string,bool> showForSelect;
  std::map<std::string,bool> showForDescribe;
  std::map<std::string,bool> showForCond;
  std::map<std::string,bool> prevSelect;
  std::map<std::string,bool> prevDescribe;
  std::map<std::string,bool> prevCond;
  bool constantColumns;
  std::vector<std::string> columns;
  bool showedColumns;
  std::string sheetName;
  bool sheetNameShown;
  coopy::cmp::Viterbi formLattice;

  MergeOutputTdiff();

  bool check(std::map<std::string,bool>& m, const std::string& name) {
    if (m.find(name)==m.end()) return false;
    return m[name];
  }

  virtual bool wantDiff() { return true; }

  virtual bool changeColumn(const OrderChange& change);
  virtual bool changeRow(const RowChange& change);

  bool operateRow(const RowChange& change, const char *tag);
  bool updateRow(const RowChange& change, const char *tag, bool select, 
		 bool update, bool practice);
  bool describeRow(const RowChange& change, const char *tag);

  virtual bool mergeStart();
  virtual bool mergeDone();

  virtual bool changeName(const NameChange& change);

  virtual bool setSheet(const char *name);

  void showSheet();

};

#endif
