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
  PoolLinkImpl link;
  link.name = key;
  link.invent = invent;
  pool_link[getKey(table_name,column_name)] = link;
  return true;
}


SheetCell PoolImpl::lookup(const std::string& table_name,
			   const std::string& column_name,
			   const SheetCell& val,
			   bool& match) {
  PoolColumnLink col = lookup(table_name,column_name);
  if (!col.is_valid()) {
    match = false;
    return SheetCell();
  }
  match = true;
  return col.get_column().lookup(val,match);
}



PoolColumnLink PoolImpl::lookup(const std::string& table_name,
				const std::string& column_name) {
  std::string key = getKey(table_name,column_name);
  std::map<std::string,PoolLinkImpl>::iterator link = pool_link.find(key);
  if (link==pool_link.end()) {
    return PoolColumnLink(null_column,false);
  }
  key = link->second.name;
  std::map<std::string,PoolSlice>::iterator p = pool.find(key);
  if (p==pool.end()) {
    return PoolColumnLink(null_column,false);
  }
  return PoolColumnLink(p->second,link->second.invent,
			table_name,column_name,link->second.name);
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
