#include <coopy/SheetPatcher.h>
#include <coopy/NameSniffer.h>

#include <map>
#include <algorithm>

#include <stdio.h>

#define WANT_MAP2STRING
#define WANT_VECTOR2STRING
#include <coopy/Stringer.h>

using namespace std;
using namespace coopy::cmp;
using namespace coopy::store;

#define FULL_COLOR (65535)
#define HALF_COLOR (65535/2)

#define FULL_DEBUG

static bool null_like(const SheetCell& a) {
  return a.escaped||a.text==""||a.text=="NULL";
}

static bool is_match(const SheetCell& a, const SheetCell& b) {
  return a==b;
  //if (a==b) return true;
  //if (!null_like(a)) return false;
  //return null_like(b);
}

void SheetPatcher::checkHeader() {
  if (checkedHeader) return;
  checkedHeader = true;
  if (flags.assume_header==false) return;
  PolySheet sheet = getSheet();
  if (!sheet.isValid()) return;
  int c = 0;
  if (activeCol.width() == sheet.width()) {
    for (c=0; c<sheet.width(); c++) {
      if (sheet.cellString(c,0) != activeCol.cellString(c,0)) {
	break;
      }
    }
  }
  if (c==sheet.width()) {
    if (!sheet.hasExternalColumnNames()) {
      dbg_printf("HEADER LINE NUDGE\n");
      //sheet.mustHaveSchema();
      rowCursor = 1;
    }
  }
}

int SheetPatcher::matchRow(const std::vector<int>& active_cond,
			   const std::vector<std::string>& active_name,
			   const std::vector<SheetCell>& cond,
			   int width, bool show) {
  PolySheet sheet = getSheet();
  if (!sheet.isValid()) return false;
  {
    int c;
    for (c=0; c<width; c++) {
      if (active_cond[c]) {
	if (cond[c].text != active_name[c]) {
	  break;
	}
      }
    }
    if (c==width) {
      //printf("WORKING WITH: %s // %s\n", sheet.toString().c_str(),
      //vector2string(sheet.getNestedDescription()).c_str());
      //if (sheet.hasRowOffset()||sheet.hasExternalColumnNames()) {
      if (sheet.getSchema()&&!sheet.hasExternalColumnNames()) {
	dbg_printf("HEADER LINE MATCH\n");
	return -2;
      }
    }
  }

  int r = -1;
  int bct = 0;
  int rbest = -1;
  for (r=0; r<sheet.height(); r++) {
    int ct = 0;
    if (activeRow.cellString(0,r)!="---") {
      bool match = true;
      for (int c=0; c<width; c++) {
	if (active_cond[c]) {
	  if (!is_match(sheet.cellSummary(c,r),cond[c])) {
	    match = false;
	    if (!show) {
	      break;
	    }
	  } else {
	    ct++;
	    if (ct>bct) {
	      bct = ct;
	      rbest = r;
	    }
	  }
	}
      }
      if (match) {
	dbg_printf("Found row %d\n", r);
	return r;
      }
    }
  }
  if (!show) {
    dbg_printf("No match for update\n");
    matchRow(active_cond,active_name,cond,width,true);
  }
  if (show) {
    fprintf(stderr,"# No match for update");
    if (sheet.getSchema()) {
      fprintf(stderr," in %s",sheet.getSchema()->getSheetName().c_str()); 
    }
    if (rbest>=0) {
      fprintf(stderr," - closest was:\n"); 
      for (int c=0; c<width; c++) {
	if (active_cond[c]) {
	  fprintf(stderr,"#   '%s' <-> '%s' %s\n",
		  sheet.cellSummary(c,rbest).text.c_str(),
		  cond[c].text.c_str(),
		  (sheet.cellSummary(c,rbest)!=cond[c])?"FAIL":"OK");
	}
      }
    } else {
      fprintf(stderr,"\n");
    }
  }
  return -1;
}

int SheetPatcher::matchCol(const std::string& mover) {
  string imover = mover;
  if (syn2name.find(mover)!=syn2name.end()) {
    imover = syn2name[mover];
  }
  for (int i=0; i<activeCol.width(); i++) {
    //printf("Checking %s against %s\n", activeCol.cellString(i,0).c_str(),
    //mover.c_str());
    if (activeCol.cellString(i,0)==imover) {
      if (statusCol.cellString(i,0)!="---") {
	//printf("Match at %d (%s)\n", i, mover.c_str());
	return i;
      }
    }
  }
  fprintf(stderr,"column not found: %s\n", mover.c_str());
  //PolySheet sheet = getSheet();
  exit(1);
  return -1;
}

int SheetPatcher::updateCols() {
  PolySheet sheet = getSheet();
  /*
  {
    printf("<activeCol>");
    for (int i=0; i<activeCol.width(); i++) {
      string name = activeCol.cellString(i,0);
      printf(" %s", name.c_str());
    }
    printf("\n");
    SheetSchema *ss = sheet.getSchema();
    if (ss!=NULL) {
      printf("<schema>   ");
      for (int i=0; i<ss->getColumnCount(); i++) {
	string name = ss->getColumnInfo(i).getName();
	printf(" %s", name.c_str());
      }
      printf("\n");
      for (int i=0; i<ss->getColumnCount(); i++) {
	string name = ss->getColumnInfo(i).getName();
	if (i<activeCol.width()) {
	  string name2 = activeCol.cellString(i,0);
	  if (name!=name2) {
	    printf("    %s <---> %s\n", name.c_str(), name2.c_str());
	    exit(1);
	    break;
	  }
	}
      }
    }
    printf("<sheet>    ");
    for (int i=0; i<sheet.width(); i++) {
      string name = sheet.cellString(i,0);
      printf(" %s", name.c_str());
    }
    printf("\n");
  }
  */
  if (activeCol.width()!=sheet.width()) {
    printf("* Please report bug: activeCol drift, %s %d\n",
	   __FILE__,
	   __LINE__);
    exit(1);
  }
  name2col.clear();
  for (int i=0; i<activeCol.width(); i++) {
    string name = activeCol.cellString(i,0);
    //printf("NAME %d %s\n", i, name.c_str());
    if (name!="---") {
      name2col[name] = i;
      col2name[i] = name;
      if (name2syn.find(name)!=name2syn.end()) {
	name2col[name2syn[name]] = i;
      }
    }
  }
  return 0;
}

