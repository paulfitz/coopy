#ifndef COOPY_COLUMNINFO
#define COOPY_COLUMNINFO

#include <string>

namespace coopy {
  namespace store {
    class ColumnInfo;
  }
}

class coopy::store::ColumnInfo {
private:
  std::string name;
  bool nameSet;
  bool primaryKey;
  bool primaryKeySet;
public:
  ColumnInfo() : nameSet(false), primaryKeySet(false), primaryKey(false) {}
  ColumnInfo(std::string name) : name(name), nameSet(true), primaryKeySet(false), primaryKey(false) {}
  ColumnInfo(std::string name, bool primary) : name(name), nameSet(true), primaryKeySet(true), primaryKey(primary) {}
  
  virtual ~ColumnInfo() {}
  
  virtual bool hasName() const { return nameSet; }
  
  virtual std::string getName() const { return name; }

  virtual bool hasPrimaryKey() const { return primaryKeySet; }
  
  virtual bool isPrimaryKey() const { return primaryKey; }
};

#endif
