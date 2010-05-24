#ifndef COOPY_MERGEOUTPUT
#define COOPY_MERGEOUTPUT

#include <string>
#include <vector>

enum {
  ORDER_CHANGE_NONE,
  ORDER_CHANGE_DELETE,
  ORDER_CHANGE_INSERT,
};

class OrderChange {
public:
  // local-to-global mapping before and after change
  std::vector<int> indicesBefore;
  std::vector<std::string> namesBefore;
  std::vector<int> indicesAfter;
  std::vector<std::string> namesAfter;

  int subject; // subject in local coords

  int mode;
};

class MergeOutput {
public:
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
};

#endif
