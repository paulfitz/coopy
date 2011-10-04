#include <coopy/Merger.h>
#include <coopy/Dbg.h>
#include <coopy/Mover.h>
#include <coopy/NameSniffer.h>
#include <coopy/IndexSniffer.h>

#include <stdlib.h>
#include <ctype.h>

#include <algorithm>

#define WANT_MAP2STRING
#define WANT_VECTOR2STRING
#include <coopy/Stringer.h>

using namespace std;
using namespace coopy::store;
using namespace coopy::cmp;

static string normalize_string(string low, const CompareFlags& flags) {
  if (flags.ignore_case) {
    for (size_t c=0; c<low.length(); c++) {
      low[c] = tolower(low[c]);
    }
  }
  return low;
}

static bool compare_string(const SheetCell& a, const SheetCell& b,
			   const CompareFlags& flags) {
  if (!flags.ignore_case) {
    return a==b;
  }
  if (a.escaped!=b.escaped) return false;
  return normalize_string(a.text,flags)==normalize_string(b.text,flags);
}

bool Merger::mergeRow(coopy::store::DataSheet& pivot, 
		      coopy::store::DataSheet& local, 
		      coopy::store::DataSheet& remote,
		      MatchUnit& row_unit, 
		      Patcher& output,
		      const CompareFlags& flags, 
		      std::vector<coopy::cmp::RowChange>& rc) {

  bool fixedColumns = flags.fixed_columns;
  bool diff = output.wantDiff();
  bool link = output.wantLinks();
  int pRow = row_unit.pivotUnit;
  int lRow = row_unit.localUnit;
  int rRow = row_unit.remoteUnit;
  bool delRow = row_unit.deleted;
  string blank = "__NOT_SET__CSVCOMPARE_SSFOSSIL";
  SheetCell blankCell;
  blankCell.text = blank;
  blankCell.escaped = true;
  vector<SheetCell> expandLocal, expandRemote, expandPivot, expandMerge;
  vector<SheetCell> saveLocal;
  vector<int> expandDel;
  vector<int> existsLocally;
  map<string,SheetCell> cond, value, value0, conflicted_value;
  vector<string> address;
  vector<string> action;
  int lastCol = -1;
  int addCol = 0;
  address.push_back("0");
  action.push_back("select");
  int at = 0;
  for (list<MatchUnit>::iterator it=col_merge.accum.begin();
       it!=col_merge.accum.end(); 
       it++) {
    MatchUnit& unit = *it;
    int pCol = unit.pivotUnit;
    int lCol = unit.localUnit;
    int rCol = unit.remoteUnit;
    bool deleted = unit.deleted;
    string mval = "";
    if (diff||!deleted) {
      expandDel.push_back(deleted);
      existsLocally.push_back(lCol!=-1);
      if (lRow>=0 && lCol>=0) {
	//printf("access local %d %d (size %d %d)\n", lCol, lRow, 
	//local.width(), local.height());
	expandLocal.push_back(local.cellSummary(lCol,lRow));
      } else {
	expandLocal.push_back(blankCell);
      }
      if (rRow>=0 && rCol>=0) {
	//printf("access remote %d %d\n", rCol, rRow);
	expandRemote.push_back(remote.cellSummary(rCol,rRow));
      } else {
	expandRemote.push_back(blankCell);
      }
      if (pRow>=0 && pCol>=0) {
	//printf("access pivot %d %d\n", pCol, pRow);
	expandPivot.push_back(pivot.cellSummary(pCol,pRow));
      } else {
	expandPivot.push_back(blankCell);
      }
    }
    if (lRow>=0 && lCol>=0 && !deleted) {
      if (diff) {
	//printf("I think that %s has name %s\n",
	//local.cellSummary(lCol,lRow).toString().c_str(),
	//names[at].c_str());
	//cond[names[at]] = pivot.cellSummary(pCol,pRow);
	cond[names[at]] = local.cellSummary(lCol,lRow);
      }
    }
    if (!deleted) {
      at++;
    }
    if (diff) {
      if (lCol!=-1) {
	lastCol = lCol;
	addCol = 0;
      } else {
	addCol++;
      }
      char buf[256];
      if (addCol>0) {
	snprintf(buf,sizeof(buf),"%d+%d", lastCol+1, addCol);
      } else {
	snprintf(buf,sizeof(buf),"%d", lastCol+1);
      }
      address.push_back(buf);
      if (deleted) {
	action.push_back("delete");
      } else if (lCol==-1) {
	action.push_back("add");
      } else {
	action.push_back("");
      }
    }
  }
  //printf("Onwards\n");
  bool conflict = false;
  bool change = false;
  expandMerge = expandLocal;
  at = 0;
  for (size_t i=0; i<expandLocal.size(); i++) {
    if (fixedColumns) {
      if (!existsLocally[i]) continue;
    }
    SheetCell& _l = expandMerge[i];
    SheetCell& _r = expandRemote[i];
    SheetCell& _p = expandPivot[i];
    bool novel = false;
    bool conflicted1 = false;
    bool deleted = (bool)expandDel[i];
    bool ignored = false;
    if (!deleted) {
      if (filtered_names.size()>0) {
	if (filtered_names.find(names[at])==filtered_names.end()) {
	  ignored = true;
	}
      }
    }
    if (!ignored) {
      if (!compare_string(_l,_r,flags)) {
	if (_l==blankCell) {
	  if (!_r.escaped) {
	    _l = _r;
	    novel = true;
	  }
	} else {
	  if (_r!=blankCell) {
	    // two assertions, do they conflict?
	    // if pivot is the same as either, then no.
	    if (compare_string(_p,_l,flags)||compare_string(_p,_r,flags)) {
	      if (compare_string(_p,_l,flags)) { 
		_l = _r; 
		change = true;
		novel = true;
	      }
	    } else {
	      fprintf(stderr,"# conflict: {{%s}} vs {{%s}} from {{%s}}\n",
		      _l.toString().c_str(),
		      _r.toString().c_str(),
		      _p.toString().c_str());
	      conflict = true;
	      conflicted1 = true;
	      change = true;
	      novel = true;
	      //break;
	    }
	  }
	}
      }
    }
    
    /*
    if (_csv_verbose) {
      printf(" expandMerge: %s\n", vector2string(expandMerge).c_str());
      printf(" expandPivot: %s\n", vector2string(expandPivot).c_str());
      printf(" expandLocal: %s\n", vector2string(expandLocal).c_str());
      printf(" expandRemote: %s\n", vector2string(expandRemote).c_str());
      printf(" change %d novel %d conflict %d\n", change, novel, conflict);
    }
    */

    if (diff) {
      if (!deleted) {
	if (novel) {
	  value[names[at]] = _l;
	  if (conflicted1) {
	    //printf("SETTING conflicted value\n");
	    conflicted_value[names[at]] = _r;
	  }
	}
	at++;
      }
    }
    if (diff&&!novel) {
      if (!delRow) {
	_l = blankCell;
	_r = blankCell;
	_p = blankCell;
	expandLocal[i] = blankCell;
      }
    }
  }

  /*
  dbg_printf("row lens merge local remote %d %d %d\n",
	     (int)expandMerge.size(), 
	     (int)expandLocal.size(),
	     (int)expandRemote.size());
  */

  if (link) {
    LinkDeclare decl;
    decl.mode = LINK_DECLARE_MERGE;
    decl.column = false;
    decl.rc_id_pivot = pRow;
    decl.rc_id_local = lRow;
    decl.rc_id_remote = rRow;
    decl.rc_deleted = delRow;
    output.declareLink(decl);
  }

  if (!diff) {
    if (conflict) {
      conflicts++;
      output.addRow("[local]",expandLocal,blank);
      output.addRow("[conflicting]",expandRemote,blank);
    } else {
      if (lRow!=-1 && rRow!=-1) {
	output.addRow("",expandMerge,blank);
      } else if (lRow!=-1) {
	output.addRow("",expandMerge,blank); // local add
      } else if (rRow!=-1) {
	output.addRow("[add]",expandMerge,blank); // remote add
      }
    }
  } else {

    /*
    if (conflict) {
      //printf("Cannot produce a diff when there are data conflicts\n");
      //fprintf(stderr,"Conflict Alert!\n");
      //return false;
    }
    */
    if (address!=lastAddress) {
      output.addRow("[for]",address,blank);
      lastAddress = address;
    }
    if (address!=lastAddress || action!=lastAction) {
      output.addRow("[do]",action,blank);
      lastAction = action;
    }

    bool activity = true;

    /*
    if (lRow!=-1) {
      if ((int)expandMerge.size()==local.width()) {
	if (current_row<local.height()) {
	  size_t i;
	  for (i=0; i<expandMerge.size(); i++) {
	    SheetCell data = expandMerge[i];
	    SheetCell was = local.cellSummary(i,current_row);
	    if (was!=data && data!=blankCell) {
	      break;
	    }
	  }
	  if (i==expandMerge.size()) {
	    activity = false;
	  }
	}
      }
    }
    */

    /*
    dbg_printf("Row: (index p/l/r %d %d %d) act %d del %d / sz %d %d %d %d\n",
	       pRow, lRow, rRow, 
	       activity, 
	       delRow,
	       (int)expandMerge.size(), local.width(), current_row, local.height());
    */

    RowChange rowChange;
    RowChange rowChangeMove;
    bool haveMove = false;
    rowChange.cond = cond;
    rowChange.val = value;
    rowChange.conflictingVal = conflicted_value;
    rowChange.names = names;
    rowChange.conflicted = conflict;
    bool prev_had_row = had_row;
    if (last_local_row!=-1) {
      had_row = true;
      had_foreign_row = false;
    }
    if (!delRow) {
      //dbg_printf("Cursor? lRow %d last_local_row %d last_local_row_marked %d had_row %d\n",
      //lRow, last_local_row, last_local_row_marked, had_row);
      if (lRow!=-1) {
	if (had_row||had_foreign_row) { //last_local_row!=-1) {
	  //if (lRow!=last_local_row+1||last_local_row==-1) {
	  if (lRow<bottom_local_row||had_foreign_row) {
	    //if (fixed_row.find(lRow)!=fixed_row.end()) {
	    if (last_local_row>=0) {
	      if (last_local_row_marked!=last_local_row) {
		RowChange alt = lastRowChange;
		alt.mode = ROW_CHANGE_CONTEXT;
		if (flags.use_order) {
		  rc.push_back(alt);
		}
	      }
	    }
	    if (prev_had_row||lRow!=0) {
	      dbg_printf("MOVE! lRow %d last_local_row %d last_local_row_marked %d\n",
			 lRow, last_local_row, last_local_row_marked);
	      RowChange alt = rowChange;
	      alt.mode = ROW_CHANGE_MOVE;
	      if (flags.use_order) {
		haveMove = true;
		rowChangeMove = alt;
		//rc.push_back(alt);
	      }
	    }
	    last_local_row_marked = lRow;
	  }
	}
      }
    }
    if (activity||delRow) {
      char buf[256];
      if (lRow==-1) {
	addition++;
	snprintf(buf,sizeof(buf),"%d+%d",last_row+1,addition);
      } else {
	snprintf(buf,sizeof(buf),"%d",lRow+1);
	addition = 0;
      }
      SheetCell cbuf(buf,false);
      expandMerge.insert(expandMerge.begin(),cbuf);
      expandLocal.insert(expandLocal.begin(),cbuf);
      expandRemote.insert(expandRemote.begin(),cbuf);
      if (change) {
	output.addRow("[-]",expandLocal,blank);
      }
      if (lRow==-1) {
	if (haveMove) {
	  rc.push_back(rowChangeMove);
	  haveMove = false;
	}
	output.addRow("[+++]",expandMerge,blank);
	rowChange.mode = ROW_CHANGE_INSERT;
	//output.changeRow(rowChange);
	//printf("last_local_row_marked %d last_local_row %d lRow %d\n",
	//last_local_row_marked, last_local_row, lRow);
	if (last_local_row>=0) {
	  if (last_local_row_marked!=last_local_row) {
	    RowChange alt = lastRowChange;
	    alt.mode = ROW_CHANGE_CONTEXT;
	    if (flags.use_order) {
	      rc.push_back(alt);
	    }
	  }
	} else {
	  if (!(prev_had_row||had_foreign_row||allGone)) {
	    RowChange alt;
	    alt.mode = ROW_CHANGE_CONTEXT;
	    if (flags.use_order) {
	      rc.push_back(alt);
	    }
	  }
	}
	rc.push_back(rowChange);
	if (last_local_row<0) {
	  had_foreign_row = true;
	}
	last_local_row_marked = lRow;
      } else {
	if (rRow==-1) {
	  if (pRow!=-1) {
	    if (haveMove) {
	      rc.push_back(rowChangeMove);
	      haveMove = false;
	    }
	    output.addRow("[---]",expandLocal,blank);
	    rowChange.mode = ROW_CHANGE_DELETE;
	    //output.changeRow(rowChange);
	    rc.push_back(rowChange);
	    last_local_row_marked = lRow;
	  }
	} else {
	  if (value.size()!=0) {
	    output.addRow("[+]",expandMerge,blank);
	    rowChange.mode = haveMove?ROW_CHANGE_MOVE:ROW_CHANGE_UPDATE;
	    haveMove = false;
	    //output.changeRow(rowChange);
	    rc.push_back(rowChange);
	    last_local_row_marked = lRow;
	  }
	}
      }
    }
    if (haveMove) {
      rc.push_back(rowChangeMove);
      haveMove = false;
    }
    if (lRow!=-1 && !delRow) {
      current_row = lRow;
      last_row = lRow;
      current_row++;
    }
    last_local_row = lRow;
    lastRowChange = rowChange;
    if (last_local_row>=bottom_local_row) {
      bottom_local_row = last_local_row;
    }
  }
  return true;
}


