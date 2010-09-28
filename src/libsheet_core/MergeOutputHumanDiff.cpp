#include <coopy/MergeOutputHumanDiff.h>
#include <coopy/SheetStyle.h>
#include <coopy/DataSheet.h>

#include <stdio.h>
#include <stdlib.h>

#define WANT_MAP2STRING
#define WANT_VECTOR2STRING
#include <coopy/Stringer.h>

using namespace std;
using namespace coopy::store;
using namespace coopy::cmp;

string encoder(const string& x) {
  //SheetStyle style;
  //string result = DataSheet::encodeCell(x,style);
  //return (result!="")?result:"\"\"";
  string result = stringer_encoder(x);
  if (result.find("=")!=result.npos) {
    if (result[0]!='"') {
      // force quoting of any material containing the "=" symbol
      result = string("\"")+result+string("\"");
    }
  }
  return result;
}

static string cond(const vector<string>& names,
		   const map<string,string>& conds,
		   const map<string,string>& vals,
		   bool act) {
  string c = "";
  string pv = "";
  string v = "";
  bool nontrivial_past = false;
  for (vector<string>::const_iterator it = names.begin();
       it!=names.end();
       it++) {
    string name = *it;
    if (act) {
      //if (conds.find(name)!=conds.end()) {
	//fprintf(stderr,"Missing condition for %s\n", name.c_str());
	//exit(1);
      if (vals.find(name)!=vals.end()) {
	string pval;
	string val = vals.find(name)->second;
	if (conds.find(name)!=conds.end()) {
	  pval = conds.find(name)->second;
	  if (pval!="") {
	    nontrivial_past = true;
	  }
	}
	if (pv!="") pv += ",";
	pv += encoder(pval);
	if (v!="") v += ",";
	v += encoder(val);
	if (c!="") c += ",";
	c += encoder(name);
      }
	//}
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
    if (pv == "" || !nontrivial_past) {
      return c + " = " + v;
    }
    return c + " = " + pv + " -> " + v;
  }
  return c + " = " + v;
}

static string cond(const vector<string>& names,
		   const map<string,string>& vals,
		   string val_label, string cond_label) {
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
      if (c!="") c += " ";
      c += encoder(name);
      if (v!="") v += " ";
      v += encoder(val);
    }
  }
  if (ct==names.size()) {
    c = "*";
    return string("  ") + val_label + " " + v;
  }
  return string("  ") + cond_label + " " + c + "\n  " + val_label + " " + v;
}

static string cond(const vector<string>& names) {
  string c = "";
  for (vector<string>::const_iterator it = names.begin();
       it!=names.end();
       it++) {
    string name = *it;
    if (c!="") c += " ";
    c += encoder(name);
  }
  return c;
}

MergeOutputHumanDiff::MergeOutputHumanDiff() {
  printf("dtbl: table difference format version 0.2, human-readable flavor\n");
  printf("This format should be considered unstable until 1.0\n\n");
  showed_initial_columns = false;
}

bool MergeOutputHumanDiff::mergeDone() {
  checkMessage();
}

bool MergeOutputHumanDiff::changeColumn(const OrderChange& change) {
  checkMessage();
  //printf("Got order change %s -> %s\n",
  //vector2string(change.namesBefore).c_str(),
  //vector2string(change.namesAfter).c_str());
  switch (change.mode) {
  case ORDER_CHANGE_DELETE:
    printf("delete column: %s\n  before %s\n  after  %s\n\n", 
	   change.namesBefore[change.subject].c_str(),
	   vector2string(change.namesBefore).c_str(),
	   vector2string(change.namesAfter).c_str());
    break;
  case ORDER_CHANGE_INSERT:
    printf("insert column: %s\n  before %s\n  after  %s\n\n", 
	   change.namesAfter[change.subject].c_str(),
	   vector2string(change.namesBefore).c_str(),
	   vector2string(change.namesAfter).c_str());
    break;
  case ORDER_CHANGE_MOVE:
    printf("move column: %s\n  before %s\n  after  %s\n\n", 
	   change.namesBefore[change.subject].c_str(),
	   vector2string(change.namesBefore).c_str(),
	   vector2string(change.namesAfter).c_str());
    break;
  default:
    printf("  Unknown column operation\n\n");
    exit(1);
    break;
  }
  return true;
}

bool MergeOutputHumanDiff::changeRow(const RowChange& change) {
  checkMessage();
  //printf("Got row change %s ==> %s\n",
  // map2string(change.cond).c_str(),
  // map2string(change.val).c_str());
  switch (change.mode) {
  case ROW_CHANGE_INSERT:
    printf("insert row:\n%s\n\n",
	   cond(change.names,change.val,"add","where").c_str());
    break;
  case ROW_CHANGE_DELETE:
    printf("delete row:\n%s\n\n",
	   cond(change.names,change.cond,"remove","where").c_str());
    break;
  case ROW_CHANGE_UPDATE:
    printf("update row:\n  where %s\n  set   %s\n\n",
	   cond(change.names,change.cond,change.val,false).c_str(),
	   cond(change.names,change.cond,change.val,true).c_str());
    break;
  default:
    printf("  Unknown row operation\n\n");
    exit(1);
    break;
  }
  return true;
}


bool MergeOutputHumanDiff::declareNames(const vector<string>& names, 
					  bool final) {
  string tag = "original ";
  string now = "";
  if (final) {
    tag = "";
    if (showed_initial_columns&&pending_message=="") {
      now = " now";
    }
  } else {
    showed_initial_columns = true;
  }
  string result = tag+"column names are"+now+": "+cond(names);
  pending_message = result;
  return true;
}


void MergeOutputHumanDiff::checkMessage() {
  if (pending_message!="") {
    printf("%s\n\n", pending_message.c_str());
    pending_message = "";
  }
}


