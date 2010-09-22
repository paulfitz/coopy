#ifndef COOPY_MERGEOUTPUT
#define COOPY_MERGEOUTPUT

#include <string>
#include <vector>
#include <map>

namespace coopy {
  namespace cmp {
    class OrderChange;
    class RowChange;
    class MergeOutput;

    enum {
      ORDER_CHANGE_NONE,
      ORDER_CHANGE_DELETE,
      ORDER_CHANGE_INSERT,
    };
    
    enum {
      ROW_CHANGE_NONE,
      ROW_CHANGE_DELETE,
      ROW_CHANGE_INSERT,
      ROW_CHANGE_UPDATE,
    };
  }
}

class coopy::cmp::OrderChange {
public:
  // local-to-global mapping before and after change
  std::vector<int> indicesBefore;
  std::vector<std::string> namesBefore;
  std::vector<int> indicesAfter;
  std::vector<std::string> namesAfter;

  int subject; // subject in local coords

  int mode;
};


class coopy::cmp::RowChange {
public:
  int mode;
  std::map<std::string,std::string> cond;
  std::map<std::string,std::string> val;
  std::vector<std::string> names;
};

class coopy::cmp::MergeOutput {
private:
  int ct;
public:
  MergeOutput() {
    ct = 0;
  }

  virtual ~MergeOutput() {}

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

  virtual bool declareNames(const std::vector<std::string>& names, bool final) {
    return false;
  }

  virtual bool setSheet(const char *name) { 
    ct++;
    return (ct<=1); 
  }
};

#endif