bool SheetPatcher::renameColumn(int idx, const std::string& name,
				const std::string& oldName) {
  PolySheet sheet = getSheet();
  dbg_printf("Should rename column %d to %s\n", idx, name.c_str());
  bool ok = sheet.modifyColumn(ColumnRef(idx),ColumnInfo(name));
  activeCol.cellString(idx,0,name);
  statusCol.cellString(idx,0,
		       statusCol.cellString(idx,0) +
		       string("(") + oldName + ")");
  if (!ok) {
    fprintf(stderr,"Failed to rename column to %s\n", name.c_str());
  }
  updateCols();
  return true;
}

bool SheetPatcher::moveColumn(int idx, int idx2) {
  //printf("MOVE %d to %d\n", idx, idx2);
  PolySheet sheet = getSheet();
  ColumnRef from(idx);
  ColumnRef to(idx2);
  bool ok = sheet.moveColumn(from,to).isValid();
  activeCol.moveColumn(from,to);
  ColumnRef at = statusCol.moveColumn(from,to);
  if (descriptive) {
    /*
      int first = idx;
      int final = at.getIndex();
      int xfinal = final;
      int sgn = 1;
      string name = "";
      string ch = ">";
      if (final<first) {
      first = final;
      final = idx;
      sgn = -1;
      ch = "<";
      }
      for (int i=first; i<final; i++) {
      if (statusCol.cellString(i,0)!="---") {
      name += ch;
      }
      }
    */
    for (int k=0; k<(int)activeCol.width(); k++) {
      string name = activeCol.cellString(k,0);
      string code = statusCol.cellString(k,0);
      char ch = code[0];
      if (ch=='-'||ch=='+') continue;
      map<string,int>::iterator it = 
	original_index.find(name);
      if (it == original_index.end()) {
	continue;
      }
      int first = k;
      int last = it->second;
      ch = '<';
      string txt = "";
      int sgn = 1;
      if (last<first) {
	sgn = -1;
	ch = '>';
      }
      for (int i=first; i!=last; i += sgn) {
	txt += ch;
      }
      statusCol.cellString(k,0,txt);
    }
  }
  updateCols();
  return ok;
}

bool SheetPatcher::changeColumn(const OrderChange& change) {
  sheetUpdateNeeded = true;
  changeCount++;
  if (chain) chain->changeColumn(change);

  dbg_printf("\n======================\nChange column...\n");

  PolySheet sheet = getSheet();
  if (!sheet.isValid()) {
    fprintf(stderr,"No sheet available to patch\n");
    return false;
  }

#ifdef FULL_DEBUG
  dbg_printf("STATE<<EOF\n%sEOF\n",sheet.toString().c_str());
#endif

  switch (change.mode) {
  case ORDER_CHANGE_DELETE:
    //return sheet->deleteColumn(ColumnRef(change.subject));
    {
      string mover = change.namesBefore[change.identityToIndex(change.subject)];
      //printf("Deleting %s\n", mover.c_str());
      int idx = matchCol(mover);
      bool ok = true;
      if (!descriptive) {
	ColumnRef col(idx); //change.identityToIndex(change.subject));
	ok = sheet.deleteColumn(col);
	activeCol.deleteColumn(col);
	statusCol.deleteColumn(col);
	if (sheet.width()==0) {
	  sheet.deleteData();
	  rowCursor = -1;
	}
      } else {
	statusCol.cellString(idx,0,"---");
	if (descriptive) {
	  Poly<Appearance> appear = sheet.getColAppearance(idx);
	  if (appear.isValid()) {
	    appear->begin();
	    appear->setBackgroundRgb16(FULL_COLOR,
				       HALF_COLOR,
				       HALF_COLOR,
				       AppearanceRange::full());
	    appear->setStrikethrough(true,AppearanceRange::full());
	    appear->end();
	  }
	}
	bool gone = true;
	for (int i=0; i<(int)statusCol.width(); i++) {
	  if (statusCol.cellString(i,0)=="") {
	    gone = false;
	    break;
	  }
	}
	if (gone) {
	  killNeutral = true;
	}
      }
      updateCols();
      return ok;
    }
    break;
  case ORDER_CHANGE_INSERT:
    {
      int toSheet = change.identityToIndexAfter(change.subject);
      string mover = change.namesAfter[toSheet];
      string before = "";
      int idx = -1;
      if (toSheet!=change.indicesAfter.size()-1) {
	before = change.namesAfter[toSheet+1];
	idx = matchCol(before);
      }
      ColumnInfo ci(mover);
      dbg_printf("Inserting column [%s]\n", mover.c_str());
      bool ok = sheet.insertColumn(ColumnRef(idx),ci).isValid();
      ColumnRef at = activeCol.insertColumn(ColumnRef(idx),ci);
      statusCol.insertColumn(ColumnRef(idx),ci);
      activeCol.cellString(at.getIndex(),0,mover);
      statusCol.cellString(at.getIndex(),0,"+++");
      if (descriptive) {
	Poly<Appearance> appear = sheet.getColAppearance(at.getIndex());
	if (appear.isValid()) {
	  appear->begin();
	  appear->setBackgroundRgb16(HALF_COLOR,
				     FULL_COLOR,
				     HALF_COLOR,
				     AppearanceRange::full());
	  appear->end();
	}
      }
      updateCols();
      return ok;
    }
    break;
  case ORDER_CHANGE_MOVE:
    //return sheet->moveColumn(ColumnRef(change.subject),
    //ColumnRef(change.object)
    //).isValid();
    {
      int toSheet = change.identityToIndexAfter(change.subject);
      string mover = change.namesAfter[toSheet];
      int idx = matchCol(mover);
      string before = "";
      int idx2 = -1;
      //printf("after %d for %d\n", toSheet, change.subject);
      if (toSheet!=change.indicesAfter.size()-1) {
	before = change.namesAfter[toSheet+1];
	//printf("Before %s\n", before.c_str());
	idx2 = matchCol(before);
      }
      //printf("Moving %s %d->%d\n", mover.c_str(), idx, idx2);
      bool ok = moveColumn(idx,idx2);
      return ok;
    }
    break;
  case ORDER_CHANGE_RENAME:
    {
      int fromSheet = change.identityToIndex(change.subject);
      int toSheet = change.identityToIndexAfter(change.subject);
      string target = change.namesBefore[fromSheet];
      string newName = change.namesAfter[toSheet];
      int idx = matchCol(target);
      return renameColumn(idx,newName,target);
    }
    break;
  default:
    fprintf(stderr,"* ERROR: Unknown column operation\n");
    break;
  }
  return false;
}

