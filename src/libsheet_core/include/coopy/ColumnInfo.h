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
public:
 ColumnInfo() : nameSet(false) {}
 ColumnInfo(std::string name) : name(name), nameSet(true) {}

  virtual ~ColumnInfo() {}

  virtual bool hasName() const { return nameSet; }

  virtual std::string getName() const { return name; }
};

#endif
