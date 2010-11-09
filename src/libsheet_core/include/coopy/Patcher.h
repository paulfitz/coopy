#ifndef COOPY_PATCHER
#define COOPY_PATCHER

#include <string>
#include <vector>
#include <map>

#include <coopy/SheetCell.h>

namespace coopy {
  namespace cmp {
    class ConfigChange;
    class OrderChange;
    class RowChange;
    class NameChange;
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

    enum {
      NAME_CHANGE_NONE,
      NAME_CHANGE_DECLARE,
      NAME_CHANGE_SELECT,
    };
  }
}

class coopy::cmp::ConfigChange {
public:
  bool ordered;
  bool complete;
  bool trustNames;

  ConfigChange() {
    ordered = true;
    complete = true;
    trustNames = false;
  }
};

class coopy::cmp::OrderChange {
public:
  // local-to-global mapping before and after change
  std::vector<int> indicesBefore;
  std::vector<std::string> namesBefore;
  std::vector<int> indicesAfter;
  std::vector<std::string> namesAfter;

  int subject; // subject in local (prior-to-action) coords
  int object; // object in local (prior-to-action) coords

  int mode;

  int identityToIndex(int id) const;

  OrderChange() {
    subject = object = -1;
    mode = -1;
  }

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
  //typedef std::map<std::string,std::string> txt2txt;
  typedef std::map<std::string,coopy::store::SheetCell> txt2cell;
  typedef std::map<std::string,bool> txt2bool;
  txt2cell cond;
  txt2cell val;
  std::vector<std::string> names;
  txt2bool indexes;

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

class coopy::cmp::NameChange {
public:
  int mode;
  bool final;
  bool constant;
  std::vector<std::string> names;

  NameChange() {
    mode = -1;
    final = false;
    constant = false;
  }

  std::string modeString() const {
    switch (mode) {
    case NAME_CHANGE_NONE:
      return "none";
    case NAME_CHANGE_DECLARE:
      return "declare";
    case NAME_CHANGE_SELECT:
      return "select";
    }
    return "unknown";
  }
};

class coopy::cmp::Patcher {
public:
  Patcher() {
  }

  virtual ~Patcher() {}

  virtual bool changeConfig(const ConfigChange& change) { return false; }

  virtual bool changeColumn(const OrderChange& change) { return false; }

  virtual bool changeRow(const RowChange& change) { return false; }

  /**
   *
   * This method is headed toward deprecation in favor of changeName.
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

  virtual bool changeName(const NameChange& change) { 
    if (change.mode==NAME_CHANGE_DECLARE) {
      return declareNames(change.names,change.final);
    }
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