static string colorEncode(const SheetCell& c) {
  std::string str = c.text;
  if (!c.escaped) {
    int score = 0;
    for (score=0; score<(int)str.length(); score++) {
      if (str[score]!='_') {
	break;
      }
    }
    if (str.substr(score,str.length())=="NULL") {
      str = std::string("_") + str;
    }
    return str;
  }
  return "NULL";
}

bool SheetPatcher::markChanges(const RowChange& change, int r,int width,
			       std::vector<int>& active_val,
			       std::vector<SheetCell>& val,
			       std::vector<SheetCell>& cval,
			       std::vector<SheetCell>& pval) {
  PolySheet sheet = getSheet();


  // Check if there is at least one change that is not just an
  // addition

  bool have_mod = false;
  bool have_add = false;
  for (int c=0; c<width; c++) {
    if (active_val[c]) {
      string key = statusCol.cellString(c,0);
      if (key!="+++"&&key!="---") {
	have_mod = true;
      }
      if (key=="+++") {
	have_add = true;
      }
    }
  }
  string row_key = sheet.cellString(0,r);

  string separator = "";
  string conflict_separator = "";
  for (int c=0; c<width; c++) {
    if (active_val[c]) {
      string key = statusCol.cellString(c,0);
      bool this_mod = (key!="+++");
      if (descriptive) {
	if (have_mod) {
	  if (separator=="") {
	    separator = "->";
	    bool more = true;
	    while (more) {
	      more = false;
	      for (int i=0; i<width; i++) {
		SheetCell prev = sheet.cellSummary(i,r);
		if (prev.text.find(separator)!=string::npos) {
		  separator = string("-") + separator;
		  more = true;
		  break;
		}
	      }
	    }
	  }
	} else if (have_add) {
	  separator = "+";
	}
      }
      string init = separator;
      if (change.conflicted) {
	if (conflict_separator=="") {
	  conflict_separator = string("!");
	  bool more = true;
	  // this is actually far too conservative
	  while (more) {
	    more = false;
	    for (int i=0; i<width; i++) {
	      SheetCell prev = sheet.cellSummary(i,r);
	      if (prev.text.find(conflict_separator)!=string::npos) {
		conflict_separator = string("-") + conflict_separator;
		more = true;
		break;
	      }
	    }
	  }
	}
	init = conflict_separator + separator;
      }
      if (init.length()>activeRow.cellString(0,r).length()) {
	activeRow.cellString(0,r,init);
      }
      if (change.conflicted) {
	if (descriptive) {
	  Poly<Appearance> appear = sheet.getCellAppearance(0,r);
	  if (appear.isValid()) {
	    appear->begin();
	    appear->setBackgroundRgb16(FULL_COLOR,
				       0,
				       0,
				       AppearanceRange::full());
	    appear->end();
	  }
	}
      }
      if (descriptive) {
	SheetCell prev = sheet.cellSummary(c,r);
	string from = prev.toString();
	if (prev.escaped) from = "";
	//string to = val[c].toString();
	SheetCell to = val[c];
	bool conflicted = false;
	if (val[c]!=cval[c]) {
	  to = cval[c];
	  conflicted = true;
	}
	SheetStyle style;
	if (this_mod||conflicted) {
	  if (conflicted&&!pval[c].escaped) {
	    sheet.cellString(c,r,
			     colorEncode(pval[c]) + 
			     (conflicted?init:separator) + 
			     colorEncode(prev) + 
			     (conflicted?init:separator) + 
			     colorEncode(to));
	  } else {
	    sheet.cellString(c,r,colorEncode(prev) + 
			     (conflicted?init:separator) + 
			     colorEncode(to));
	  }
	} else {
	  /*
	  if (row_key=="---"&&key=="+++") {
	    // no point adding anything here
	  } else if (row_key=="+++"&&key=="---") {
	    // no point adding anything here
	  } else {
	  */
	  sheet.cellSummary(c,r,to);
	}
	if (this_mod||conflicted) {
	  Poly<Appearance> appear = sheet.getCellAppearance(c,r);
	  if (appear.isValid()) {
	    appear->begin();
	    if (conflicted) {
	      appear->setBackgroundRgb16(FULL_COLOR,
					 0,
					 0,
					 AppearanceRange::full());
	    } else {
	      appear->setBackgroundRgb16(HALF_COLOR,
					 HALF_COLOR,
					 FULL_COLOR,
					 AppearanceRange::full());
	    }
	    appear->setWeightBold(true,AppearanceRange::full());
	    appear->end();
	  }
	}
      } else {
	if (change.conflicted) {
	  sheet.cellSummary(conflictColumn,r,SheetCell("CONFLICT",false));
	  //printf("AT %d %d\n", conflictColumn,r);

	  Poly<Appearance> appear = sheet.getCellAppearance(conflictColumn,r);
	  if (appear.isValid()) {
	    appear->begin();
	    appear->setBackgroundRgb16(HALF_COLOR,
				       HALF_COLOR,
				       FULL_COLOR,
				       AppearanceRange::full());
	    appear->end();
	  }

	  SheetCell from = pval[c];
	  SheetCell to = val[c];
	  SheetCell alt = cval[c];
	  if (to!=alt) {
	    string ctxt = 
	      "((( " + from.toString() + " ))) " +
	      to.toString() + " /// " + alt.toString();
	    sheet.cellSummary(c,r,SheetCell(ctxt,false));
	    Poly<Appearance> appear = sheet.getCellAppearance(c,r);
	    if (appear.isValid()) {
	      appear->begin();
	      appear->setBackgroundRgb16(FULL_COLOR,
					 HALF_COLOR,
					 HALF_COLOR,
					 AppearanceRange::full());
	      appear->end();
	    }
	  }
	} else {
	  sheet.cellSummary(c,r,val[c]);
	}
      }
    }
  }
  return true;
}


