#ifndef COOPY_POOLIMPL
#define COOPY_POOLIMPL

#include <coopy/Pool.h>

#include <map>

namespace coopy {
  namespace store {
    class PoolLinkImpl;
    class PoolSlice;
    class PoolImpl;
  }
}


class coopy::store::PoolLinkImpl {
public:
  std::string name;
  bool invent;

  PoolLinkImpl() {
    invent = 0;
  }
};

class coopy::store::PoolSlice : public PoolColumn {
public:
  std::string pool_name;
  std::map<std::string,SheetCell> item;

  virtual bool is_valid() const {
    return pool_name!="";
  }

  virtual SheetCell lookup(const SheetCell& val, bool& match);
};

class coopy::store::PoolImpl : public Pool {
private:
  std::map<std::string,PoolLinkImpl> pool_link;
  std::map<std::string,PoolSlice> pool;
  PoolSlice null_column;

  std::string getKey(const std::string& table_name,
		     const std::string& column_name) {
    std::string key = table_name + ":::" + column_name;
    return key;
  }

public:

  virtual bool create(const std::string& key,
		      const std::string& table_name,
		      const std::string& column_name,
		      bool invent);

  virtual PoolColumnLink lookup(const std::string& table_name,
				const std::string& column_name);

  virtual SheetCell lookup(const std::string& table_name,
			   const std::string& column_name,
			   const SheetCell& val,
			   bool& match);
};

#endif
