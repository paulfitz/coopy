#include <coopy/MergeOutputVerboseDiff.h>
#include <coopy/SheetStyle.h>
#include <coopy/DataSheet.h>

#include <stdlib.h>

#define WANT_MAP2STRING
#define WANT_VECTOR2STRING
#include <coopy/Stringer.h>

using namespace std;

string encoder(string x) {
  SheetStyle style;
  string result = DataSheet::encodeCell(x,style);
  return (result!="")?result:"\"\"";
}

static string cond(const vector<string>& names,
		   const map<string,string>& conds,
		   const map<string,string>& vals,
		   bool act) {
  string c = "";
  string pv = "";
  string v = "";
  for (vector<string>::const_iterator it = names.begin();
       it!=names.end();
       it++) {
    string name = *it;
    if (act) {
      if (vals.find(name)!=vals.end()) {
	string pval = conds.find(name)->second;
	string val = vals.find(name)->second;
	if (pv!="") pv += ",";
	pv += encoder(pval);
	if (v!="") v += ",";
	v += encoder(val);
	if (c!="") c += ",";
	c += encoder(name);
      }
    } else {
      if (conds.find(name)!=conds.end()) {
	if (vals.find(name)==vals.end()) {
	  string val = conds.find(name)->second;
	  if (c!="") c += ",";
	  c += encoder(name);
	  if (v!="") v += ",";
	  v += encoder(val);
	}
      }
    }
  }
  if (act) {
    if (pv == "") {
    }
    return c + " = " + pv + " -> " + v;
  }
  return c + " = " + v;
}

static string cond(const vector<string>& names,
		   const map<string,string>& vals) {
  SheetStyle style;
  string c = "";
  string v = "";
  size_t ct = 0;
  for (vector<string>::const_iterator it = names.begin();
       it!=names.end();
       it++) {
    string name = *it;
    if (vals.find(name)!=vals.end()) {
      ct++;
      string val = vals.find(name)->second;
      if (c!="") c += ",";
      c += encoder(name);
      if (v!="") v += ",";
      v += encoder(val);
    }
  }
  if (ct==names.size()) {
    c = "*";
  }
  return c + " = " + v;
}

static string cond(const vector<string>& names) {
  SheetStyle style;
  string c = "";
  for (vector<string>::const_iterator it = names.begin();
       it!=names.end();
       it++) {
    string name = *it;
    if (c!="") c += ",";
    c += DataSheet::encodeCell(name,style);
  }
  return c;
}

bool MergeOutputVerboseDiff::changeColumn(const OrderChange& change) {
  checkMessage();
  //printf("Got order change %s -> %s\n",
  //vector2string(change.namesBefore).c_str(),
  //vector2string(change.namesAfter).c_str());
  if (change.mode==ORDER_CHANGE_DELETE) {
    printf("delete column %s\n", 
	   change.namesBefore[change.subject].c_str());
  } else {
    printf("insert column %s in position %d\n", 
	   change.namesAfter[change.subject].c_str(),
	   change.subject);
  }
  return true;
}

bool MergeOutputVerboseDiff::changeRow(const RowChange& change) {
  checkMessage();
  //printf("Got row change %s ==> %s\n",
  // map2string(change.cond).c_str(),
  // map2string(change.val).c_str());
  switch (change.mode) {
  case ROW_CHANGE_INSERT:
    printf("insert row with %s\n",
	   cond(change.names,change.val).c_str());
    break;
  case ROW_CHANGE_DELETE:
    printf("delete row with %s\n",
	   cond(change.names,change.cond).c_str());
    break;
  case ROW_CHANGE_UPDATE:
    printf("for row with %s set %s\n",
	   cond(change.names,change.cond,change.val,false).c_str(),
	   cond(change.names,change.cond,change.val,true).c_str());
    break;
  default:
    printf("  Unknown row operation\n");
    exit(1);
    break;
  }
  return true;
}


bool MergeOutputVerboseDiff::declareNames(const vector<string>& names, 
					  bool final) {
  string tag = "initial ";
  if (final) {
    tag = "";
  }
  string result = tag+"column names are "+cond(names);
  pending_message = result;
  return true;
}


void MergeOutputVerboseDiff::checkMessage() {
  if (pending_message!="") {
    printf("%s\n", pending_message.c_str());
    pending_message = "";
  }
}


