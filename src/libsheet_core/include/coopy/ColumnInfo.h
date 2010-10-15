#ifndef COOPY_COLUMNINFO
#define COOPY_COLUMNINFO

#include <string>

namespace coopy {
  namespace store {
    class ColumnInfo;
  }
}

class coopy::store::ColumnInfo {
public:
  ColumnInfo() {}

  virtual ~ColumnInfo() {}

  virtual bool hasName() const { return false; }

  virtual std::string getName() const;
};

#endif
