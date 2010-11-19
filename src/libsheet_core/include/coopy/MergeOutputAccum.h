#ifndef COOPY_MERGEOUTPUTACCUM
#define COOPY_MERGEOUTPUTACCUM

#include <coopy/MergeOutput.h>
#include <coopy/CsvSheet.h>

namespace coopy {
  namespace cmp {
    class MergeOutputAccum;
  }
}

class coopy::cmp::MergeOutputAccum : public MergeOutput {
private:
  coopy::store::CsvSheet result;
  coopy::store::PolySheet fallback;
  bool sync;
  bool useFallback;
public:
 MergeOutputAccum() : fallback(&result,false) {
    sync = false;
    MergeOutput::attachSheet(fallback);
    useFallback = true;
  }

  virtual void attachSheet(coopy::store::PolySheet sheet) {
    MergeOutput::attachSheet(sheet);
    useFallback = false;
  }

  virtual bool mergeStart() {
    sync = false;
    return true;
  }

  virtual bool wantDiff() { return false; }

  virtual bool addRow(const char *tag,
		      const std::vector<coopy::store::SheetCell>& row,
		      const std::string& blank);

  virtual bool stripMarkup();

  //const coopy::store::CsvSheet& get() { return result; }

  virtual coopy::store::PolySheet getSheet() {
    if (useFallback) {
      return fallback;
    }
    if (!sync) {
      coopy::store::PolySheet s = MergeOutput::getSheet();
      s.copyData(result);
      sync = true;
    }
    return output_sheet;
  }

};

#endif