bool Merger::merge(MergerState& state) {
  last_local_row = -1;
  bottom_local_row = -1;
  last_local_row_marked = -1;
  had_row = false;
  had_foreign_row = false;
  allGone = false;

  coopy::store::DataSheet& pivot = state.pivot;
  coopy::store::DataSheet& local = state.local;
  coopy::store::DataSheet& remote = state.remote;
  const OrderResult& row_local = state.nrow_local;
  const OrderResult& row_remote = state.nrow_remote;
  const OrderResult& col_local = state.ncol_local;
  const OrderResult& col_remote = state.ncol_remote;
  Patcher& output = state.output;
  const CompareFlags& flags = state.flags;
  NameSniffer& local_names = state.local_names;
  NameSniffer& remote_names = state.remote_names;

  bool diff = output.wantDiff();
  bool link = output.wantLinks();

  dbg_printf("Merging column order...\n");
  CompareFlags cflags = flags;
  cflags.head_trimmed = false;
  cflags.tail_trimmed = false;
  col_merge.merge(col_local,col_remote,cflags);

  dbg_printf("Merging row order...\n");

  if (false) { //trust_ids) {
    row_merge.merge_by_id(row_local,row_remote,flags);
  } else {
    if (col_merge.overlap==0 && diff) {
      dbg_printf("No overlap, just use remote...\n");
      row_merge.accum.clear();
      for (int i=0; i<remote.height(); i++) {
	MatchUnit unit;
	unit.pivotUnit = -1;
	unit.localUnit = -1;
	unit.remoteUnit = i;
	unit.deleted = false;
	row_merge.accum.push_back(unit);
      }
    } else {
      row_merge.merge(row_local,row_remote,flags);
    }
  }

  conflicts = 0;
  dbg_printf("Order merges are done...\n");

  if (link) {
    local_names.sniff();
    remote_names.sniff();

    /*
    // perspective: LOCAL, COLUMN
    for (int i=0; i<local.width(); i++) {
      LinkDeclare decl;
      decl.mode = LINK_DECLARE_LOCAL;
      decl.column = true;
      decl.rc_id_pivot = col_local.a2b(i);
      decl.rc_id_local = i;
      decl.rc_id_remote = -1;
      if (decl.rc_id_pivot!=-1) {
	decl.rc_id_remote = col_remote.b2a(decl.rc_id_pivot);
      }
      decl.rc_deleted = 0;
      decl.rc_str_local = local_names.suggestColumnName(i);
      if (decl.rc_id_remote!=-1) {
	decl.rc_str_remote = remote_names.suggestColumnName(decl.rc_id_remote);
      }
      output.declareLink(decl);      
    }

    // perspective: REMOTE, COLUMN
    for (int i=0; i<remote.width(); i++) {
      LinkDeclare decl;
      decl.mode = LINK_DECLARE_REMOTE;
      decl.column = true;
      decl.rc_id_pivot = col_remote.a2b(i);
      decl.rc_id_local = -1;
      if (decl.rc_id_pivot!=-1) {
	decl.rc_id_local = col_local.b2a(decl.rc_id_pivot);
      }
      decl.rc_id_remote = i;
      decl.rc_deleted = 0;
      decl.rc_str_remote = remote_names.suggestColumnName(i);
      if (decl.rc_id_local!=-1) {
	decl.rc_str_local = local_names.suggestColumnName(decl.rc_id_local);
      }
      output.declareLink(decl);      
    }
    */

    // perspective: MERGE, COLUMN
    for (list<MatchUnit>::iterator it=col_merge.accum.begin();
	 it!=col_merge.accum.end(); 
	 it++) {
      MatchUnit& unit = *it;
      int pCol = unit.localUnit;
      int lCol = unit.pivotUnit;
      int rCol = unit.remoteUnit;
      bool deleted = unit.deleted;
      LinkDeclare decl;
      decl.mode = LINK_DECLARE_MERGE;
      decl.column = true;
      decl.rc_id_pivot = pCol;
      decl.rc_id_local = lCol;
      decl.rc_id_remote = rCol;
      decl.rc_deleted = deleted;
      if (lCol!=-1) {
	decl.rc_str_local = local_names.suggestColumnName(lCol);
      }
      if (rCol!=-1) {
	decl.rc_str_remote = remote_names.suggestColumnName(rCol);
      }
      output.declareLink(decl);
    }

    /*
    // perspective: LOCAL, ROW
    for (int i=0; i<local.height(); i++) {
      LinkDeclare decl;
      decl.mode = LINK_DECLARE_LOCAL;
      decl.column = false;
      decl.rc_id_pivot = row_local.a2b(i);
      decl.rc_id_local = i;
      decl.rc_id_remote = -1;
      if (decl.rc_id_pivot!=-1) {
	decl.rc_id_remote = row_remote.b2a(decl.rc_id_pivot);
      }
      decl.rc_deleted = 0;
      output.declareLink(decl);      
    }

    // perspective: REMOTE, ROW
    for (int i=0; i<remote.height(); i++) {
      LinkDeclare decl;
      decl.mode = LINK_DECLARE_REMOTE;
      decl.column = false;
      decl.rc_id_pivot = row_remote.a2b(i);
      decl.rc_id_local = -1;
      if (decl.rc_id_pivot!=-1) {
	decl.rc_id_local = row_local.b2a(decl.rc_id_pivot);
      }
      decl.rc_id_remote = i;
      decl.rc_deleted = 0;
      output.declareLink(decl);      
    }
    */
  }

  allGone = false;
  if (diff) {
    current_row = 0;
    last_row = -1;
    addition = 0;
    lastAddress.clear();
    lastAction.clear();

    local_names.sniff();
    remote_names.sniff();
    //NameSniffer localName(local);
    //NameSniffer remoteName(remote);

    // for now, we will only use filtered index if column manipulations
    // are non-existent or trivial
    IndexSniffer localIndex(local,state.flags,local_names);
    bool constantColumns = true;
    bool constantIndex = true;

    vector<int> local_cols;
    vector<string> local_col_names;
    vector<string> original_col_names;
    for (int i=0; i<local.width(); i++) {
      local_cols.push_back(i);

      string name = local_names.suggestColumnName(i);
      /*
      if (name[0]>='0'&&name[0]<='9') {
	name = string("[") + name + "]";
      }
      */
      local_col_names.push_back(name);
    }
    original_col_names = local_col_names;

    vector<int> index_flags = localIndex.suggestIndexes();
    RowChange::txt2bool indexes;
    bool atLeastOne = false;
    for (int i=0; i<(int)original_col_names.size(); i++) {
      string name = original_col_names[i];
      indexes[name] = (index_flags[i]>0);
      atLeastOne = atLeastOne||indexes[name];
    }
    if (!atLeastOne) {
      indexes.clear();
    }

    vector<OrderChange> cc;

    bool fixedColumns = flags.fixed_columns;

    // Pass 1: signal any column deletions
    for (list<MatchUnit>::iterator it=col_merge.accum.begin();
	 it!=col_merge.accum.end(); 
	 it++) {
      MatchUnit& unit = *it;
      //int pCol = unit.localUnit;
      int lCol = unit.pivotUnit;
      //int rCol = unit.remoteUnit;
      bool deleted = unit.deleted;
      if (lCol!=-1 && deleted) {
	OrderChange change;
	change.indicesBefore = local_cols;
	change.namesBefore = local_col_names;
	vector<int>::iterator it = std::find(local_cols.begin(),
					     local_cols.end(),
					     lCol);
	if (it==local_cols.end()) {
	  fprintf(stderr,"Merge logic failure\n");
	  exit(1);
	}
	int idx = it-local_cols.begin();
	if (indexes.find(change.namesBefore[idx])!=indexes.end()) {
	  if (indexes[change.namesBefore[idx]]) {
	    constantIndex = false;
	  }
	}
	change.mode = ORDER_CHANGE_DELETE;
	change.subject = lCol;
	local_cols.erase(it);
	local_col_names.erase(local_col_names.begin()+idx);
	change.indicesAfter = local_cols;
	change.namesAfter = local_col_names;
	//output.changeColumn(change);
	cc.push_back(change);
	if (local_cols.size()==0) {
	  allGone = true;
	}
      }
    }


    // Pass 2: check order
    vector<int> shuffled_cols;
    for (list<MatchUnit>::iterator it=col_merge.accum.begin();
	 it!=col_merge.accum.end(); 
	 it++) {
      MatchUnit& unit = *it;
      //int pCol = unit.localUnit;
      int lCol = unit.pivotUnit;
      //int rCol = unit.remoteUnit;
      bool deleted = unit.deleted;
      if (lCol!=-1 && !deleted) {
	shuffled_cols.push_back(lCol);
      }
      //printf("[%d:%d:%d %d] ", lCol, pCol, rCol, deleted);
    }
    //printf("\n");

    dbg_printf("Column order is now %s\n",
	       vector2string(local_col_names).c_str());

    // Pass 2: signal any column shuffling
    // 1 2 3 4
    // 2 3 4 1
    Mover move;
    vector<int> move_order;
    
    if (local_cols.size()!=shuffled_cols.size()) {
      dbg_printf("Match failed %s:%d\n",
		 __FILE__, __LINE__);
      fprintf(stderr,"Match failed, please report %s:%d\n",
	      __FILE__, __LINE__);
      exit(1);
    }

    move.move(local_cols,shuffled_cols,move_order);
    dbg_printf("* move complete\n");
 
    if (move_order.size()>0) {
      // Should send messages for this case, but we're not ready
      // yet to exercise it.
      // For now, local order will remain unchanged.

      dbg_printf("MOVE order %d\n", (int)local_cols.size());
      dbg_printf("  [%s]\n", vector2string(local_cols).c_str());
      dbg_printf("  [%s]\n", vector2string(shuffled_cols).c_str());
      dbg_printf("  [%s]\n\n", vector2string(move_order).c_str());

      //vector<int> local_cols_save = local_cols;
      for (int m=0; m<(int)move_order.size(); m++) {
	int a = move_order[m];
	//int a = local_cols_save[p]
	dbg_printf("Move %d\n", a);

	OrderChange change;
	change.indicesBefore = local_cols;
	change.namesBefore = local_col_names;
	vector<int>::iterator it = std::find(local_cols.begin(),
					     local_cols.end(),
					     a);
	if (it==local_cols.end()) {
	  fprintf(stderr,"Merge logic failure\n");
	  exit(1);
	}
	vector<int>::iterator it2 = std::find(shuffled_cols.begin(),
					      shuffled_cols.end(),
					      a);
	if (it2==shuffled_cols.end()) {
	  fprintf(stderr,"Merge logic failure\n");
	  exit(1);
	}
	change.subject = *it;
	change.object = *it2;
	int idx = it-local_cols.begin();
	int idx2 = it2-shuffled_cols.begin();
	change.mode = ORDER_CHANGE_MOVE;
	local_cols.erase(it);
	string name = local_col_names[idx];
	change.object = *it2;
	local_col_names.erase(local_col_names.begin()+idx);
	local_cols.insert(local_cols.begin()+idx2,a);
	local_col_names.insert(local_col_names.begin()+idx2,name);
	change.indicesAfter = local_cols;
	change.namesAfter = local_col_names;
	//output.changeColumn(change);
	if (change.namesBefore!=change.namesAfter) {
	  cc.push_back(change);
	}
      }
    }

    dbg_printf("Column order is now %s\n",
	       vector2string(local_col_names).c_str());

    //printf(">>> %s %d\n", __FILE__, __LINE__);

    // Pass 3: signal any column insertions
    int at = 0;
    for (list<MatchUnit>::iterator it=col_merge.accum.begin();
	 it!=col_merge.accum.end(); 
	 it++) {
      MatchUnit& unit = *it;
      //int pCol = unit.localUnit;
      int lCol = unit.pivotUnit;
      int rCol = unit.remoteUnit;
      bool deleted = unit.deleted;
      if (lCol==-1 && rCol!=-1 && !deleted) {
	OrderChange change;
	change.indicesBefore = local_cols;
	change.namesBefore = local_col_names;
	change.mode = ORDER_CHANGE_INSERT;
	local_cols.insert(local_cols.begin()+at,-rCol-1);

	string name = remote_names.suggestColumnName(rCol);
	bool collision = false;
	if (name[0]>='0'&&name[0]<='9') {
	  name = string("{") + name + "}";
	}
	do {
	  collision = false;
	  for (int i=0; i<(int)local_col_names.size(); i++) {
	    if (local_col_names[i]==name) {
	      collision = true;
	      name = name + "_";
	      break;
	    }
	  }
	} while (collision);
	local_col_names.insert(local_col_names.begin()+at,name);
	change.indicesAfter = local_cols;
	change.namesAfter = local_col_names;
	change.subject = local_cols[at];
	//output.changeColumn(change);
	cc.push_back(change);
	at++;
      }
      if (lCol!=-1 && !deleted) {
	at++;
      }
    }

    //printf(">>> %s %d\n", __FILE__, __LINE__);

    dbg_printf("Column order is now %s\n",
	       vector2string(local_col_names).c_str());

    if (cc.size()>0) {
      constantColumns = false;
    }
    
    names = local_col_names;
    filtered_names.clear();

    if (fixedColumns) {
      for (int i=0; i<(int)original_col_names.size(); i++) {
	filtered_names.insert(original_col_names[i]);
      }
    }

    vector<RowChange> rc;
    // Now process rows
    if (!state.allIdentical) {
      for (list<MatchUnit>::iterator it=row_merge.accum.begin();
	   it!=row_merge.accum.end(); 
	   it++) {
	MatchUnit& unit = *it;

	// Special case: if all columns were deleted, then we assume
	// all local rows are deleted.
	if (allGone) {
	  unit.localUnit = -1;
	  unit.pivotUnit = -1;
	}

	if (unit.remoteUnit!=-1 || !allGone) {
	  bool ok = mergeRow(pivot,local,remote,unit,output,flags,rc);
	  if (!ok) { return false; }
	}
      }
    }

    if (fixedColumns) {
      local_col_names = original_col_names;
    }

    //printf(">>> %s %d\n", __FILE__, __LINE__);

    if (!fixedColumns) {
      NameChange nc;
      nc.mode = NAME_CHANGE_DECLARE;
      nc.final = false;
      nc.constant = constantColumns;
      nc.names = original_col_names;
      output.changeName(nc);
    }

    //printf(">>> %s %d\n", __FILE__, __LINE__);

    if (!fixedColumns) {
      for (int i=0; i<(int)cc.size(); i++) {
	OrderChange& change = cc[i];
	output.changeColumn(change);
      }
    }

    {
      NameChange nc;
      nc.mode = NAME_CHANGE_DECLARE;
      nc.final = true;
      nc.names = local_col_names;
      nc.constant = constantColumns;
      output.changeName(nc);
    }

    //printf(">>> %s %d\n", __FILE__, __LINE__);

    if (!constantIndex) {
      for (int i=0; i<(int)original_col_names.size(); i++) {
	string name = original_col_names[i];
	indexes[name] = true;
      }
    }
    for (int i=0; i<(int)rc.size(); i++) {
      /*
	scope for being smarter here about what gets scoped in.
       */
      RowChange& change = rc[i];
      change.indexes = indexes;
      change.allNames = local_col_names;
      output.changeRow(change);
    }

    //printf(">>> %s %d\n", __FILE__, __LINE__);

    output.mergeDone();
    return true;
  }

  // MERGE

  vector<string> header;
  for (list<MatchUnit>::iterator it=col_merge.accum.begin();
       it!=col_merge.accum.end(); 
       it++) {
    MatchUnit& unit = *it;
    //int pCol = unit.localUnit;
    int lCol = unit.pivotUnit;
    int rCol = unit.remoteUnit;
    bool deleted = unit.deleted;
    if (!deleted) {
      if (lCol!=-1&&rCol!=-1) {
	header.push_back("");
      } else if (lCol!=-1) {
	header.push_back(""); // local add
      } else if (rCol!=-1) {
	header.push_back("[add]"); // remote add
      } else {
	header.push_back("[float]");
      }
    }
  }
  output.addHeader("[conflict]",header,"");

  for (list<MatchUnit>::iterator it=row_merge.accum.begin();
       it!=row_merge.accum.end(); 
       it++) {
    MatchUnit& unit = *it;
    //int _l = unit.localUnit;
    //int _p = unit.pivotUnit;
    //int _r = unit.remoteUnit;
    bool deleted = unit.deleted;
    if (!deleted) {
      vector<RowChange> rc;
      bool ok = mergeRow(pivot,local,remote,unit,output,flags,rc);
      if (!ok) return false;
      for (int i=0; i<(int)rc.size(); i++) {
	output.changeRow(rc[i]);
      }
    }
  }

  output.mergeDone();

  if (conflicts==0) {
    dbg_printf("No conflicts!\n");
    output.stripMarkup();
  }

  //dbg_printf("Got merged result (%dx%d)\n", result.width(), result.height());
  //CsvFile::write(result,"result.csv");
  return true;
}



