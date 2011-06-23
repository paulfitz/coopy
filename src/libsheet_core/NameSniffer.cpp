#include <coopy/NameSniffer.h>
#include <coopy/DataStat.h>
#include <coopy/Stringer.h>

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
      if (sheet.width()!=schema->getColumnCount()) {
	dbg_printf("Problem detecting schema\n");
	dbg_printf("  table has %d columns\n", sheet.width());
	dbg_printf("  schema has %d columns\n", schema->getColumnCount());
      } else {
	COOPY_ASSERT(sheet.width()==schema->getColumnCount());
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
    }
  } else {
    dbg_printf("Full sniff\n");
  }

  if (div<0) {
    DataStat stat;
    stat.evaluate2(sheet,flags);
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
  string lastName = "";
  for (int i=0; i<sheet.width(); i++) {
    string name = sheet.cellString(i,div);
    if (name=="") {
      string below = ".";
      if (sheet.height()>div) {
	below = sheet.cellString(i,div+1);
      }
      if (below!="") {
	dbg_printf("Reject header, blank name\n");
	failure = true;
	break;
      }
    } else {
      lastName = name;
    }
    if (name=="") {
      name = lastName + "_";
      lastName = name;
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
  return Stringer::getSpreadsheetColumnName(col);
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
    if (ext[i].size()==1) {
      int j = ext[i][0]-'A';
      if (j>=0&&j<=26) {
	dbg_printf("pos %d %s\n", j, names[j].c_str());
	subset_index.push_back(j);
	break;
      }
    }
  }
  return subset_index.size()==ext.size();
}
