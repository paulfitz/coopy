

/*
  Changes in 0.4:
  Version number change.
  The special "*" character goes away.
  A "row transform" operation is added.
  Context distinguished from index

  Changes in 0.5:
  sheet command
  
 */

#include <coopy/MergeOutputCsvDiff.h>
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

#define OP_MATCH "*"
#define OP_ASSIGN "="
#define OP_MATCH_ASSIGN "*="
#define OP_CONTEXT "#"
#define OP_NONE ""

MergeOutputCsvDiff::MergeOutputCsvDiff() {
}

bool MergeOutputCsvDiff::mergeStart() {

  currentSheetName = "";
  pendingSheetName = "";
  result.setStrict(0);
  mergeClear();
  result.addField("dtbl",false);
  result.addField("csv",false);
  result.addField("version",false);
  result.addField("0.5",false);
  result.addRecord();

  return true;
}

bool MergeOutputCsvDiff::mergeClear() {
  ops.clear();
  nops.clear();
  activeColumn.clear();
  showForSelect.clear();
  showForDescribe.clear();
  prevSelect.clear();
  prevDescribe.clear();
  constantColumns = true;
  columns.clear();
  showedColumns = false;
  return true;
}


bool MergeOutputCsvDiff::mergeDone() {
  mergeClear();
  return true;
}

bool MergeOutputCsvDiff::mergeAllDone() {
  SheetStyle style;
  //SheetCell c = result.cellSummary(0,0);
  fprintf(out,"%s",result.encode(style).c_str());
  return true;
}


bool MergeOutputCsvDiff::changeColumn(const OrderChange& change) {
  constantColumns = false;
  switch (change.mode) {
  case ORDER_CHANGE_DELETE:
    clearThroat();
    result.addField("column",false);
    result.addField("delete",false);
    //result.addField(ROW_COL,false);
    for (int i=0; i<(int)change.namesAfter.size(); i++) {
      result.addField(change.namesAfter[i].c_str(),false);
    }
    result.addRecord();
    break;
  case ORDER_CHANGE_INSERT:
    clearThroat();
    result.addField("column",false);
    result.addField("insert",false);
    //result.addField(ROW_COL,false);
    for (int i=0; i<(int)change.namesAfter.size(); i++) {
      result.addField(change.namesAfter[i].c_str(),false);
    }
    result.addRecord();
    break;
  case ORDER_CHANGE_MOVE:
    clearThroat();
    result.addField("column",false);
    result.addField("move",false);
    //result.addField(ROW_COL,false);
    for (int i=0; i<(int)change.namesAfter.size(); i++) {
      result.addField(change.namesAfter[i].c_str(),false);
    }
    result.addRecord();
    break;
  case ORDER_CHANGE_RENAME:
    clearThroat();
    result.addField("column",false);
    result.addField("rename",false);
    for (int i=0; i<(int)change.namesAfter.size(); i++) {
      result.addField(change.namesAfter[i].c_str(),false);
    }
    result.addRecord();
    break;
  default:
    fprintf(stderr,"  Unknown column operation\n\n");
    exit(1);
    break;
  }
  activeColumn.clear();
  for (int i=0; i<(int)change.namesAfter.size(); i++) {
    activeColumn[change.namesAfter[i]] = true;
  }
  nops = change.namesAfter;
  return true;
}

bool MergeOutputCsvDiff::operateRow(const RowChange& change, const char *tag) {
  vector<string> lnops;
  for (int i=0; i<(int)change.names.size(); i++) {
    if (activeColumn[change.names[i]]) {
      lnops.push_back(change.names[i]);
    }
  }
  if (lnops!=nops) {
    if (!showedColumns) {
      clearThroat();
      result.addField("column",false);
      result.addField("name",false);
      for (int i=0; i<(int)columns.size(); i++) {
	result.addField(columns[i].c_str(),false);
      }
      result.addRecord();
      showedColumns = true;
    }
    if (columns!=lnops) {
      clearThroat();
      result.addField("link",false);
      result.addField("name",false);
      for (int i=0; i<(int)change.names.size(); i++) {
	if (activeColumn[change.names[i]]) {
	  result.addField(change.names[i].c_str(),false);
	}
      }
      result.addRecord();
      columns = lnops;
    }
    nops = lnops;
  }

  if (prevSelect!=showForSelect || prevDescribe!=showForDescribe) {
    clearThroat();
    result.addField((tag=="act")?"link":"row",false);
    result.addField(tag,false);
    for (int i=0; i<(int)change.names.size(); i++) {
      if (activeColumn[change.names[i]]) {
	result.addField(ops[i].c_str(),false);
      }
    }
    result.addRecord();
  }
  return true;
}

