#include <coopy/MergeOutputCsvDiff.h>
#include <coopy/SheetStyle.h>
#include <coopy/DataSheet.h>

#include <stdio.h>
#include <stdlib.h>

#define WANT_MAP2STRING
#define WANT_VECTOR2STRING
#include <coopy/Stringer.h>
#define ROW_COL "ROW"

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

MergeOutputCsvDiff::MergeOutputCsvDiff() {
  result.setStrict(0);
  result.addField("dtbl");
  result.addField("csv");
  result.addField("version");
  result.addField("0.2");
  result.addRecord();
  result.addField("config");
  result.addField("empty_tag");
  result.addField("*");
  result.addRecord();
  result.addField("config");
  result.addField("row_tag");
  result.addField(ROW_COL);
  result.addRecord();
}

bool MergeOutputCsvDiff::mergeDone() {
  SheetStyle style;
  printf("%s",result.encode(style).c_str());
}

bool MergeOutputCsvDiff::changeColumn(const OrderChange& change) {
  switch (change.mode) {
  case ORDER_CHANGE_DELETE:
    result.addField("column");
    result.addField("delete");
    result.addField(ROW_COL);
    for (int i=0; i<(int)change.namesAfter.size(); i++) {
      result.addField(change.namesAfter[i].c_str());
    }
    result.addRecord();
    break;
  case ORDER_CHANGE_INSERT:
    result.addField("column");
    result.addField("insert");
    result.addField(ROW_COL);
    for (int i=0; i<(int)change.namesAfter.size(); i++) {
      result.addField(change.namesAfter[i].c_str());
    }
    result.addRecord();
    break;
  case ORDER_CHANGE_MOVE:
    result.addField("column");
    result.addField("move");
    result.addField(ROW_COL);
    for (int i=0; i<(int)change.namesAfter.size(); i++) {
      result.addField(change.namesAfter[i].c_str());
    }
    result.addRecord();
    break;
  default:
    printf("  Unknown column operation\n\n");
    exit(1);
    break;
  }
  return true;
}

bool MergeOutputCsvDiff::selectRow(const RowChange& change, const char *tag) {
  result.addField("row");
  result.addField(tag);
  result.addField("*");
  for (int i=0; i<(int)change.names.size(); i++) {
    string name = change.names[i];
    if (change.cond.find(name)!=change.cond.end()) {
      result.addField(change.cond.find(name)->second.c_str());
    } else {
      result.addField("*");
    }
  }
  result.addRecord();
  return true;
}

bool MergeOutputCsvDiff::describeRow(const RowChange& change, const char *tag) {
  result.addField("row");
  result.addField(tag);
  result.addField("*");
  for (int i=0; i<(int)change.names.size(); i++) {
    string name = change.names[i];
    if (change.val.find(name)!=change.val.end()) {
      result.addField(change.val.find(name)->second.c_str());
    } else {
      result.addField("*");
    }
  }
  result.addRecord();
  return true;
}

bool MergeOutputCsvDiff::changeRow(const RowChange& change) {
  switch (change.mode) {
  case ROW_CHANGE_INSERT:
    describeRow(change,"insert");
    break;
  case ROW_CHANGE_DELETE:
    selectRow(change,"delete");
    break;
  case ROW_CHANGE_UPDATE:
    selectRow(change,"select");
    describeRow(change,"update");
    break;
  default:
    printf("  Unknown row operation\n\n");
    exit(1);
    break;
  }
  return true;
}


bool MergeOutputCsvDiff::declareNames(const vector<string>& names, 
					  bool final) {
  if (!final) {
    result.addField("column");
    result.addField("name");
    result.addField(ROW_COL);
    for (int i=0; i<(int)names.size(); i++) {
      result.addField(names[i].c_str());
    }
    result.addRecord();
  }
  return true;
}