class Invention {
public:
  SheetCell origin;
  int offset;
  PoolColumnLink link;
};

bool SheetPatcher::handleConflicts() {
  if (readyForConflicts) return true;
  if (descriptive) return true;
  if (!merging) return false;
  for (int i=0; i<(int)column_names.size(); i++) {
    if (column_names[i]=="_MERGE_") {
      readyForConflicts = true;
      conflictColumn = i;
      return true;
    }
  }
  PolySheet sheet = getSheet();
  if (!sheet.isValid()) return false;
  sheet.mustHaveSchema();

  //printf("SCHEMA %s\n", sheet.getSchema()->toString().c_str());
  //printf("SCHEMA %s\n", active_sheet.getSchema()->toString().c_str());
  //printf("sheet before %s\n", sheet.toString().c_str());
  ColumnInfo con("_MERGE_");
  con.setType("text","sqlite");
  ColumnRef at = active_sheet.insertColumn(ColumnRef(-1),con);
  activeCol.insertColumn(ColumnRef(-1),con);
  statusCol.insertColumn(ColumnRef(-1),con);
  readyForConflicts = at.isValid();
  conflictColumn = at.getIndex();

  //printf("sheet after %s\n", sheet.toString().c_str());
  //printf("SCHEMA AFTER %s\n", active_sheet.getSchema()->toString().c_str());

  return readyForConflicts;
}

