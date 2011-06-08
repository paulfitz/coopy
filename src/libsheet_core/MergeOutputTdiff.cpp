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

#define OP_MATCH "..."
#define OP_ASSIGN "..."
#define OP_MATCH_ASSIGN "..."
#define OP_CONTEXT "#"
#define OP_NONE ""

static string celly(const SheetCell& c) {
  if (c.text=="NULL"&&!c.escaped) {
    return "\'NULL\'";
  }
  if (c.escaped) {
    return "NULL";
  }
  bool needQuote = false;
  bool lastWasMinus = false;
  for (int i=0; i<(int)c.text.length() && !needQuote; i++) {
    switch (c.text[i]) {
    case ':':
    case '=':
    case '|':
      needQuote = true;
      break;
    case '-':
      break;
    case '>':
      if (lastWasMinus) {
	needQuote = true;
      }
      break;
    case '\'':
      needQuote = true;
      break;
    default:
      if (c.text[i]<32) {
	needQuote = true;
      }
      break;
    }
    lastWasMinus = (c.text[i]=='-');
  }
  if (needQuote) {
    return quoteSql(c.text,'\'',true);
  }
  return c.text;
}

MergeOutputTdiff::MergeOutputTdiff() {
  setSheet("");
  sheetNameShown = true;
  lastWasFactored = false;
}

bool MergeOutputTdiff::mergeStart() {
  if (!getFlags().omit_format_name) {
    fprintf(out,"# tdiff version 0.3\n");
  }
  return true;
}

void MergeOutputTdiff::showSheet() {
  if (!sheetNameShown) {
    fprintf(out,"\n@@@ %s\n\n", sheetName.c_str());
    sheetNameShown = true;
  }
}


bool MergeOutputTdiff::mergeDone() {
  flushRows();
}

bool MergeOutputTdiff::changeColumn(const OrderChange& change) {
  showSheet();
  constantColumns = false;
  switch (change.mode) {
  case ORDER_CHANGE_DELETE:
    {
      int idx = change.identityToIndex(change.subject);
      if (change.namesBefore.size()<=idx) {
	fprintf(stderr, "Could not find column to remove\n");
	exit(1);
      } else {
	fprintf(out,"@- %s", change.namesBefore[idx].c_str());
      }
    }
    break;
  case ORDER_CHANGE_INSERT:
    {
      int idx = change.identityToIndexAfter(change.subject);
      if (change.namesAfter.size()<=idx) {
	fprintf(stderr, "Could not find column to insert\n");
	exit(1);
      } else {
	fprintf(out,"@+ %s", change.namesAfter[idx].c_str());
      }
    }
    break;
  case ORDER_CHANGE_MOVE:
    {
      int idx = change.identityToIndex(change.subject);
      if (change.namesBefore.size()<=idx) {
	fprintf(stderr, "Could not find column to move\n");
	exit(1);
      } else {
	fprintf(out,"@: %s", change.namesBefore[idx].c_str());
      }
    }
    break;
  default:
    fprintf(stderr,"  Unknown column operation\n\n");
    exit(1);
    break;
  }
  fprintf(out, " |");
  for (int i=0; i<(int)change.namesAfter.size(); i++) {
    fprintf(out,"%s|",change.namesAfter[i].c_str());
  }
  fprintf(out,"\n");

  activeColumn.clear();
  for (int i=0; i<(int)change.namesAfter.size(); i++) {
    activeColumn[change.namesAfter[i]] = true;
  }
  //nops = change.namesAfter;
  return true;
}

bool MergeOutputTdiff::operateRow(const RowChange& change, const char *tag) {
  /*
  vector<string> lnops;
  for (int i=0; i<(int)change.names.size(); i++) {
    if (activeColumn[change.names[i]]) {
      lnops.push_back(change.names[i]);
    }
  }
  */
  if (true) { //lnops!=nops) {
    if (true) {
      fprintf(out, "@ |");
      for (int i=0; i<(int)change.names.size(); i++) {
	if (activeColumn[change.names[i]]) {
	  bool select = check(showForSelect,change.names[i]);
	  bool cond = check(showForCond,change.names[i]);
	  bool view = check(showForDescribe,change.names[i]);
	  fprintf(out,"%s%s%s|",
		  change.names[i].c_str(),
		  select?"=":"",
		  (view&&!(cond||select))?"=":"");  // = was ->
	}
      }
      fprintf(out,"\n");
      showedColumns = true;
    }
    //nops = lnops;
  }

  return true;
}

