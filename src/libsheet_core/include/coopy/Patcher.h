#ifndef COOPY_PATCHER
#define COOPY_PATCHER

#include <string>
#include <vector>
#include <map>

namespace coopy {
  namespace cmp {
    class OrderChange;
    class RowChange;
    class Patcher;

    enum {
      ORDER_CHANGE_NONE,
      ORDER_CHANGE_DELETE,
      ORDER_CHANGE_INSERT,
      ORDER_CHANGE_MOVE,
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

  std::string modeString() const {
    switch (mode) {
    case ORDER_CHANGE_NONE:
      return "none";
    case ORDER_CHANGE_DELETE:
      return "delete";
    case ORDER_CHANGE_INSERT:
      return "insert";
    case ORDER_CHANGE_MOVE:
      return "move";
    }
    return "unknown";
  }
};


class coopy::cmp::RowChange {
public:
  int mode;
  std::map<std::string,std::string> cond;
  std::map<std::string,std::string> val;
  std::vector<std::string> names;

  std::string modeString() const {
    switch (mode) {
    case ROW_CHANGE_NONE:
      return "none";
    case ROW_CHANGE_DELETE:
      return "delete";
    case ROW_CHANGE_INSERT:
      return "insert";
    case ROW_CHANGE_UPDATE:
      return "update";
      //case ROW_CHANGE_MOVE:
      //return "move";
    }
    return "unknown";
  }
};


class coopy::cmp::Patcher {
public:
  Patcher() {
  }

  virtual ~Patcher() {}

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

  virtual bool mergeDone() {
    return true;
  }

  virtual bool setSheet(const char *name) {
    return false;
  }
};



#endif
