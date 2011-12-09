#include <coopy/BookCompare.h>
#include <coopy/SheetCompare.h>
#include <coopy/Dbg.h>
#include <coopy/CsvWrite.h>
#include <coopy/IndexSniffer.h>

#include <set>
#include <list>
#include <algorithm>

#include <stdio.h>

using namespace std;
using namespace coopy::store;
using namespace coopy::cmp;

int BookCompare::create(coopy::store::TextBook& local, 
			Patcher& output, const CompareFlags& flags) {
  if (!flags.pool) return -1;

  vector<string> names = local.getNames();
  output.mergeStart();
  for (int i=0; i<(int)names.size(); i++) {
    string sheetName = names[i];
    output.setSheet(sheetName.c_str());
    PolySheet sheet = local.readSheet(sheetName);
    output.addPoolsFromFlags(sheet);
    NameSniffer sniffer(sheet,flags);
    bool addedAuto = false;
    NameChange nc;
    nc.mode = NAME_CHANGE_DECLARE;
    nc.constant = true;
    nc.final = false;
    for (int i=0; i<sheet.width(); i++) {
      nc.names.push_back(sniffer.suggestColumnName(i));
    }
    nc.loud = (sheet.height()==0);
    output.changeName(nc);
    for (int i=0; i<sheet.width(); i++) {
      string name = sniffer.suggestColumnName(i);
      PoolColumnLink pc = flags.pool->lookup(sheetName,name);
      if (!pc.isValid()) {
	ColumnType t = sniffer.suggestColumnType(i);
	if (t.autoIncrement&&!addedAuto) {
	  flags.pool->create(sheetName,sheetName,name,true);
	  pc = flags.pool->lookup(sheetName,name);
	  addedAuto = true;
	}
	if (t.foreignKey!="") {
	  flags.pool->create(t.foreignTable,sheetName,name,false);
	  pc = flags.pool->lookup(sheetName,name);
	}
      }
      if (pc.isValid()) {
	PoolChange c;
	c.poolName = pc.getPoolName();
	c.tableName = pc.getTableName();
	c.pool.push_back(TableField("",pc.getColumnName(),pc.isInventor()));
	output.changePool(c);
      }
    }
    for (int y=0; y<sheet.height(); y++) {
      RowChange rc;
      rc.mode = ROW_CHANGE_INSERT;
      rc.names = nc.names;
      rc.allNames = nc.names;
      for (int x=0; x<sheet.width(); x++) {
	string name = sniffer.suggestColumnName(x);
	rc.val[name] = sheet.cellSummary(x,y);
      }
      output.changeRow(rc);
    }
  }
  output.mergeDone();
  output.mergeAllDone();

  return 0;
}