bool SheetPatcher::changeRow(const RowChange& change) {
  sheetUpdateNeeded = true;

  PolySheet sheet = getSheet();
  if (!sheet.isValid()) {
    fprintf(stderr,"No sheet available to patch.\n");
    return false;
  }

  if (!declaredNames) {
    if (chain) chain->declareNames(change.allNames,false);
    declareNames(change.allNames,false);
  }

  if (change.conflicted) {
    if (!handleConflicts()) {
      fprintf(stderr,"Cannot handle conflicts.\n");
      return false;
    }
  }

  if (activeRow.height()!=sheet.height() || activeRow.width()!=1) {
    activeRow.resize(1,sheet.height());
  }

  dbg_printf("\n======================\nRow cursor in: %d\n", rowCursor);
  if (coopy_is_verbose()) {
    RowChange c;
    c = change;
    c.show();
  }

  if (!change.sequential) rowCursor = -1;
  //map<string,int> dir;
  vector<int> active_cond;
  vector<string> active_name;
  int active_conds = 0;
  vector<SheetCell> cond;
  vector<int> active_val;
  vector<SheetCell> val;
  vector<SheetCell> cval;
  vector<SheetCell> pval;
  vector<string> allNames = change.allNames;
  int width = sheet.width(); //(int)change.allNames.size();
  /*
  if (width==0) {
    if (column_names.size()==0) {
      NameSniffer sniffer(sheet);
      column_names = sniffer.suggestNames();
    }
    allNames = column_names;
    width = (int)allNames.size();
  }
  */
  for (int i=0; i<width; i++) {
    //dir[allNames[i]] = i;
    active_cond.push_back(0);
    active_name.push_back("");
    cond.push_back(SheetCell());
    active_val.push_back(0);
    val.push_back(SheetCell());
    cval.push_back(SheetCell());
    pval.push_back(SheetCell());
  }
  for (RowChange::txt2cell::const_iterator it = change.cond.begin();
       it!=change.cond.end(); it++) {
    if (name2col.find(it->first)!=name2col.end()) {
      int idx = name2col[it->first]; //dir[it->first];
      //printf("  [cond] %d %s -> %s\n", idx, it->first.c_str(), it->second.toString().c_str());
      active_cond[idx] = 1;
      active_conds++;
      cond[idx] = it->second;
      active_name[idx] = it->first;
    }
  }
  for (RowChange::txt2cell::const_iterator it = change.val.begin();
       it!=change.val.end(); it++) {
    if (name2col.find(it->first)!=name2col.end()) {
      int idx = name2col[it->first]; //dir[it->first];
      //printf("  [val] %d %s -> %s\n", idx, it->first.c_str(), it->second.toString().c_str());
      active_val[idx] = 1;
      val[idx] = it->second;
      cval[idx] = it->second;
      pval[idx] = it->second;
    } else {
      if (std::find(change.names.begin(),change.names.end(),it->first)!=
	  change.names.end()) {
	fprintf(stderr,"Unknown column %s\n", it->first.c_str());
      }
    }
  }
  for (RowChange::txt2cell::const_iterator it = change.conflictingVal.begin();
       it!=change.conflictingVal.end(); it++) {
    if (name2col.find(it->first)!=name2col.end()) {
      int idx = name2col[it->first];
      cval[idx] = it->second;
    }
  }
  for (RowChange::txt2cell::const_iterator it = change.conflictingParentVal.begin();
       it!=change.conflictingParentVal.end(); it++) {
    if (name2col.find(it->first)!=name2col.end()) {
      int idx = name2col[it->first];
      pval[idx] = it->second;
    }
  }


  bool result = false;
  bool defer = false;
  
  switch (change.mode) {
  case ROW_CHANGE_INSERT:
    {
      RowRef tail(rowCursor);
      Poly<SheetRow> inserter;
      if (sheet.isSequential()) {
	inserter = sheet.insertRowOrdered(tail);
      } else {
	inserter = sheet.insertRow();
      }
      vector<Invention> inventions;
      for (int c=0; c<width; c++) {
	if (active_val[c]) {
	  PoolColumnLink link;
	  if (flags.foreign_pool) {
	    std::map<std::string,coopy::store::PoolColumnLink>::iterator it =
	      name2pool.find(col2name[c]);
	    if (it!=name2pool.end()) {
	      link = it->second;
	    }
	  }
	  if (link.isValid()&&!descriptive) {
	    // Skip if inventor
	    // But should try to recover id to place in pool
	    
	    if (!link.isInventor()) {
	      bool found = false;
	      PoolRecord v = link.getColumn().lookup(val[c],found);
	      if (found) {
		if (!v.linked) found = false;
	      } else {
		if (flags.pool) {
		  if (flags.pool->isScanned()) {
		    // if prescanned, then unmarked records can pass through
		    found = true;
		    v.cell = val[c];
		  }
		}
	      }
	      if (!found) {
		/*
		fprintf(stderr, "Problem translating a local value to a remote value. Diagnostics:\n");
		fprintf(stderr, "  * Table: %s\n", link.getTableName().c_str());
		fprintf(stderr, "  * Column: %s\n", link.getColumnName().c_str());
		fprintf(stderr, "  * Value: %s\n", val[c].toString().c_str());
		fprintf(stderr, "This value is not (yet) present in the '%s' pool. Solutions:\n", link.getPoolName().c_str());
		fprintf(stderr, "  * Use --table A --table B ... to control order in which tables are updated.\n");
		fprintf(stderr, "    Try to do more fundamental inserts first, then actions that reference them\n");
		fprintf(stderr, "  * Or rerun with --native flag if no translation is needed.\n");
		//fprintf(stderr, "Exit, fatal error.\n");
		//exit(1);
		*/

		result = false;
		defer = true;
		inserter->undo();
		break;

	      } else {
		inserter->setCell(c,v.cell);
	      }
	    } else {
	      inserter->invent(c);
	      Invention invent = { val[c], c, link };
	      inventions.push_back(invent);
	    }
	  } else {
	    inserter->setCell(c,val[c]);
	  }
	}
      }
      if (defer) {
	break;
      }
      inserter->flush();
      if (inventions.size()>0) {
	int r = inserter->getRowAfterFlush().getIndex();
	for (int i=0; i<(int)inventions.size(); i++) {
	  Invention& inv = inventions[i];
	  SheetCell dest = sheet.cellSummary(inv.offset,r);
	  inv.link.getColumn().put(inv.origin,dest);
	  dbg_printf("Link %s -> %s\n", inv.origin.toString().c_str(),
		     dest.toString().c_str());
	}
      }
      if (sheet.isSequential()) {
	int r = inserter->getRowAfterFlush().getIndex();
	activeRow.insertRowOrdered(tail);
	activeRow.cellString(0,r,"+++");
	if (descriptive) {
	  Poly<Appearance> appear = sheet.getRowAppearance(r);
	  if (appear.isValid()) {
	    appear->begin();
	    appear->setBackgroundRgb16(HALF_COLOR,
				       FULL_COLOR,
				       HALF_COLOR,
				       AppearanceRange::full());
	    appear->setWeightBold(true,AppearanceRange::full());
	    appear->end();
	  }
	}
	r++;
	if (r>=sheet.height()) {
	  r = -1;
	}
	rowCursor = r;
      }
    }
    break;
  case ROW_CHANGE_DELETE:
    {
      int r = matchRow(active_cond,active_name,cond,width);
      if (r<0) {
	result = false;
	break;
      }
      RowRef row(r);
      rowCursor = r;
      if (!descriptive) {
	sheet.deleteRow(row);
	activeRow.deleteRow(row);
      } else {
	Poly<Appearance> appear = sheet.getRowAppearance(r);
	if (appear.isValid()) {
	  appear->begin();
	  appear->setBackgroundRgb16(FULL_COLOR,
				     HALF_COLOR,
				     HALF_COLOR,
				     AppearanceRange::full());
	  //appear->setWeightBold(true,AppearanceRange::full());
	  appear->setStrikethrough(true,AppearanceRange::full());
	  appear->end();
	}
	activeRow.cellString(0,r,"---");
      }
      if (rowCursor>=sheet.height()) {
	rowCursor = -1;
      }
      result = true;
    }
    break;
  case ROW_CHANGE_CONTEXT:
    {
      if (active_conds>0) {
	int r = matchRow(active_cond,active_name,cond,width);
	if (r!=-2) {
	  if (r<0) {
	    result = false;
	    break;
	  }
	  r++;
	} else {
	  r = 0;
	}
	if (r>=sheet.height()) {
	  r = -1;
	}
	RowRef row(r);
	rowCursor = r;
      } else {
	rowCursor = 0;
	checkHeader();
      }
      result = true;
    }
    break;
  case ROW_CHANGE_MOVE:
    {
      bool success = false;
      int r = matchRow(active_cond,active_name,cond,width);
      if (r<0) {
	result = false;
	break;
      }
      RowRef from(r);
      RowRef to(rowCursor);
      dbg_printf("Moving %d to %d in sheet of length %d\n", from.getIndex(), to.getIndex(), sheet.height());
      RowRef result = sheet.moveRow(from,to);
      if (result.getIndex() == -1) {
	fprintf(stderr,"Row move failed in sheet of type %s\n",
		sheet.desc().c_str());
      } else {
	activeRow.moveRow(from,to);
      }
      r = result.getIndex();
      dbg_printf("Move result was %d\n", r);
      for (int y=0; y<sheet.height(); y++) {
	dbg_printf("%d %s / ", y, sheet.cellString(0,y).c_str());
      }
      dbg_printf("\n");
      markChanges(change,r,width,active_val,val,cval,pval);
      r++;
      if (r>=sheet.height()) {
	r = -1;
      }
      rowCursor = r;
      result = true;
    }
    break;
  case ROW_CHANGE_UPDATE:
    {
      bool success = false;
      int r = matchRow(active_cond,active_name,cond,width);
      if (r<0) {
	rowCursor = -1;
	result = false;
	break;
      }
      dbg_printf("Match for assignment\n");
      markChanges(change,r,width,active_val,val,cval,pval);
      r++;
      if (r>=sheet.height()) {
	r = -1;
      }
      rowCursor = r;
      dbg_printf("Cursor moved to %d\n", r);
      result = true;
    }
    break;
  default:
    fprintf(stderr,"* ERROR: unsupported row operation\n");
    result = false;
    break;
  }

  if (defer) {
    deferred_rows.push_back(RowUnit(sheetName,change));
    dbg_printf("DEFERRED a row\n");
  }

  if (result) {
    changeCount++;
    if (chain) chain->changeRow(change);
  }

  return result;
}

