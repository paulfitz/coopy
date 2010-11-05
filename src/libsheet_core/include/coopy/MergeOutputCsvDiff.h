#ifndef COOPY_MERGEOUTPUTCSVDIFF
#define COOPY_MERGEOUTPUTCSVDIFF

#include <coopy/MergeOutput.h>
#include <coopy/CsvSheet.h>

namespace coopy {
  namespace cmp {
    class MergeOutputCsvDiff;
    class MergeOutputCsvDiffV0p2;
  }
}

class coopy::cmp::MergeOutputCsvDiff : public MergeOutput {
public:
  coopy::store::CsvSheet result;

  MergeOutputCsvDiff();

  virtual bool wantDiff() { return true; }

  virtual bool changeColumn(const OrderChange& change);
  virtual bool changeRow(const RowChange& change);

  bool selectRow(const RowChange& change, const char *tag);
  bool describeRow(const RowChange& change, const char *tag);

  virtual bool mergeDone();
  virtual bool declareNames(const std::vector<std::string>& names, bool final);

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
