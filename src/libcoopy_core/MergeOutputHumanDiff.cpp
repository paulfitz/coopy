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

static string encoder(const string& x) {
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

static string encoder(const SheetCell& x) {
  return encoder(x.text);
}

static string cond(const vector<string>& names,
		   const map<string,SheetCell>& conds,
		   const map<string,SheetCell>& vals,
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
	SheetCell pval;
	SheetCell val = vals.find(name)->second;
	if (conds.find(name)!=conds.end()) {
	  pval = conds.find(name)->second;
	  if (pval.text!="") {
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
	  SheetCell val = conds.find(name)->second;
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
		   const map<string,SheetCell>& vals,
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
      SheetCell val = vals.find(name)->second;
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
}

bool MergeOutputHumanDiff::mergeStart() {
  fprintf(out,"dtbl: human-readable table difference format version 0.3\n\n");
  showed_initial_columns = false;
  return true;
}

bool MergeOutputHumanDiff::mergeDone() {
  checkMessage();
  return true;
}

bool MergeOutputHumanDiff::changeColumn(const OrderChange& change) {
  checkMessage();
  //printf("Got order change %s -> %s\n",
  //vector2string(change.namesBefore).c_str(),
  //vector2string(change.namesAfter).c_str());
  switch (change.mode) {
  case ORDER_CHANGE_DELETE:
    fprintf(out,"delete column: %s\n  before %s\n  after  %s\n\n", 
	   change.namesBefore[change.subject].c_str(),
	   vector2string(change.namesBefore).c_str(),
	   vector2string(change.namesAfter).c_str());
    break;
  case ORDER_CHANGE_INSERT:
    fprintf(out,"insert column: %s\n  before %s\n  after  %s\n\n", 
	   change.namesAfter[change.subject].c_str(),
	   vector2string(change.namesBefore).c_str(),
	   vector2string(change.namesAfter).c_str());
    break;
  case ORDER_CHANGE_MOVE:
    fprintf(out,"move column: %s\n  before %s\n  after  %s\n\n", 
	   change.namesBefore[change.subject].c_str(),
	   vector2string(change.namesBefore).c_str(),
	   vector2string(change.namesAfter).c_str());
    break;
  default:
    fprintf(out,"  Unknown column operation\n\n");
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
    fprintf(out,"insert row:\n%s\n\n",
	   cond(change.names,change.val,"add","where").c_str());
    break;
  case ROW_CHANGE_DELETE:
    fprintf(out,"delete row:\n%s\n\n",
	   cond(change.names,change.cond,"remove","where").c_str());
    break;
  case ROW_CHANGE_UPDATE:
    fprintf(out,"update row:\n  where %s\n  set   %s\n\n",
	   cond(change.names,change.cond,change.val,false).c_str(),
	   cond(change.names,change.cond,change.val,true).c_str());
    break;
  default:
    fprintf(out,"  Unknown row operation\n\n");
    exit(1);
    break;
  }
  return true;
}


bool MergeOutputHumanDiff::declareNames(const std::vector<std::string>& names, 
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
    fprintf(out,"%s\n\n", pending_message.c_str());
    pending_message = "";
  }
}