int BookCompare::compare(TextBook& pivot, TextBook& local, TextBook& remote, 
			 Patcher& output, const CompareFlags& flags) {
  // Merge currently based purely on names, no content comparison.
  // Hence a sheet rename cannot be guessed at yet.

  if (!remote.isValid()) {
    return create(local,output,flags);
  }

  bool pivot_is_local = false;
  bool pivot_is_remote = false;
  bool local_is_remote = false;
  if (&pivot.tail()==&local.tail()) {
    pivot_is_local = true;
  }
  if (&pivot.tail()==&remote.tail()) {
    pivot_is_remote = true;
  }
  if (&local.tail()==&remote.tail()) {
    local_is_remote = true;
  }
  //printf("%d %d %d\n", pivot_is_local, pivot_is_remote, local_is_remote);

  dbg_printf("BookCompare::compare begins\n");

  output.setFlags(flags);
  output.mergeStart();

  vector<string> pivot_names = pivot.getNames();
  vector<string> local_names = local.getNames();
  vector<string> remote_names = remote.getNames();
  set<string> pivot_name_set(pivot_names.begin(),pivot_names.end());
  set<string> local_name_set(local_names.begin(),local_names.end());
  set<string> remote_name_set(remote_names.begin(),remote_names.end());
  set<string> candidate_name_set;
  set<string> name_set;
  set_union(local_name_set.begin(),
	    local_name_set.end(),
	    remote_name_set.begin(),
	    remote_name_set.end(),
	    inserter(candidate_name_set, candidate_name_set.end()));
  for (set<string>::const_iterator it=candidate_name_set.begin();
       it!=candidate_name_set.end();
       it++) {
    string name = *it;

    if (flags.tables.size()>0) {
      if (flags.tables.find(name)==flags.tables.end()) continue;
    }

    bool in_pivot = (pivot_name_set.find(name)!=pivot_name_set.end());
    bool in_local = (local_name_set.find(name)!=local_name_set.end());
    bool in_remote = (remote_name_set.find(name)!=remote_name_set.end());
    if (!in_pivot) {
      // added in either local or remote
      name_set.insert(name);
    } else if (in_local&&in_remote) {
      // not removed
      name_set.insert(name);
    }
  }

  for (set<string>::const_iterator it=name_set.begin();
       it!=name_set.end();
       it++) {
    string name = *it;
    dbg_printf("\n\nBookCompare::compare - Working on \"%s\"\n", name.c_str());
    PolySheet pivot_sheet;
    PolySheet local_sheet;
    PolySheet remote_sheet;
    if (pivot_names.size()==1 && 
	local_names.size()==1 && 
	remote_names.size()==1) {
      pivot_sheet = pivot.readSheetByIndex(0);
      local_sheet = pivot_is_local?pivot_sheet:local.readSheetByIndex(0);
      remote_sheet = pivot_is_remote?pivot_sheet:(local_is_remote?local_sheet:remote.readSheetByIndex(0));
    } else {
      pivot_sheet = pivot.readSheet(name.c_str());
      local_sheet =  pivot_is_local?pivot_sheet:local.readSheet(name.c_str());
      remote_sheet = pivot_is_remote?pivot_sheet:(local_is_remote?local_sheet:remote.readSheet(name.c_str()));
    }

    PolySheet mapping;
    CompareFlags flags2 = flags;
    if (flags2.mapping_book) {
      mapping = flags2.mapping_book->readSheet(name.c_str());
      if (mapping.isValid()) {
	flags2.mapping = &mapping;
      }
    }

    if (pivot_sheet.isValid()&&
	local_sheet.isValid()&&
	remote_sheet.isValid()) {
      SheetCompare cmp;
      if (local.namedSheets()&&remote.namedSheets()) {
	bool ok = output.setSheet(name.c_str());
	if (!ok) {
	  fprintf(stderr,"Output format rejected sheet \"%s\"\n", name.c_str());
	  return -1;
	}
      }
      int r = cmp.compare(pivot_sheet,local_sheet,remote_sheet,output,
			  flags2);
      if (r!=0) return r;
    } else if (local_sheet.isValid()&&pivot_sheet.isValid()&&
	       !remote_sheet.isValid()) {
      dbg_printf("sheet removed in remote\n");
      output.removeSheet(name.c_str());
    } else if (remote_sheet.isValid()&&pivot_sheet.isValid()&&
	       !local_sheet.isValid()) {
      dbg_printf("sheet removed in local\n");
      output.removeSheet(name.c_str());
    } else if (remote_sheet.isValid()&&!pivot_sheet.isValid()&&
	       !local_sheet.isValid()) {
      dbg_printf("sheet added in remote\n");
      output.addSheet(name.c_str(),remote_sheet);
    } else if (local_sheet.isValid()&&!pivot_sheet.isValid()&&
	       !remote_sheet.isValid()) {
      dbg_printf("sheet added in local\n");
      output.addSheet(name.c_str(),local_sheet);
    } else {
      fprintf(stderr,"Could not find matching sheets - no big deal, but not ready for this yet\n");
      return -1;
    }
    dbg_printf("BookCompare::compare - Done with \"%s\"\n", name.c_str());
    output.mergeDone();
  }

  output.mergeAllDone();

  return 0;
}
  
void BookCompare::setVerbose(bool verbose) {
  _csv_verbose = verbose;
}

class ConflictCell {
public:
  SheetCell local, remote, pivot;
  bool have_local;
  bool have_remote;
  bool have_pivot;

  ConflictCell() { reset(); }

  void reset() {
    local = remote = pivot = SheetCell();
    have_local = have_remote = have_pivot = false;
  }

  bool parse(const SheetCell& src);
};

bool ConflictCell::parse(const SheetCell& src) {
  reset();

  if (src.text.substr(0,3)!="(((") return false;

  // current format: ((( pivot ))) local /// remote
  // not worrying about null or quoting yet

  string::size_type pivot_start = 4;
  string::size_type pivot_end = src.text.find(" ))) ");
  if (pivot_end == string::npos) return false;
  string::size_type local_start = pivot_end + 5;
  string::size_type local_end = src.text.find(" /// ");
  if (local_end == string::npos) return false;
  string::size_type remote_start = local_end + 5;
  local = SheetCell(src.text.substr(local_start,local_end-local_start),false);
  remote = SheetCell(src.text.substr(remote_start,src.text.length()),false);
  pivot = SheetCell(src.text.substr(pivot_start,pivot_end-pivot_start),false);
  have_local = have_remote = have_pivot = true;

  return true;
}


