#ifndef COOPY_POOLIMPL
#define COOPY_POOLIMPL

#include <coopy/Pool.h>
#include <coopy/PolyBook.h>

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

  virtual bool isValid() const {
    return pool_name!="";
  }

  virtual SheetCell lookup(const SheetCell& val, bool& match);

  virtual bool put(const SheetCell& src, const SheetCell& dest) {
    item[src.toString()] = dest;
    return true;
  }
};

class coopy::store::PoolImpl : public Pool {
private:
  PolyBook book;
  std::map<std::string,PoolLinkImpl> pool_link;
  std::map<std::string,PoolSlice> pool;
  PoolSlice null_column;

  std::string getKey(const std::string& table_name,
		     const std::string& column_name) {
    std::string key = table_name + ":::" + column_name;
    return key;
  }

public:
  void attachBook(PolyBook& book) {
    this->book = book;
    load();
  }

  bool load();

  bool save();

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
