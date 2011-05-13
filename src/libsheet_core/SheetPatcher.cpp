#include <coopy/SheetPatcher.h>
#include <coopy/NameSniffer.h>

#include <map>
#include <algorithm>

#include <stdio.h>

using namespace std;
using namespace coopy::cmp;
using namespace coopy::store;

#define FULL_COLOR (65535)
#define HALF_COLOR (65535/2)

int SheetPatcher::matchRow(const vector<int>& active_cond,
			   const vector<SheetCell>& cond,
			   int width) {
  PolySheet sheet = getSheet();
  if (!sheet.isValid()) return false;
  int r = -1;
  for (r=0; r<sheet.height(); r++) {
    if (activeRow.cellString(0,r)!="---") {
      bool match = true;
      for (int c=0; c<width; c++) {
	if (active_cond[c]) {
	  if (sheet.cellSummary(c,r)!=cond[c]) {
	    match = false;
	    break;
	  }
	}
      }
      if (match) return r;
    }
  }
  return -1;
}


bool SheetPatcher::changeColumn(const OrderChange& change) {
  changeCount++;
  if (chain) chain->changeColumn(change);

  PolySheet sheet = getSheet();
  if (!sheet.isValid()) {
    fprintf(stderr,"No sheet available to patch\n");
    return false;
  }
  switch (change.mode) {
  case ORDER_CHANGE_DELETE:
    //return sheet->deleteColumn(ColumnRef(change.subject));
    {
      ColumnRef col(change.identityToIndex(change.subject));
      bool ok = sheet.deleteColumn(col);
      activeCol.deleteColumn(col);
      //dbg_printf("Sheet width is %d\n", sheet->width());
      if (sheet.width()==0) {
	sheet.deleteData();
	rowCursor = -1;
      }
      return ok;
    }
    break;
  case ORDER_CHANGE_INSERT:
    //return sheet->insertColumn(ColumnRef(change.subject)).isValid();
    {
      int toSheet = change.identityToIndexAfter(change.subject);
      if (toSheet==change.indicesAfter.size()-1) {
	toSheet = -1;
      } else {
	int toId = change.indicesAfter[toSheet+1];
	toSheet = change.identityToIndex(toId);
      }
      bool ok = sheet.insertColumn(ColumnRef(toSheet)).isValid();
      ColumnRef at = activeCol.insertColumn(ColumnRef(toSheet));
      activeCol.cellString(at.getIndex(),0,"+++");
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
      return ok;
    }
    break;
  case ORDER_CHANGE_MOVE:
    //return sheet->moveColumn(ColumnRef(change.subject),
    //ColumnRef(change.object)
    //).isValid();
    {
      int toSheet = change.identityToIndexAfter(change.subject);
      if (toSheet==change.indicesAfter.size()-1) {
	toSheet = -1;
      } else {
	int toId = change.indicesAfter[toSheet+1];
	toSheet = change.identityToIndex(toId);
      }
      ColumnRef from(change.identityToIndex(change.subject));
      ColumnRef to(toSheet);
      bool ok = sheet.moveColumn(from,to).isValid();
      activeCol.moveColumn(from,to);
      return ok;
    }
    break;
  default:
    fprintf(stderr,"* ERROR: Unknown column operation\n");
    break;
  }
  return false;
}