bool SheetPatcher::declareNames(const std::vector<std::string>& names, 
				bool final) {
  bool first = !declaredNames;
  declaredNames = true;
  PolySheet sheet = getSheet();
  if (!sheet.isValid()) return false;
  if (chain) chain->declareNames(names,final);
  if (config.trustNames==false) {
    if (!descriptive) {
      if ((int)names.size()!=sheet.width()) {
	//fprintf(stderr,"* ERROR: name mismatch\n");
	return false;
      }
    }
    if (first) {
      for (int i=0; i<(int)names.size(); i++) {
	if (names[i]!=activeCol.cellString(i,0)) {
	  //if (names[i][0]=='[') {
	  syn2name[names[i]] = activeCol.cellString(i,0);
	  name2syn[activeCol.cellString(i,0)] = names[i];
	  //}
	}
      }
      updateCols();
    } else {
      if (!descriptive) {
	for (int i=0; i<(int)names.size(); i++) {
	  if (names[i]!=activeCol.cellString(i,0)) {
	    if (names[i][0]!='[') {
	      int idx = matchCol(names[i]);
	      int idx1 = matchCol(activeCol.cellString(i,0));
	      moveColumn(idx,idx1);
	    }
	  }
	}
	updateCols();
      }
    }
  } else {
    for (int i=0; i<(int)names.size(); i++) {
      printf("Checking %s\n", names[i].c_str());
    }
    fprintf(stderr,"Named columns not implemented yet\n");
    exit(1);
  }
  original_index.clear();
  for (int i=0; i<(int)names.size(); i++) {
    original_index[names[i]] = i;
  }
  return true;
}

