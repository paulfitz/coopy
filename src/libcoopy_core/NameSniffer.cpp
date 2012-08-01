#include <coopy/NameSniffer.h>
#include <coopy/DataStat.h>
#include <coopy/Stringer.h>

#include <map>

using namespace coopy::store;
using namespace coopy::cmp;
using namespace std;

void NameSniffer::sniff(int suggest) {
  if (sniffed) return;
  sniffed = true;
  embed = false;
  fake = false;
  canUseTop = false;
  names.clear();
  ct.clear();

  SheetSchema *schema = sheet.getSchema();

  /*
  if (schema==NULL) {
    schema = sheet.getMeta();
    if (schema!=NULL) {
      printf("Hey! I found a schema in getMeta.\n");
    }
  } 
  */
  /*
  else {
    SheetSchema *schema2 = sheet.getMeta();
    if (schema2!=NULL) {
      dbg_printf("Hey! I found two schema %ld\n",
		 (long int)(&sheet.tail_const()));
      dbg_printf("schema 1 %s\n", schema->toString().c_str());
      dbg_printf("schema 2 %s\n", schema2->toString().c_str());
    }
  }
  */

  div = suggest;
  if (schema!=NULL) {
    //printf("Working with %s\n", schema->toString().c_str());
    div = schema->headerHeight()-1;
    fake = schema->isGuess();
    if (schema->getColumnCount()==0 && schema->headerHeight()>0) {
      // minimal schema, not complete
      dbg_printf("Sniffing... minimal schema!\n");
      div = schema->headerHeight()-1;
    } else {
      dbg_printf("Sniffing... found schema! %s\n", schema->toString().c_str());
      if (sheet.width()!=schema->getColumnCount()) {
	dbg_printf("Problem detecting schema\n");
	dbg_printf("  table has %d columns\n", sheet.width());
	dbg_printf("  schema has %d columns\n", schema->getColumnCount());
	for (int i=0; i<schema->getColumnCount(); i++) {
	  dbg_printf("    Column %d: %s\n", i, schema->getColumnInfo(i).getName().c_str());
	}
	dbg_printf("Table contents:\n%s", sheet.toString().c_str());
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
	  dbg_printf("Found names in schema (%d)\n", names.size());
	  /*
	  if (schema->headerHeight()>0) {
	    dbg_printf("Also, embedded\n");
	    div = schema->headerHeight()-1;
	    embed = true;
	  }
	  */
	  if (!embed && sheet.width()==names.size() && 
	      sheet.height()>=1 && sheet.getDatabase()==NULL) {
	    bool ok = true;
	    for (int i=0; i<(int)names.size(); i++) {
	      if (sheet.cellString(i,0)!=names[i]) {
		ok = false;
		break;
	      }
	    }
	    if (ok) {
	      dbg_printf("Also, embedded, it seems\n");
	      div = 0;
	      embed = true;
	    }
	  }
	  while (ct.size()<names.size()) {
	    ct.push_back(ColumnType());
	  }
	  return;
	}
      }
    }
  } else {
    dbg_printf("Full sniff\n");
  }

  dbg_printf("NON-SCHEMA sniff\n");
  if (div<0) {
    DataStat stat;
    stat.evaluate2(sheet,flags);
    div = stat.getRowDivider();
    ct = stat.suggestTypes();
  }

  int adiv = div;
  if (div<0) {
    // no obvious header
    fake = true;
    while (ct.size()<names.size()) {
      ct.push_back(ColumnType());
    }
    if (sheet.height()<1) {
      return;
    }
    adiv = 0;
    canUseTop = true;
    return;
  }

  // try header line
  names.clear();
  map<string,int> nameCheck;
  bool failure = false;
  string lastName = "";
  for (int i=0; i<sheet.width(); i++) {
    string name = sheet.cellString(i,adiv);
    if (name=="") {
      string below = "";
      if (sheet.height()>adiv+1) {
	below = sheet.cellString(i,adiv+1);
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
    canUseTop = false;
  } 
  div = adiv;
  
  while (ct.size()<names.size()) {
    ct.push_back(ColumnType());
  }
  if (!failure) {
    embed = true;
  }
}


std::string NameSniffer::suggestColumnName(int col) const {
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


bool NameSniffer::resniff(NameSniffer& alt) {
  if (sheet.height()>5) return false;
  if (sheet.height()==0) return false;
  if (!fake) return false;
  if (alt.isFake()) return false;
  int ct = 0;
  std::vector<std::string> onames = alt.suggestNames();
  int at = -1;
  for (int k=0; k<(int)onames.size(); k++) {
    bool done = false;
    for (int i=0; i<sheet.width() && !done && at!=-2; i++) {
      for (int j=0; j<sheet.height(); j++) {
	if (sheet.cellString(i,j) == onames[k]) {
	  ct++;
	  if (at>=-1) {
	    at = j;
	  } else if (at!=j) {
	    at = -2;
	  }
	  done = true;
	  break;
	}
      }
    }
  }
  if (at>=0) {
    sniffed = false;
    sniff(at);
    return true;
  }
  return false;
}


