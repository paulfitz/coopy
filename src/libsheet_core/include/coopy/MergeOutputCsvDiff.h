#ifndef COOPY_MERGEOUTPUTCSVDIFF
#define COOPY_MERGEOUTPUTCSVDIFF

#include <coopy/MergeOutput.h>
#include <coopy/CsvSheet.h>

#include <map>

namespace coopy {
  namespace cmp {
    class MergeOutputCsvDiff;
    class MergeOutputCsvDiffV0p2;
    typedef MergeOutputCsvDiffV0p2 MergeOutputCsvDiffStable;
    typedef MergeOutputCsvDiff MergeOutputCsvDiffUnstable;
  }
}

class coopy::cmp::MergeOutputCsvDiff : public MergeOutput {
public:
  coopy::store::CsvSheet result;
  std::vector<std::string> ops;
  std::map<std::string,bool> activeColumn;
  std::map<std::string,bool> showForSelect;
  std::map<std::string,bool> showForDescribe;

  MergeOutputCsvDiff();

  virtual bool wantDiff() { return true; }

  virtual bool changeColumn(const OrderChange& change);
  virtual bool changeRow(const RowChange& change);

  bool operateRow(const RowChange& change, const char *tag);
  bool selectRow(const RowChange& change, const char *tag);
  bool describeRow(const RowChange& change, const char *tag);

  virtual bool mergeDone();

  virtual bool changeName(const NameChange& change);

  const coopy::store::CsvSheet& get() { return result; }

};

class coopy::cmp::MergeOutputCsvDiffV0p2 : public MergeOutput {
public:
  coopy::store::CsvSheet result;

  MergeOutputCsvDiffV0p2();

  virtual bool wantDiff() { return true; }

  virtual bool changeColumn(const OrderChange& change);
  virtual bool changeRow(const RowChange& change);

  bool selectRow(const RowChange& change, const char *tag);
  bool describeRow(const RowChange& change, const char *tag);

  virtual bool mergeDone();

  virtual bool declareNames(const std::vector<std::string>& names, bool final);

  const coopy::store::CsvSheet& get() { return result; }

};

#endif