bool SheetPatcher::setSheet(const char *name) {
  checkedHeader = false;
  updateSheet();
  sheetUpdateNeeded = true;

  if (chain) chain->setSheet(name);

  TextBook *book = getBook();
  if (book==NULL) return false;

  //reset
  config = ConfigChange();
  columns.clear();
  column_names.clear();
  rowCursor = -1;
  declaredNames = false;

  // load
  PolySheet psheet;
  attachSheet(psheet);
  psheet = book->readSheet(name);
  if (!psheet.isValid()) {
    if (!book->namedSheets()) {
      psheet = book->readSheetByIndex(0);
    }
  }
  if (!psheet.isValid()) {
    if (string(name)=="sheet") { // default name
      if (book->namedSheets()) {
	if (book->getNames().size() == 1) {
	  psheet = book->readSheetByIndex(0);
	}
      }
    }
  }
  if (!psheet.isValid()) {
    fprintf(stderr,"Cannot find sheet %s, oh dear\n", name);
    return false;
  }
  dbg_printf("Moved to sheet %s\n", name);
  attachSheet(psheet);
  //sheet = &psheet;

  if (sheetName!=name) {
    sheetName = name;
    sheetChange = true;
  }

  setNames();

  addPoolsFromFlags(psheet);

  return true;
}


bool SheetPatcher::mergeStart() {
  sheetChange = true;
  sheetName = "";
  killNeutral = false;
  activeRow.resize(1,0);
  setNames();
  if (chain) chain->mergeStart();
  return true;
}

static string clean(const string& s) {
  return (s!="NULL")?s:"";
}

bool SheetPatcher::mergeAllDone() {
  dbg_printf("SheetPatcher::mergeAllDone\n");
  if (chain) chain->mergeAllDone();
  updateSheet();
  return true;
}

bool SheetPatcher::mergeDone() {
  int len = -1;
  int prev_len = -2;
  do {
    prev_len = len;
    len = deferred_rows.size();
    if (len>0) {
      dbg_printf("Dealing with deferred rows\n");
      std::list<RowUnit> cp = deferred_rows;
      deferred_rows.clear();
      for (std::list<RowUnit>::iterator it = cp.begin();
	   it != cp.end(); it++) {
	RowUnit& unit = *it;
	if (unit.sheet_name!=sheetName) {
	  setSheet(unit.sheet_name.c_str());
	}
	changeRow(unit.change);
      }
    }
  } while (len>0 && len!=prev_len);
  if (len>0) {
    fprintf(stderr,"Cyclic dependency, cannot apply deferred rows\n");
    exit(1);
  }

  if (chain) chain->mergeDone();
  return true;
}


void SheetPatcher::setNames() {
  PolySheet sheet = getSheet();
  if (!sheet.isValid()) return;
  if (sheetChange) {
    dbg_printf("SheetPatcher Working on sheet %ld\n",
	       (long int)(&sheet.tail_const()));
    sheetChange = false;
    clearNames();
    activeCol.resize(0,1);
    statusCol.resize(0,1);
    sniffer = new NameSniffer(sheet,getFlags());
    COOPY_ASSERT(sniffer);
    sniffedSheet = &sheet;
    activeCol.resize(sheet.width(),1);
    statusCol.resize(sheet.width(),1);
    for (int i=0; i<sheet.width(); i++) {
      string name = sniffer->suggestColumnName(i);
      activeCol.cellString(i,0,name);
    }
    updateCols();
    updatePool();
  }
}


void SheetPatcher::updatePool() {
  PolySheet sheet = getSheet();
  if (!sheet.isValid()) return;
  setNames();
  if (descriptive) return;
  if (!sniffer) return;
  if (sheetName=="") return;
  name2pool.clear();
  bool addedAuto = false;
  for (int i=0; i<sheet.width(); i++) {
    string name = sniffer->suggestColumnName(i);
    const CompareFlags& flags = getFlags();
    if (!flags.pool) continue;
    //printf("Looking up %s %s\n", sheetName.c_str(), name.c_str());
    PoolColumnLink pc = flags.pool->lookup(sheetName,name);
    if (!pc.isValid()) {
      ColumnType t = sniffer->suggestColumnType(i);
      if (t.autoIncrement&&!addedAuto) {
	//printf("Found autoinc'er\n");
	flags.pool->create(sheetName,sheetName,name,true);
	pc = flags.pool->lookup(sheetName,name);
	addedAuto = true;
      }
      if (t.foreignKey!="") {
	//printf("Found xref\n");
	flags.pool->create(t.foreignTable,sheetName,name,false);
	pc = flags.pool->lookup(sheetName,name);
      }
    }
    if (!pc.isValid()) continue;
    //printf("Found a pool at %s %s\n", sheetName.c_str(), name.c_str());
    name2pool[name] = pc;
    CompareFlags& flags2 = getMutableFlags();
    if (!flags2.foreign_pool_set) {
      flags2.foreign_pool = true;
      flags2.foreign_pool_set = true;
    }
  }
}