bool SheetPatcher::changeRow(const RowChange& change) {
  changeCount++;
  if (chain) chain->changeRow(change);

  PolySheet sheet = getSheet();
  if (!sheet.isValid()) {
    fprintf(stderr,"No sheet available to patch\n");
    return false;
  }

  if (activeRow.height()!=sheet.height() || activeRow.width()!=1) {
    activeRow.resize(1,sheet.height());
  }

  dbg_printf("\n======================\nRow cursor in: %d\n", rowCursor);
  if (!change.sequential) rowCursor = -1;
  map<string,int> dir;
  vector<int> active_cond;
  vector<SheetCell> cond;
  vector<int> active_val;
  vector<SheetCell> val;
  vector<string> allNames = change.allNames;
  int width = (int)change.allNames.size();
  if (width==0) {
    if (column_names.size()==0) {
      NameSniffer sniffer(sheet);
      column_names = sniffer.suggestNames();
    }
    allNames = column_names;
    width = (int)allNames.size();
  }
  for (int i=0; i<width; i++) {
    dir[allNames[i]] = i;
    active_cond.push_back(0);
    cond.push_back(SheetCell());
    active_val.push_back(0);
    val.push_back(SheetCell());
  }
  for (RowChange::txt2cell::const_iterator it = change.cond.begin();
       it!=change.cond.end(); it++) {
    int idx = dir[it->first];
    //printf("  [cond] %d %s -> %s\n", idx, it->first.c_str(), it->second.toString().c_str());
    active_cond[idx] = 1;
    cond[idx] = it->second;
  }
  for (RowChange::txt2cell::const_iterator it = change.val.begin();
       it!=change.val.end(); it++) {
    int idx = dir[it->first];
    //printf("  [val] %d %s -> %s\n", idx, it->first.c_str(), it->second.toString().c_str());
    active_val[idx] = 1;
    val[idx] = it->second;
  }
  
  switch (change.mode) {
  case ROW_CHANGE_INSERT:
    {
      if (sheet.isSequential()) {
	RowRef tail(rowCursor);
	int r = sheet.insertRow(tail).getIndex();
	activeRow.insertRow(tail);
	/*
	if (rowCursor!=-1) {
	  rowCursor++;
	  if (rowCursor==sheet->height()) {
	    rowCursor = -1;
	  }
	  }*/
	if (r>=0) {
	  activeRow.cellString(0,r,"+++");
	  for (int c=0; c<width; c++) {
	    if (active_val[c]) {
	      sheet.cellSummary(c,r,val[c]);
	    }
	  }
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
	}
	r++;
	if (r>=sheet.height()) {
	  r = -1;
	}
	rowCursor = r;
      } else {
	Poly<SheetRow> inserter = sheet.insertRow();
	for (int c=0; c<width; c++) {
	  if (active_val[c]) {
	    inserter->setCell(c,val[c]);
	  }
	}
	inserter->flush();
      }
    }
    break;
  case ROW_CHANGE_DELETE:
    {
      int r = matchRow(active_cond,cond,width);
      if (r<0) return false;
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
      return true;
    }
    break;
  case ROW_CHANGE_CONTEXT:
    {
      int r = matchRow(active_cond,cond,width);
      if (r<0) return false;
      r++;
      if (r>=sheet.height()) {
	r = -1;
      }
      RowRef row(r);
      rowCursor = r;
      return true;
    }
    break;
  case ROW_CHANGE_MOVE:
    {
      bool success = false;
      int r = matchRow(active_cond,cond,width);
      if (r<0) return false;
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
      r++;
      if (r>=sheet.height()) {
	r = -1;
      }
      rowCursor = r;
      return true;
    }
    break;
  case ROW_CHANGE_UPDATE:
    {
      bool success = false;
      int r = matchRow(active_cond,cond,width);
      if (r<0) {
	dbg_printf("No match for update\n");
	rowCursor = -1;
	return false;
      }
      dbg_printf("Match for assignment\n");
      activeRow.cellString(0,r,"->");
      for (int c=0; c<width; c++) {
	if (active_val[c]) {
	  if (descriptive) {
	    SheetCell prev = sheet.cellSummary(c,r);
	    string from = prev.toString();
	    if (prev.escaped) from = "";
	    string to = val[c].toString();
	    sheet.cellString(c,r,from + "->" + to);
	    Poly<Appearance> appear = sheet.getCellAppearance(c,r);
	    if (appear.isValid()) {
	      appear->begin();
	      appear->setBackgroundRgb16(HALF_COLOR,
					 HALF_COLOR,
					 FULL_COLOR,
					 AppearanceRange::full());
	      appear->setWeightBold(true,AppearanceRange::full());
	      appear->end();
	    }
	  } else {
	    sheet.cellSummary(c,r,val[c]);
	  }
	}
      }
      r++;
      if (r>=sheet.height()) {
	r = -1;
      }
      rowCursor = r;
      dbg_printf("Cursor moved to %d\n", r);
      return true;
    }
    break;
  default:
    fprintf(stderr,"* ERROR: unsupported row operation\n");
    return false;
    break;
  }
  return true;
}