bool MergeOutputCsvDiff::updateRow(const RowChange& change, const char *tag,
				   bool select, bool update, bool practice) {
  bool ok = true;

  if (!practice) {
    clearThroat();
    result.addField("row",false);
    result.addField(tag,false);
  }
  for (int i=0; i<(int)change.names.size(); i++) {
    string name = change.names[i];
    if (activeColumn[name]) {
      bool shown = false;
      if (change.cond.find(name)!=change.cond.end() && 
	  showForSelect[name] && select) {
	if (!practice) {
	  clearThroat();
	  result.addField(change.cond.find(name)->second);
	}
	shown = true;
      }
      if (change.val.find(name)!=change.val.end() && 
	  showForDescribe[name] && update) {
	if (!practice) {
	  clearThroat();
	  result.addField(change.val.find(name)->second);
	}
	if (shown) ok = false; // collision
	shown = true;
      }
      if (!shown) {
	if (!practice) {
	  clearThroat();
	  result.addField("",false);
	}
      }
    }
  }
  if (!practice) {
    result.addRecord();
  }
  return ok;
}

bool MergeOutputCsvDiff::changeRow(const RowChange& change) {
  vector<string> lops;
  activeColumn.clear();
  prevSelect = showForSelect;
  prevDescribe = showForDescribe;
  showForSelect.clear();
  showForDescribe.clear();
  for (int i=0; i<(int)change.names.size(); i++) {
    string name = change.names[i];
    bool condActive = false;
    bool valueActive = false;
    if (change.cond.find(name)!=change.cond.end()) {
      condActive = true;
    }
    if (change.val.find(name)!=change.val.end()) {
      valueActive = true;
    }
    bool shouldMatch = condActive && change.indexes.find(name)->second;
    bool shouldAssign = valueActive;
    if (shouldAssign) {
      // conservative choice, should be optional
      if (change.cond.find(name)!=change.cond.end()) {
	shouldMatch = true;
      }
    }
    //printf("==> %s %d %d %d %d\n", name.c_str(), condActive, valueActive,
    //shouldMatch, shouldAssign);
    //bool shouldShow = shouldMatch || shouldAssign;

    if (change.mode==ROW_CHANGE_INSERT) {
      // we do not care about matching
      shouldMatch = prevSelect[name];
      shouldAssign = true;
    }
    if (change.mode==ROW_CHANGE_DELETE) {
      // we do not care about assigning
      shouldAssign = prevDescribe[name];
      shouldMatch = true;
    }

    // ignoring shouldShow for now.
    int opidx = (shouldMatch?2:0) + (shouldAssign?1:0);
    string opi[4] = {
      OP_NONE,         // !match  !assign
      OP_ASSIGN,       // !match   assign
      OP_MATCH,        //  match  !assign
      OP_MATCH_ASSIGN, //  match   assign
    };
    string op = opi[opidx];
    
    if (opidx!=0) {
      activeColumn[name] = true;
    }

    // no way yet to communicate CONTEXT request
    lops.push_back(op);
    //showForSelect.push_back(shouldMatch);
    //showForDescribe.push_back(shouldAssign);
    showForSelect[name] = shouldMatch;
    showForDescribe[name] = shouldAssign;
  }

  if (lops!=ops) {
    ops = lops;
    operateRow(change,"act");
  }
  switch (change.mode) {
  case ROW_CHANGE_INSERT:
    updateRow(change,"insert",false,true,false);
    break;
  case ROW_CHANGE_DELETE:
    updateRow(change,"delete",true,false,false);
    break;
  case ROW_CHANGE_MOVE:
    {
      bool terse = updateRow(change,"practice",true,true,true);
      if (terse) {
	updateRow(change,"move",true,true,false);
      } else {
	updateRow(change,"select",true,false,false);
	updateRow(change,"move",false,true,false);
      }
    }
    break;
  case ROW_CHANGE_CONTEXT:
    updateRow(change,(change.cond.size()>0)?"after":"start",true,false,false);
    break;
  case ROW_CHANGE_UPDATE:
    {
      bool terse = updateRow(change,"practice",true,true,true);
      if (terse) {
	updateRow(change,"update",true,true,false);
      } else {
	updateRow(change,"select",true,false,false);
	updateRow(change,"update",false,true,false);
      }
    }
    break;
  default:
    fprintf(stderr,"  Unknown row operation\n\n");
    exit(1);
    break;
  }
  return true;
}


bool MergeOutputCsvDiff::changeName(const NameChange& change) {
  const vector<string>& names = change.names;
  bool final = change.final;
  bool constant = change.constant;
  if (!final) {
    activeColumn.clear();
    for (int i=0; i<(int)names.size(); i++) {
      activeColumn[names[i]] = true;
      showForSelect[names[i]] = true;
      showForDescribe[names[i]] = true;
    }
    if (!constant) {
      clearThroat();
      result.addField("column",false);
      result.addField("name",false);
      //result.addField(ROW_COL,false);
      for (int i=0; i<(int)names.size(); i++) {
	result.addField(names[i].c_str(),false);
      }
      result.addRecord();
      showedColumns = true;
    }
  }
  columns = names;
  return true;
}

bool MergeOutputCsvDiff::setSheet(const char *name) {
  pendingSheetName = name;
  return true;
}

bool MergeOutputCsvDiff::clearThroat() {
  if (currentSheetName!=pendingSheetName) {
    result.addField("table",false);
    result.addField("name",false);
    result.addField(pendingSheetName.c_str(),false);
    result.addRecord();
    currentSheetName = pendingSheetName;
  }
  return true;
}