bool SheetPatcher::updateSheet() {
  if (!sheetUpdateNeeded) return false;

  if (descriptive) {
    PolySheet sheet = getSheet();
    if (!sheet.isValid()) return false;
    if (sheet.height()==0) return true;
    sheet.insertColumn(ColumnRef(0)); //,ColumnInfo("_ACTION_")); 
    dbg_printf("Added description column\n");
    //sheet.insertRow(RowRef(0));
    for (int i=0; i<sheet.height()&&i<activeRow.height(); i++) {
      dbg_printf("  row %d\n", i);
      string txt = activeRow.cellString(0,i);
      if (txt==""&&killNeutral) {
	txt = "---";
      }
      if (txt!="") {
	sheet.cellString(0,i,txt);
	Poly<Appearance> appear = sheet.getCellAppearance(0,i);
	if (appear.isValid()) {
	  appear->begin();
	  appear->setWeightBold(true,AppearanceRange::full());
	  appear->setStrikethrough(false,AppearanceRange::full());
	  appear->end();
	}
      }
    }
    COOPY_ASSERT(sniffer);
    int r = -1;
    r = sniffer->getHeaderHeight()-1;
    if (r>=0 && r<sheet.height()) {
      string key = clean(sheet.cellString(0,r));
      if (key == "" || key == "->") {
	for (int i=0; i<=r; i++) {
	  sheet.cellString(0,i,string("@@")+clean(sheet.cellString(0,i)));
	}
      } else {
	r = -1;
      }
    }
    int yoff = 0;
    if (r<0) {
      sheet.insertRow(RowRef(0)); 
      sheet.cellString(0,yoff,"@@");
      r = 0;
    } else {
      yoff = r;
    }
    for (int i=1; i<sheet.width(); i++) {
      dbg_printf("Header clobber %s\n", sheet.cellString(i,yoff).c_str());
      sheet.cellString(i,yoff,activeCol.cellString(i-1,0));
    }
    bool colAction = false;
    for (int i=1; i<sheet.width(); i++) {
      if (statusCol.cellString(i-1,0)!="") {
	colAction = true;
	break;
      }
    }
    if (colAction) {
      sheet.insertRow(RowRef(0)); 
      sheet.cellString(0,0,"!");
      for (int i=1; i<sheet.width(); i++) {
	sheet.cellString(i,0,statusCol.cellString(i-1,0));
      }
      yoff++;
    }
    Poly<Appearance> appear = sheet.getRowAppearance(r+yoff);
    if (appear.isValid()) {
      appear->begin();
      appear->setWeightBold(true,AppearanceRange::full());
      appear->end();
    }

    vector<int> show;
    show.resize(sheet.height(),0);
    for (int i=0; i<sheet.height(); i++) {
      bool present = clean(sheet.cellString(0,i))!="";
      if (present) {
	for (int j=-2; j<=2; j++) {
	  int k = i+j;
	  if (k>=0 && k<sheet.height()) {
	    show[k] = 1;
	  }
	}
      }
    }
    int offset = 0;
    bool addedBreak = false;
    int start_delete = -1;
    int count_delete = 0;
    for (int i=0; i<(int)show.size(); i++) {
      dbg_printf("Checking %d...\n", i);
      if (show[i]==0) {	
	int k = i+offset;
	if (addedBreak) {
	  dbg_printf("Deleting row %d\n", k);
	  if (start_delete==-1) {
	    start_delete = k;
	    count_delete = 0;
	  }
	  count_delete++;
	  //sheet.deleteRow(RowRef(k));
	  offset--;
	} else {
	  if (start_delete!=-1) {
	    dbg_printf("Implementing deletion of %d rows starting at %d\n",
		       count_delete,
		       start_delete);
	    
	    sheet.deleteRows(RowRef(start_delete),
			     RowRef(start_delete+count_delete-1));
	    start_delete = -1;
	  }
	  dbg_printf("Adding ... marks\n");
	  sheet.cellString(0,k,"...");
	  for (int j=1; j<sheet.width(); j++) {
	    sheet.cellString(j,k,"...");
	  }
	  dbg_printf("Done adding ... marks\n");
	  addedBreak = true;
	}
      } else {
	addedBreak = false;
      }
    }    
    if (start_delete!=-1) {
      sheet.deleteRows(RowRef(start_delete),
		       RowRef(start_delete+count_delete-1));
      start_delete = -1;
    }
  }

  if (forReview) {
    PolySheet sheet = getSheet();
    if (!sheet.isValid()) return false;
    sheet.insertColumn(ColumnRef(0)); 
    sheet.insertRow(RowRef(0)); 
    sheet.cellString(0,0,"Allow? (yes/no)"); 
  }

  sheetUpdateNeeded = false;

  return true;
}

coopy::store::PolySheet SheetPatcher::getSheet() {
  PolySheet sheet = Patcher::getSheet();
  if (!sheet.isValid()) return sheet;
  if (!active_sheet.isValid()) {
    active_sheet = sheet;
  }
  if (&sheet.tail()!=&active_sheet.tail()) {
    active_sheet = sheet;
  }
  if (!getFlags().assume_header) {
    active_sheet.forbidSchema();
  }
  return active_sheet;
}

bool SheetPatcher::metaHint(const DataSheet& sheet) {
  getSheet();
  if (!active_sheet.isValid()) return false;
  if (!active_sheet.getMeta()) {
    if (sheet.getMeta()) {
      bool done = false;
      if (active_sheet.getSchema()) {
	dbg_printf("*** meta hint - was %s\n", 
	       active_sheet.getSchema()->toString().c_str());
	done = active_sheet.getSchema()->copy(*sheet.getMeta());
      }
      if (!done) {
	active_sheet.setSchema(sheet.getMeta()->clone(),true);
      }
      sheetChange = true;
      dbg_printf("*** meta hint - is now %s\n", 
		 active_sheet.getSchema()->toString().c_str());
      if (sheet.hasRowOffset()&&!active_sheet.hasRowOffset()) {
	if (sheet.height()==active_sheet.height()) {
	  active_sheet.getSchema()->setHeaderHeight(0);
	}
      }
      //printf("offset %d\n", sheet.hasRowOffset());
      ///printf("offset %d\n", active_sheet.hasRowOffset());
      //printf("FROM\n%s\n\nTo\n%s\n\n",
      //sheet.tail_const().toString().c_str(),
      //active_sheet.tail_const().toString().c_str());
      setNames();
    }
  }
  return true;
}