int BookCompare::resolve(coopy::store::TextBook& pivot, 
			 coopy::store::TextBook& local, 
			 coopy::store::TextBook& remote, 
			 Patcher& output, const CompareFlags& flags) {
  string resolve = flags.resolve;
  bool use_remote = false;
  bool use_local = false;
  if (resolve=="theirs") {
    use_remote = true;
  } else if (resolve=="ours") {
    use_local = true;
  } else if (resolve=="neither") {
    // ok
  } else {
    if (resolve=="") {
      fprintf(stderr,
	      "Please choose one of --theirs / --ours / --neither for conflict resolution.\n");
    } else
      fprintf(stderr,
	      "Do not understand choice '%s' for conflict resolution.\n",
	      resolve.c_str());
    return 1;
  }

  output.setFlags(flags);
  output.mergeStart();

  vector<string> local_names = local.getNames();
  for (int i=0; i<(int)local_names.size(); i++) {
    PolySheet sheet = local.readSheetByIndex(i);
    if (!sheet.isValid()) continue;
    sheet.mustHaveSchema();
    SheetSchema *ss = sheet.getSchema();
    if (!ss) continue;
    int idx = ss->getColumnIndexByName("_MERGE_");
    if (idx<0) continue;
    output.setSheet(local_names[i].c_str());
    NameChange nc0;
    nc0.mode = NAME_CHANGE_DECLARE;
    nc0.final = false;
    nc0.constant = false;
    NameChange nc1;
    nc1.mode = NAME_CHANGE_DECLARE;
    nc1.final = true;
    nc1.constant = false;

    OrderChange oc;
    oc.mode = ORDER_CHANGE_DELETE;
    for (int i=0; i<ss->getColumnCount(); i++) {
      string name = ss->getColumnInfo(i).getName();
      oc.indicesBefore.push_back(i);
      oc.namesBefore.push_back(name);
      nc0.names.push_back(name);
      if (name!="_MERGE_") {
	oc.indicesAfter.push_back(i);
	oc.namesAfter.push_back(name);
	nc1.names.push_back(name);
      } else {
	oc.subject = i;
      }
    }

    sheet.hideHeaders();
    int w = sheet.width();
    int h = sheet.height();
    vector<string> onames = nc0.names;
    vector<string> names = nc1.names;

    // logic dupe from Merger.cpp
    NameSniffer ns(sheet,flags);
    IndexSniffer localIndex(sheet,flags,ns);
    vector<int> index_flags = localIndex.suggestIndexes();
    RowChange::txt2bool indexes;
    bool atLeastOne = false;
    for (int i=0; i<(int)names.size(); i++) {
      string name = names[i];
      indexes[name] = (index_flags[i]>0);
      atLeastOne = atLeastOne||indexes[name];
    }
    if (!atLeastOne) {
      indexes.clear();
    }
    // end logic dup

    list<RowChange> rcs;
    for (int y=0; y<h; y++) {
      string state = sheet.cellString(idx,y);
      if (state!="CONFLICT") continue;
      RowChange rc;
      rc.mode = ROW_CHANGE_UPDATE;
      rc.indexes = indexes;
      rc.allNames = nc1.names;
      rc.names = nc1.names;
      for (int x=0; x<w; x++) {
	if (x==idx) continue;
	SheetCell v = sheet.cellSummary(x,y);
	string col = onames[x];
	rc.cond[col] = v;
	ConflictCell cc;
	if (cc.parse(v)) {
	  if (use_local) {
	    rc.val[col] = cc.local;
	  } else if (use_remote) {
	    rc.val[col] = cc.remote;
	  } else {
	    rc.val[col] = cc.pivot;
	  }
	}
      }
      //output.changeRow(rc);
      rcs.push_back(rc);
    }

    output.changeName(nc0);
    output.changeColumn(oc);
    output.changeName(nc1);
    for (list<RowChange>::iterator it=rcs.begin(); it!=rcs.end(); it++) {
      output.changeRow(*it);
    }

    output.mergeDone();
  }
  output.mergeAllDone();

  return 0;
}


