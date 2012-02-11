#include <coopy/MergeOutputFilter.h>

using namespace std;

using namespace coopy::cmp;
using namespace coopy::store;

bool MergeOutputFilter::mergeAllDone() {
  Pool *pool = getFlags().pool;
  if (pool) {
    // default assumption is that IDs from parent are shared with remote
    // if assumption is not true, don't do the following.

    map<string,int> seen;
    for (std::list<RowUnit>::iterator it=rows.begin(); it!=rows.end(); it++) {
      RowUnit& unit = *it;
      RowChange& change = unit.change;
      if (change.mode == ROW_CHANGE_INSERT) {
	const std::string& name = unit.sheet_name;

	if (seen.find(name)==seen.end()) {
	  PolySheet sheet;
	  TextBook *book = getBook();
	  if (!book) {
	    fprintf(stderr,"Cannot find input\n");
	  } else {
	    sheet = getBook()->readSheet(name);
	    if (sheet.isValid()) {
	      addPoolsFromFlags(sheet,false);
	      NameSniffer sniffer(sheet,getFlags());
	      addPoolsFromSchema(sheet,sniffer,name,false);
	    }
	    seen[name] = 1;
	  }
	}

	for (RowChange::txt2cell::const_iterator it = change.val.begin();
	     it!=change.val.end(); it++) {
	  PoolColumnLink link = pool->lookup(name,it->first);
	  if (link.isInventor()) {
	    PoolRecord& rec = link.getColumn().put(it->second,SheetCell());
	    rec.linked = false;
	  }
	}
      }
    }
    pool->setScanned();
  }

  if (desired_sheets.size()<2) {
    for (std::list<RowUnit>::iterator it=rows.begin(); it!=rows.end(); it++) {
      emitRow(*it);
    }
    for (std::map<std::string,SheetUnit>::iterator it=sheet_units.begin();
	 it!=sheet_units.end(); it++) {
      emitPreambleIfNeeded(it->second);
    }
  } else {
    const CompareFlags& flags = getFlags();
    // could do with some optimization...
    for (int i=0; i<(int)flags.ordered_tables.size(); i++) {
      string name = flags.ordered_tables[i];
      for (std::list<RowUnit>::iterator it=rows.begin(); it!=rows.end(); it++) {
	if (it->sheet_name == name) {
	  emitRow(*it);
	}
      }
      emitPreambleIfNeeded(sheet_units[name]);
    }
  }
  chain->mergeDone();

  return chain->mergeAllDone();
}


bool MergeOutputFilter::emitRow(const RowUnit& row) {
  string name = row.sheet_name;
  if (name!=last_sheet_name) {
    emitPreamble(sheet_units[name]);
  }
  string resolve = getFlags().resolve;
  if (resolve!="") {
    if (row.change.conflicted) {
      if (resolve=="ours") {
	return true;
      }
      RowChange change2 = row.change;
      if (resolve=="theirs") {
	for (RowChange::txt2cell::iterator it = change2.conflictingVal.begin(); 
	     it != change2.conflictingVal.end(); it++) {
	  change2.val[it->first] = it->second;
	}
      } else if (resolve=="neither") {
	for (RowChange::txt2cell::iterator it = 
	       change2.conflictingParentVal.begin(); 
	     it != change2.conflictingParentVal.end(); it++) {
	  change2.val[it->first] = it->second;
	}
      }
      change2.conflictingVal.clear();
      change2.conflictingParentVal.clear();
      change2.conflicted = false;
      return chain->changeRow(change2);
    }
  }
  return chain->changeRow(row.change);
}

bool MergeOutputFilter::emitPreamble(const SheetUnit& preamble) {
  string name = preamble.sheet_name;
  //printf("emit [%s]?\n", name.c_str());
  if (name==last_sheet_name) { return false; }
  //printf("emit [%s]!\n", name.c_str());

  TextBook *book = getBook();
  if (getFlags().create_unknown_sheets && book) {
    PolySheet sheet;
    sheet = book->readSheet(name);
    if (sheet.isValid()) {
      if (getFlags().clean_sheets) {
	sheet.deleteData();
      }
    } else {
      dbg_printf("I need to create %s\n", name.c_str());

      for (std::list<PoolChange>::const_iterator it=preamble.pools.begin();
	   it != preamble.pools.end(); it++) {
	dbg_printf("apply pool\n");
	applyPool(*it);
      }

      vector<string> names;
      if (preamble.have_name1) {
	names = preamble.name1.names;
      } else if (preamble.have_name0) {
	names = preamble.name1.names;
      }

      if (names.size()==0) {
	fprintf(stderr,"Do not know column names to create %s\n", name.c_str());
	exit(1);
      }
      SimpleSheetSchema sss0, sss1;
      sss0.setSheetName(name.c_str());
      for (int i=0; i<(int)names.size(); i++) {
	sss0.addColumn(names[i].c_str());
      }
      book->setPool(getFlags().pool);
      book->fixSchema(sss0,sss1);
      book->addSheet(sss1);
    }
  }

  if (name!="") {
    chain->setSheet(name.c_str());
  }
  last_sheet_name = name;
  if (started_sheets.find(name)!=started_sheets.end()) {
    return true;
  }
  started_sheets[name] = 1;

  if (preamble.have_name0) {
    chain->changeName(preamble.name0);
  }
  for (std::list<OrderChange>::const_iterator it = preamble.orders.begin();
       it != preamble.orders.end(); it++) {
    chain->changeColumn(*it);
  }
  if (preamble.have_name1) {
    chain->changeName(preamble.name1);
  }
  for (std::list<PoolChange>::const_iterator it = preamble.pools.begin();
       it != preamble.pools.end(); it++) {
    chain->changePool(*it);
  }
  return true;
}


bool MergeOutputFilter::mergeStart() {
  const CompareFlags& flags = getFlags();
  for (int i=0; i<(int)flags.ordered_tables.size(); i++) {
    desired_sheets[flags.ordered_tables[i]] = 1;
  }
  return chain->mergeStart();
}


bool MergeOutputFilter::emitPreambleIfNeeded(const SheetUnit& preamble) { 
  if (preamble.row_count==0) {
    if (getFlags().canSchema()||preamble.orders.size()>0||
	preamble.pools.size()>0) {
      return emitPreamble(preamble);
    }
  }
  return true;
}


bool MergeOutputFilter::changeRow(const RowChange& change) { 
  if (!isActiveTable()) return false;
  SheetUnit unit = getSheetUnit();
  if (!(unit.have_name0||unit.have_name1)) {
    NameChange nc;
    nc.mode = NAME_CHANGE_DECLARE;
    nc.constant = true;
    nc.final = true;
    nc.names = change.allNames;
    changeName(nc);
  }
  switch (change.mode) {
  case ROW_CHANGE_INSERT:
    if (!getFlags().canInsert()) { return false; }
    break;
  case ROW_CHANGE_DELETE:
    if (!getFlags().canDelete()) { return false; }
    break;
  case ROW_CHANGE_UPDATE:
  case ROW_CHANGE_MOVE:
    if (!getFlags().canUpdate()) { return false; }
    break;
  }
  rows.push_back(RowUnit(sheet_name,change));
  getSheetUnit().row_count++;
  return true;
}

