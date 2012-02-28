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
  std::string table_name;
  std::string column_name;
  bool invent;

  PoolLinkImpl() {
    invent = 0;
  }
};

class coopy::store::PoolSlice : public PoolColumn {
public:
  PoolRecord dud;
  std::string pool_name;
  std::map<std::string,PoolRecord> item;

  virtual bool isValid() const {
    return pool_name!="";
  }

  virtual PoolRecord& lookupMod(const SheetCell& val, bool& match);

  virtual const PoolRecord& lookup(const SheetCell& val, bool& match) const;

  virtual PoolRecord& put(const SheetCell& src, const SheetCell& dest) {
    PoolRecord rec = {dest,true,false,true};
    return item[src.toString()] = rec;
  }
};

class coopy::store::PoolImpl : public Pool {
private:
  PolyBook book;
  std::map<std::string,PoolLinkImpl> pool_link;
  std::map<std::string,PoolLinkImpl *> pool_link_root;
  std::map<std::string,PoolSlice> pool;
  PoolSlice null_column;

  std::string getKey(const std::string& table_name,
		     const std::string& column_name) {
    std::string key = table_name + ":::" + column_name;
    return key;
  }

  PoolRecord dud;

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

  virtual PoolRecord& lookup(const std::string& table_name,
			     const std::string& column_name,
			     const SheetCell& val,
			     bool& match);

  virtual PoolColumnLink trace(const PoolColumnLink& src);

};

#endif
