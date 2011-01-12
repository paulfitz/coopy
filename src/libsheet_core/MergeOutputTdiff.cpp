#include <coopy/MergeOutputTdiff.h>
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

MergeOutputTdiff::MergeOutputTdiff() {
  constantColumns = true;
  showedColumns = false;
}

bool MergeOutputTdiff::mergeStart() {
  fprintf(out,"# tdiff version 0.2\n\n");
  return true;
}

bool MergeOutputTdiff::mergeDone() {
}

bool MergeOutputTdiff::changeColumn(const OrderChange& change) {
  constantColumns = false;
  switch (change.mode) {
  case ORDER_CHANGE_DELETE:
    fprintf(out,"/* Column deletion ignored */\n\n");
    break;
  case ORDER_CHANGE_INSERT:
    fprintf(out,"/* Column insertion ignored */\n\n");
    break;
  case ORDER_CHANGE_MOVE:
    fprintf(out,"/* Column move ignored */\n\n");
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

bool MergeOutputTdiff::operateRow(const RowChange& change, const char *tag) {
  vector<string> lnops;
  for (int i=0; i<(int)change.names.size(); i++) {
    if (activeColumn[change.names[i]]) {
      lnops.push_back(change.names[i]);
    }
  }
  if (lnops!=nops) {
    /*
    if (!showedColumns) {
      fprintf(out, "%c* column name ",'/');
      for (int i=0; i<(int)columns.size(); i++) {
	fprintf(out,"%s ",columns[i].c_str());
      }
      fprintf(out,"*%c\n\n",'/');
      showedColumns = true;
    }
    */
    if (columns!=lnops || !showedColumns) {
      //fprintf(out, "/* link name ");
      fprintf(out, "@ |");
      for (int i=0; i<(int)change.names.size(); i++) {
	if (activeColumn[change.names[i]]) {
	  fprintf(out,"%s|",change.names[i].c_str());
	}
      }
      fprintf(out,"\n");
      showedColumns = true;
    }
    nops = lnops;
  }

  if (prevSelect!=showForSelect || prevDescribe!=showForDescribe) {
    //fprintf(out, "/* %s %s ",(tag=="act")?"link":"row",tag);
    /*
    for (int i=0; i<(int)change.names.size(); i++) {
      if (activeColumn[change.names[i]]) {
	fprintf(out,"%s ",ops[i].c_str());
      }
    }
    */
    //fprintf(out,"*/\n\n");
  }
  return true;
}

// practice mode is unnecessary for this output style
bool MergeOutputTdiff::updateRow(const RowChange& change, const char *tag,
				   bool select, bool update, bool practice) {
  bool ok = true;

  char ch = '?';
  if (!practice) {
    if (string(tag)=="update") {
      ch = '=';
    } else if (string(tag)=="insert") {
      ch = '+';
    } else if (string(tag)=="delete") {
      ch = '-';
    }
    fprintf(out, "%c |",ch);
  }
  for (int i=0; i<(int)change.names.size(); i++) {
    string name = change.names[i];
    if (activeColumn[name]) {
      bool shown = false;
      bool transition = false;
      if (change.cond.find(name)!=change.cond.end() && 
	  showForSelect[name] && select) {
	if (!practice) {
	  fprintf(out,"%s",change.cond.find(name)->second.toString().c_str());
	  transition = true;
	}
	shown = true;
      }
      if (change.val.find(name)!=change.val.end() && 
	  showForDescribe[name] && update) {
	if (!practice) {
	  fprintf(out,"%s%s",
		  transition?"->":"",
		  change.val.find(name)->second.toString().c_str());
	}
	if (shown) ok = false; // collision
	shown = true;
      }
      if (!shown) {
	if (!practice) {
	  fprintf(out,"*");
	}
      }
      fprintf(out,"|");
    }
  }
  if (!practice) {
    fprintf(out,"\n");
  }
  return ok;
}

bool MergeOutputTdiff::changeRow(const RowChange& change) {
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
    }
    if (change.mode==ROW_CHANGE_DELETE) {
      // we do not care about assigning
      shouldAssign = prevDescribe[name];
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
  case ROW_CHANGE_UPDATE:
    {
      //bool terse = updateRow(change,"practice",true,true,true);
      //if (terse) {
      updateRow(change,"update",true,true,false);
      //} else {
      //updateRow(change,"select",true,false,false);
      //updateRow(change,"update",false,true,false);
      //}
    }
    break;
  default:
    fprintf(stderr,"  Unknown row operation\n\n");
    exit(1);
    break;
  }
  return true;
}


bool MergeOutputTdiff::changeName(const NameChange& change) {
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
      fprintf(out, "/* %s %s ","column","name");
      //result.addField(ROW_COL,false);
      for (int i=0; i<(int)names.size(); i++) {
	fprintf(out,"%s ",names[i].c_str());
      }
      fprintf(out,"*/\n\n");
      showedColumns = true;
    }
  }
  columns = names;
  return true;
}

