#ifndef COOPY_MERGEOUTPUT
#define COOPY_MERGEOUTPUT

#include <coopy/Patcher.h>
#include <coopy/CompareFlags.h>

namespace coopy {
  namespace cmp {
    class MergeOutput;
  }
}

/**
 *
 * A generator of descriptions of a merge.  It contains a collection
 * of callbacks, called at various stages during a merge.  Callbacks
 * can be ignored if not relevant to the description being generated.
 *
 */
class coopy::cmp::MergeOutput : public Patcher {
private:
  int ct;
protected:
  CompareFlags flags;
  FILE *out;
public:
  MergeOutput() {
    ct = 0;
    out = stdout;
  }

  virtual ~MergeOutput() {}

  virtual bool setFlags(const CompareFlags& flags) {
    this->flags = flags;
    out = flags.out;
  }

  virtual bool wantDiff() { return false; }

  virtual bool addRow(const char *tag,
		      const std::vector<std::string>& row,
		      const std::string& blank) { return false; }

  virtual bool addHeader(const char *tag,
			 const std::vector<std::string>& row,
			 const std::string& blank) {
    return addRow(tag,row,blank);
  }

  virtual bool stripMarkup() { return false; }

  virtual bool changeColumn(const OrderChange& change) { return false; }

  virtual bool changeRow(const RowChange& change) { return false; }

  /**
   *
   * Called with a sequence of column names.  It is called twice.
   * First, with final=false, giving an initial sequence.
   * Then, a series of changeColumn() calls may happen, specifying
   * manipulations of the columns.  After all changeColumn() calls
   * have happened, this method is called again, with final=true,
   * giving a final sequence of column names.
   *
   */
  virtual bool declareNames(const std::vector<std::string>& names, bool final) {
    return false;
  }

  virtual bool mergeStart() {
    return true;
  }

  virtual bool mergeDone() {
    return true;
  }

  virtual bool setSheet(const char *name) { 
    ct++;
    return (ct<=1); 
  }
};

#endif