// practice mode is unnecessary for this output style
bool MergeOutputTdiff::updateRow(const RowChange& change, const char *tag,
				 bool select, bool update, bool practice,
				 bool factored) {
  bool ok = true;

  char ch = '?';
  bool assign = false;
  if (!practice) {
    if (string(tag)=="update") {
      ch = '=';
      assign = true;
    } else if (string(tag)=="insert") {
      ch = '+';
    } else if (string(tag)=="delete") {
      ch = '-';
    } else if (string(tag)=="after") {
      ch = '*';
    } else if (string(tag)=="move") {
      ch = ':';
      assign = true;
    }
    fprintf(out, "%c |",ch);
  }
  for (int i=0; i<(int)change.names.size(); i++) {
    string name = change.names[i];
    if (activeColumn[name]) {
      bool shown = false;
      bool transition = false; //showForDesign[name]&&showForSelect[name];
      //if (change.cond.find(name)!=change.cond.end() && 
      //  showForSelect[name] && select) {
      bool select = check(showForSelect,name);
      bool cond = check(showForCond,name);
      bool view = check(showForDescribe,name);
      if (!factored) {
	fprintf(out,"%s%s%s%s",
		name.c_str(),
		select?"=":"",
		(view&&!(cond||select))?((ch=='+')?":->":":*->"):"",
		(cond&&!(view||select))?":":"");
      } else {
	if (assign&&(!cond)) {
	  fprintf(out,"*->");
	}
      }
      if (showForCond[name] && select) {
	fprintf(out,"%s",celly(change.cond.find(name)->second).c_str());
	transition = true;
	shown = true;
      }
      if (showForDescribe[name] && update) {
	fprintf(out,"%s%s",
		transition?"->":"",
		celly(change.val.find(name)->second).c_str());
	if (shown) ok = false; // collision
	shown = true;
      }
      if (!shown) {
	fprintf(out,"*");
      }
      fprintf(out,"|");
    }
  }
  fprintf(out,"\n");
  return ok;
}

bool MergeOutputTdiff::changeRow(const RowChange& change,
				 bool factored,
				 bool caching) {
  showSheet();
  vector<string> lops;
  activeColumn.clear();
  prevSelect = showForSelect;
  prevDescribe = showForDescribe;
  prevCond = showForCond;
  showForSelect.clear();
  showForDescribe.clear();
  showForCond.clear();
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
    bool shouldCond = condActive;
    bool shouldMatch = condActive && change.indexes.find(name)->second;
    bool shouldAssign = valueActive;
    if (shouldAssign) {
      // conservative choice, should be optional
      if (change.cond.find(name)!=change.cond.end()) {
	shouldMatch = true;
      }
    }

    if (change.mode==ROW_CHANGE_INSERT) {
      // we do not care about matching
      shouldMatch = false; //revSelect[name];
    }
    if (change.mode==ROW_CHANGE_DELETE) {
      // we do not care about assigning
      shouldAssign = false; //prevDescribe[name];
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
    showForSelect[name] = shouldMatch;
    showForDescribe[name] = shouldAssign;
    showForCond[name] = shouldCond;
  }
  if (caching) {
    // state 0 = no factoring of header
    // state 1 = factoring of header
    float costFactored = 1;
    float costUnfactored = 1.9;
    dbg_printf("local ops %s\n", vector2string(lops).c_str());
    float costSwitch = 0.25;
    if (lops!=ops) {
      //if (ops.size()>0) {
      costFactored += 1.1;
      //}
      ops = lops;
      costSwitch = 0;
    }
    //printf("factored %g unfactored %g\n", costFactored, costUnfactored);
    formLattice.beginTransitions();
    formLattice.addTransition(0,0,costUnfactored);
    formLattice.addTransition(1,0,costUnfactored+costSwitch);
    formLattice.addTransition(0,1,costFactored+costSwitch);
    formLattice.addTransition(1,1,costFactored);
    formLattice.endTransitions();
    rowCache.push_back(change);
    return true;
  }

  dbg_printf("round 2 - local ops %s (%d)\n", vector2string(lops).c_str(), factored);
  if (factored) {
    if (lops!=ops) {
      ops = lops;
      operateRow(change,"act");
    }
  } else {
    ops = lops;
  }
  lastWasFactored = factored;
  switch (change.mode) {
  case ROW_CHANGE_INSERT:
    updateRow(change,"insert",false,true,false,factored);
    break;
  case ROW_CHANGE_DELETE:
    updateRow(change,"delete",true,false,false,factored);
    break;
  case ROW_CHANGE_CONTEXT:
    updateRow(change,"after",true,false,false,factored);
    break;
  case ROW_CHANGE_MOVE:
    updateRow(change,"move",true,true,false,factored);
    break;
  case ROW_CHANGE_UPDATE:
    updateRow(change,"update",true,true,false,factored);
    break;
  default:
    fprintf(stderr,"  Unknown row operation\n\n");
    exit(1);
    break;
  }
  return true;
}


bool MergeOutputTdiff::changeName(const NameChange& change) {
  flushRows();
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
      showSheet();
      //fprintf(out, "/* %s %s ","column","name");
      //result.addField(ROW_COL,false);
      fprintf(out,"@@ |");
      for (int i=0; i<(int)names.size(); i++) {
	fprintf(out,"%s|",names[i].c_str());
      }
      fprintf(out,"\n");
      showedColumns = true;
    }
  }
  columns = names;
  return true;
}


bool MergeOutputTdiff::setSheet(const char *name) {
  sheetNameShown = false;
  sheetName = name;
  flushRows();
  return true;
}


void MergeOutputTdiff::flushRows() {
  ops.clear();
  //opsLoose.clear();
  lastWasFactored = false;
  //nops.clear();
  activeColumn.clear();
  showForSelect.clear();
  showForDescribe.clear();
  prevSelect.clear();
  prevDescribe.clear();
  columns.clear();
  constantColumns = true;
  showedColumns = false;
  if (rowCache.size()==0) return;
  /*for (int i=0; i<(int)rowCache.size(); i++) {
    RowChange& change = rowCache[i];
    changeRow(change,false,true);
  }*/
  if (coopy_is_verbose()) {
    formLattice.showPath();
  }
  for (int i=0; i<(int)rowCache.size(); i++) {
    RowChange& change = rowCache[i];
    changeRow(change,(formLattice(i)==1),false);
  }
  formLattice.reset();
  rowCache.clear();
}

