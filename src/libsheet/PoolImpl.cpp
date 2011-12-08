#include <coopy/PoolImpl.h>
#include <coopy/Dbg.h>


using namespace coopy::store;
using namespace std;


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
  if (!col.isValid()) {
    match = false;
    return SheetCell();
  }
  match = true;
  return col.getColumn().lookup(val,match);
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

bool PoolImpl::load() {
  dbg_printf("Load pool\n");
  vector<string> names = book.getNames();
  for (int k=0; k<(int)names.size(); k++) {
    string name = names[k];
    PoolSlice& slice = pool[name] = PoolSlice();
    slice.pool_name = name;
    PolySheet sheet = book.readSheet(name);
    for (int y=0; y<sheet.height(); y++) {
      slice.item[sheet.cellSummary(0,y).toString()] = sheet.cellSummary(1,y);
    }
  }
  return true;
}

bool PoolImpl::save() {
  dbg_printf("Save pool\n");
  for (map<string,PoolSlice>::iterator it=pool.begin(); it!=pool.end(); it++) {
    string name = it->first;
    PoolSlice& pool = it->second;
    SimpleSheetSchema schema;
    schema.setSheetName(name.c_str());
    schema.addColumn("local");
    schema.addColumn("remote");
    PolySheet sheet = book.provideSheet(schema);
    sheet.deleteData();
    for (map<string,SheetCell>::iterator it2 = pool.item.begin();
	 it2 != pool.item.end(); it2++) {
      string from = it2->first;
      SheetCell to = it2->second;
      Poly<SheetRow> row = sheet.insertRow();
      row->setCell(0,SheetCell(from,false)); // fix for escaped cells
      row->setCell(1,to);
      row->flush();
    }
  }

  return true;
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
