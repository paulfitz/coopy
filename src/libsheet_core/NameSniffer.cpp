#include <coopy/NameSniffer.h>
#include <coopy/DataStat.h>

#include <map>

using namespace coopy::store;
using namespace coopy::cmp;
using namespace std;

void NameSniffer::sniff() {
  if (sniffed) return;
  sniffed = true;
  embed = false;
  fake = false;
  names.clear();
  ct.clear();

  SheetSchema *schema = sheet.getSchema();
  div = -1;
  if (schema!=NULL) {
    if (schema->getColumnCount()==0 && schema->headerHeight()>=0) {
      // minimal schema, not complete
      dbg_printf("Sniffing... minimal schema!\n");
      div = schema->headerHeight();
    } else {
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
  } else {
    dbg_printf("Full sniff\n");
  }

  if (div<0) {
    DataStat stat;
    stat.evaluate(sheet);
    div = stat.getRowDivider();
    ct = stat.suggestTypes();
  }
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
    while (ct.size()<names.size()) {
      ct.push_back(ColumnType());
    }
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


bool NameSniffer::subset(std::vector<std::string>& ext) {
  subset_index.clear();
  for (int i=0; i<(int)ext.size(); i++) {
    for (int j=0; j<(int)names.size(); j++) {
      if (ext[i]==names[j]) {
	dbg_printf("pos %d %s\n", j, names[j].c_str());
	subset_index.push_back(j);
	break;
      }
    }
  }
  return subset_index.size()==ext.size();
}
