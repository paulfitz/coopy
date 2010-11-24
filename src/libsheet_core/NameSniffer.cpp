#include <coopy/NameSniffer.h>
#include <coopy/DataStat.h>

#include <map>

using namespace coopy::store;
using namespace coopy::cmp;
using namespace std;

void NameSniffer::sniff() {
  embed = false;
  fake = false;
  names.clear();
  ct.clear();

  SheetSchema *schema = sheet.getSchema();
  if (schema!=NULL) {
    dbg_printf("Sniffing... found schema!\n");
    for (int i=0; i<sheet.width(); i++) {
      ColumnInfo info = schema->getColumnInfo(i);
      if (!info.hasName()) {
	names.clear();
	ct.clear();
	break;
      }
      names.push_back(info.getName());
      ct.push_back(info.getColumnType());
    }
    if (names.size()>0) {
      dbg_printf("Found names in schema\n");
      return;
    }
  }

  DataStat stat;
  stat.evaluate(sheet);
  div = stat.getRowDivider();
  ct = stat.suggestTypes();
  if (div<0) {
    // no obvious header
    fake = true;
    return;
  }

  map<string,int> nameCheck;
  bool failure = false;
  for (int i=0; i<sheet.width(); i++) {
    string name = sheet.cellString(i,div);
    if (name=="") {
      dbg_printf("Reject header, blank name\n");
      failure = true;
      break;
    }
    if (nameCheck.find(name)!=nameCheck.end()) {
      dbg_printf("Reject header, repeated name %s\n", name.c_str());
      failure = true;
      break;
    }
    nameCheck[name] = 1;
    names.push_back(name);
  }
  if (failure) {
    names.clear();
    ct.clear();
    fake = true;
  } else {
    embed = true;
  }
}


std::string NameSniffer::suggestColumnName(int col) {
  if (names.size()>0) {
    return names[col];
  }
  char buf[256];
  sprintf(buf,"%d",col);
  return buf;
}
