#include <coopy/PoolImpl.h>
#include <coopy/Dbg.h>


using namespace coopy::store;


bool PoolImpl::create(const std::string& key,
		      const std::string& table_name,
		      const std::string& column_name,
		      bool invent) {
  dbg_printf("Pool: %s %s:%s %s\n",
	     key.c_str(), table_name.c_str(), column_name.c_str(),
	     invent?"(create)":"");
  std::map<std::string,PoolSlice>::iterator p = pool.find(key);
  if (p==pool.end()) {
    pool[key] = PoolSlice();
    p = pool.find(key);
    p->second.pool_name = key;
  }
  PoolLink link;
  link.name = key;
  link.invent = invent;
  pool_link[getKey(table_name,column_name)] = link;
  return true;
}


SheetCell PoolImpl::lookup(const std::string& table_name,
			   const std::string& column_name,
			   const SheetCell& val,
			   bool& match) {
  PoolColumn& col = lookup(table_name,column_name);
  if (!col.isValid()) {
    match = false;
    return SheetCell();
  }
  match = true;
  return col.lookup(val,match);
}



PoolColumn& PoolImpl::lookup(const std::string& table_name,
			     const std::string& column_name) {
  std::string key = getKey(table_name,column_name);
  std::map<std::string,PoolLink>::iterator link = pool_link.find(key);
  if (link==pool_link.end()) {
    return null_column;
  }
  key = link->second.name;
  std::map<std::string,PoolSlice>::iterator p = pool.find(key);
  if (p==pool.end()) {
    return null_column;
  }
  return p->second;
}


SheetCell PoolSlice::lookup(const SheetCell& val, bool& match) {
  std::map<std::string,SheetCell>::iterator it = item.find(val.toString());
  if (it==item.end()) {
    match = false;
    return val;
  }
  match = true;
  return it->second;
}
