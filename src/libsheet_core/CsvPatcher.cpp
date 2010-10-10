#include <coopy/CsvPatcher.h>

#include <map>
#include <algorithm>

#include <stdio.h>

using namespace std;
using namespace coopy::cmp;
using namespace coopy::store;

bool CsvPatcher::changeColumn(const OrderChange& change) {
  if (sheet==NULL) return false;
  switch (change.mode) {
  case ORDER_CHANGE_DELETE:
    return sheet->deleteColumn(ColumnRef(change.subject));
    //return sheet->deleteColumn(ColumnRef(change.identityToIndex(change.subject)));
    break;
  case ORDER_CHANGE_INSERT:
    return sheet->insertColumn(ColumnRef(change.subject)).isValid();
    //return sheet->insertColumn(ColumnRef(change.identityToIndex(change.subject))).isValid();
    break;
  case ORDER_CHANGE_MOVE:
    return sheet->moveColumn(ColumnRef(change.subject),
			     ColumnRef(change.object)
			     ).isValid();
    //return sheet->moveColumn(ColumnRef(change.identityToIndex(change.subject)),
    //			     ColumnRef(change.identityToIndex(change.object))
    //			     ).isValid();
  default:
    fprintf(stderr,"* ERROR: Unknown column operation\n");
    break;
  }
  return false;
}

bool CsvPatcher::changeRow(const RowChange& change) {
  if (sheet==NULL) return false;
  map<string,int> dir;
  vector<int> active_cond;
  vector<string> cond;
  vector<int> active_val;
  vector<string> val;
  for (int i=0; i<(int)change.names.size(); i++) {
    dir[change.names[i]] = i;
    active_cond.push_back(0);
    cond.push_back("");
    active_val.push_back(0);
    val.push_back("");
  }
  for (RowChange::txt2txt::const_iterator it = change.cond.begin();
       it!=change.cond.end(); it++) {
    int idx = dir[it->first];
    //printf("  [cond] %d %s -> %s\n", idx, it->first.c_str(), it->second.c_str());
    active_cond[idx] = 1;
    cond[idx] = it->second;
  }
  for (RowChange::txt2txt::const_iterator it = change.val.begin();
       it!=change.val.end(); it++) {
    int idx = dir[it->first];
    //printf("  [val] %d %s -> %s\n", idx, it->first.c_str(), it->second.c_str());
    active_val[idx] = 1;
    val[idx] = it->second;
  }
  
  switch (change.mode) {
  case ROW_CHANGE_INSERT:
    {
      RowRef tail;
      int r = sheet->insertRow(tail).getIndex();
      if (r>=0) {
	//printf("Row %d -- %d\n", r, sheet->height());
	for (int c=0; c<sheet->width(); c++) {
	  if (active_val[c]) {
	    //printf("  %d %d %s\n", c, r, val[c].c_str());
	    sheet->cellString(c,r,val[c]);
	  }
	}
      }
    }
    break;
  case ROW_CHANGE_DELETE:
    {
      bool success = false;
      int r;
      for (r=0; r<sheet->height(); r++) {
	bool match = true;
	for (int c=0; c<sheet->width(); c++) {
	  if (active_cond[c]) {
	    if (sheet->cellString(c,r)!=cond[c]) {
	      match = false;
	      break;
	    }
	  }
	}
	if (match) {
	  RowRef row(r);
	  sheet->deleteRow(row);
	  success = true;
	  break;
	}
      }
      return success;
    }
    break;
  case ROW_CHANGE_UPDATE:
    {
      bool success = false;
      int r;
      for (r=0; r<sheet->height(); r++) {
	bool match = true;
	for (int c=0; c<sheet->width(); c++) {
	  if (active_cond[c]) {
	    if (sheet->cellString(c,r)!=cond[c]) {
	      match = false;
	      break;
	    }
	  }
	}
	if (match) {
	  for (int c=0; c<sheet->width(); c++) {
	    if (active_val[c]) {
	      sheet->cellString(c,r,val[c]);
	    }
	  }
	  success = true;
	  break;
	}
      }
      return success;
    }
    break;
  default:
    fprintf(stderr,"* ERROR: unsupported row operation\n");
    return false;
    break;
  }
  return true;
}

bool CsvPatcher::declareNames(const std::vector<std::string>& names, 
			      bool final) {
  if (sheet==NULL) return false;
  if ((int)names.size()!=sheet->width()) {
    fprintf(stderr,"* ERROR: name mismatch\n");
    return false;
  }
  return true;
}

