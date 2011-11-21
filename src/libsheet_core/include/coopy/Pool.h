#ifndef COOPY_POOL
#define COOPY_POOL

#include <coopy/SheetCell.h>
#include <coopy/RefCount.h>

#include <map>

namespace coopy {
  namespace store {
    class Pool;
    class PoolColumn;
  }
}

class coopy::store::PoolColumn : public RefCount {
public:
  virtual ~PoolColumn() {}
  virtual bool isValid() const = 0;
  virtual SheetCell lookup(const SheetCell& val, bool& match) = 0;
};

class coopy::store::Pool {
public:
  virtual ~Pool() {}

  virtual bool create(const std::string& key,
		      const std::string& table_name,
		      const std::string& column_name,
		      bool invent) = 0;

  virtual PoolColumn& lookup(const std::string& table_name,
			     const std::string& column_name) = 0;

  virtual SheetCell lookup(const std::string& table_name,
			   const std::string& column_name,
			   const SheetCell& val,
			   bool& match) = 0;
};

#endif