bool SheetPatcher::declareNames(const std::vector<std::string>& names, 
				bool final) {
  PolySheet sheet = getSheet();
  if (!sheet.isValid()) return false;
  if (chain) chain->declareNames(names,final);
  if (config.trustNames==false) {
    if ((int)names.size()!=sheet.width()) {
      fprintf(stderr,"* WARNING: name mismatch\n");
      return false;
    }
  } else {
    for (int i=0; i<(int)names.size(); i++) {
      printf("Checking %s\n", names[i].c_str());
    }
    fprintf(stderr,"Named columns not implemented yet\n");
    exit(1);
  }
  return true;
}

bool SheetPatcher::setSheet(const char *name) {
  if (chain) chain->setSheet(name);

  TextBook *book = getBook();
  if (book==NULL) return false;

  //reset
  config = ConfigChange();
  columns.clear();
  column_names.clear();
  rowCursor = -1;
  activeRow.resize(1,0);
  activeCol.resize(0,1);

  // load
  PolySheet psheet;
  attachSheet(psheet);
  psheet = book->readSheet(name);
  if (!psheet.isValid()) {
    fprintf(stderr,"Cannot find sheet %s\n", name);
    return false;
  }
  dbg_printf("Moved to sheet %s\n", name);
  attachSheet(psheet);
  //sheet = &psheet;
  setNames();

  return true;
}


bool SheetPatcher::mergeStart() {
  activeRow.resize(1,0);
  activeCol.resize(0,1);
  setNames();
  if (chain) chain->mergeStart();
  return true;
}

bool SheetPatcher::mergeDone() {
  if (chain) chain->mergeDone();
  if (descriptive) {
    PolySheet sheet = getSheet();
    if (!sheet.isValid()) return false;
    sheet.insertColumn(ColumnRef(0)); 
    //sheet.insertRow(RowRef(0));
    for (int i=0; i<sheet.height(); i++) {
      string txt = activeRow.cellString(0,i);
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
    int r = sniffer->getHeaderHeight()-1;
    if (r>=0 && r<sheet.height()) {
      for (int i=0; i<=r; i++) {
	sheet.cellString(0,i,string("@")+sheet.cellString(0,i));
      }
    }
    if (r<0) {
      sheet.insertRow(RowRef(0)); 
      sheet.cellString(0,yoff,"@");
      for (int i=1; i<sheet.width(); i++) {
	sheet.cellString(i,yoff,sniffer->suggestColumnName(i-1));
      }
    }
    int yoff = 0;
    /*
    {
      sheet.insertRow(RowRef(0)); 
      sheet.cellString(0,0,"!");
      for (int i=1; i<sheet.width(); i++) {
	sheet.cellString(i,0,activeCol.cellString(i-1,0));
      }
      yoff++;
    }
    */
    Poly<Appearance> appear = sheet.getRowAppearance(r+yoff);
    if (appear.isValid()) {
      appear->begin();
      appear->setWeightBold(true,AppearanceRange::full());
      appear->end();
    }

  }
  return true;
}

bool SheetPatcher::mergeAllDone() {
  if (chain) chain->mergeAllDone();
  return true;
}


void SheetPatcher::setNames() {
  PolySheet sheet = getSheet();
  if (!sheet.isValid()) return;
  if (&sheet!=sniffedSheet) {
    clearNames();
    sniffer = new NameSniffer(sheet);
    COOPY_ASSERT(sniffer);
    sniffedSheet = &sheet;
    activeCol.resize(sheet.width(),1);
    for (int i=0; i<sheet.width(); i++) {
      activeCol.cellString(i,0) = sniffer->suggestColumnName(i);
    }
  }
}


