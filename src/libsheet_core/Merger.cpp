#include <coopy/Merger.h>
#include <coopy/Dbg.h>

#include <stdlib.h>

#include <algorithm>

using namespace std;

class Mover {
public:
  void move(const vector<int>& src, const vector<int>& dest, 
	    vector<int>& order, int depth) {
    vector<int> best_order;
    if (src!=dest) {

      // code not exercised yet
      fprintf(stderr,"Merger.cpp: Reorder needed for columns\n");
      exit(1);

      for (int i=0; i<src.size(); i++) {
	if (src[i]==dest[i]) {
	  continue;
	}
	vector<int> norder = order;
	vector<int> nsrc = src;
	int x = src[i];
	nsrc.erase(nsrc.begin()+i);
	vector<int>::const_iterator it = std::find(dest.begin(),
						   dest.end(),
						   x);
	nsrc.insert(nsrc.begin()+(it-dest.begin()),x);
	norder.push_back(i);
	move(nsrc,dest,norder,depth+1);
	if (norder.size()<best_order.size() || best_order.size()==0) {
	  best_order = norder;
	}
      }
    }
    printf("At %d, order len %d\n", depth, best_order.size());
    order = best_order;
  }
};


void Merger::mergeRow(TextSheet& pivot, TextSheet& local, TextSheet& remote,
		      MatchUnit& row_unit, MergeOutput& output) {
  
  bool diff = output.wantDiff();
  int pRow = row_unit.pivotUnit;
  int lRow = row_unit.localUnit;
  int rRow = row_unit.remoteUnit;
  bool delRow = row_unit.deleted;
  string blank = "__NOT_SET__CSVCOMPARE_SSFOSSIL";
  vector<string> expandLocal, expandRemote, expandPivot, expandMerge;
  vector<string> address;
  vector<string> action;
  int lastCol = -1;
  int addCol = 0;
  address.push_back("0");
  action.push_back("select");
  for (list<MatchUnit>::iterator it=col_merge.accum.begin();
       it!=col_merge.accum.end(); 
       it++) {
    MatchUnit& unit = *it;
    int pCol = unit.pivotUnit;
    int lCol = unit.localUnit;
    int rCol = unit.remoteUnit;
    bool deleted = unit.deleted;
    if (diff||!deleted) {
      if (lRow!=-1 && lCol!=-1) {
	//printf("access local %d %d (size %d %d)\n", lCol, lRow, 
	//local.width(), local.height());
	expandLocal.push_back(local.cell(lCol,lRow));
      } else {
	expandLocal.push_back(blank);
      }
      if (rRow!=-1 && rCol!=-1) {
	//printf("access remote %d %d\n", rCol, rRow);
	expandRemote.push_back(remote.cell(rCol,rRow));
      } else {
	expandRemote.push_back(blank);
      }
      if (pRow!=-1 && pCol!=-1) {
	//printf("access pivot %d %d\n", pCol, pRow);
	expandPivot.push_back(pivot.cell(pCol,pRow));
      } else {
	expandPivot.push_back(blank);
      }
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
  for (int i=0; i<expandLocal.size(); i++) {
    string& _l = expandMerge[i];
    string& _r = expandRemote[i];
    string& _p = expandPivot[i];
    bool novel = false;
    if (_l!=_r) {
      if (_l==blank) {
	_l = _r;
	novel = true;
      } else {
	if (_r!=blank) {
	  // two assertions, do they conflict?
	  // if pivot is the same as either, then no.
	  if (_p==_l||_p==_r) {
	    if (_p==_l) { 
	      _l = _r; 
	      change = true;
	      novel = true;
	    }
	  } else {
	    conflict = true;
	    break;
	  }
	}
      }
    }
    if (diff&&!novel) {
      if (!delRow) {
	_l = blank;
	_r = blank;
	_p = blank;
	expandLocal[i] = blank;
      }
    }
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
    if (conflict) {
      printf("Cannot produce a diff when there are data conflicts\n");
      exit(1);
    }
    if (address!=lastAddress) {
      output.addRow("[for]",address,blank);
      lastAddress = address;
    }
    if (address!=lastAddress || action!=lastAction) {
      output.addRow("[do]",action,blank);
      lastAction = action;
    }

    bool activity = true;
    if (expandMerge.size()==local.width()) {
      if (current_row<local.height()) {
	int i;
	for (i=0; i<expandMerge.size(); i++) {
	  string data = expandMerge[i];
	  string was = local.cell(i,current_row);
	  if (was!=data && data!=blank) {
	    break;
	  }
	}
	if (i==expandMerge.size()) {
	  activity = false;
	}
      }
    }

    dbg_printf("diff %d (%d %d %d) %d\n", activity, 
	       lRow, rRow, pRow, 
	       delRow);

    if (activity||delRow) {
      char buf[256];
      if (lRow==-1) {
	addition++;
	snprintf(buf,sizeof(buf),"%d+%d",last_row+1,addition);
      } else {
	snprintf(buf,sizeof(buf),"%d",lRow+1);
	addition = 0;
      }
      expandMerge.insert(expandMerge.begin(),buf);
      expandLocal.insert(expandLocal.begin(),buf);
      expandRemote.insert(expandRemote.begin(),buf);
      if (change) {
	output.addRow("[-]",expandLocal,blank);
      }
      if (lRow==-1) {
	output.addRow("[+++]",expandMerge,blank);
      } else {
	if (rRow==-1) {
	  output.addRow("[---]",expandLocal,blank);
	} else {
	  output.addRow("[+]",expandMerge,blank);
	}
      }
    }
    if (lRow!=-1) {
      current_row = lRow;
      last_row = lRow;
      current_row++;
    }
  }
}


void Merger::merge(TextSheet& pivot, TextSheet& local, TextSheet& remote,
		   const OrderResult& row_local,
		   const OrderResult& row_remote,
		   const OrderResult& col_local,
		   const OrderResult& col_remote,
		   MergeOutput& output) {
  bool diff = output.wantDiff();

  dbg_printf("Merging row order...\n");
  row_merge.merge(row_local,row_remote);
  dbg_printf("Merging column order...\n");
  col_merge.merge(col_local,col_remote);
  conflicts = 0;

  if (diff) {
    current_row = 0;
    last_row = -1;
    addition = 0;
    lastAddress.clear();
    lastAction.clear();

    vector<int> local_cols;
    vector<string> local_col_names;
    for (int i=0; i<local.width(); i++) {
      local_cols.push_back(i);
      char buf[256];
      sprintf(buf,"[%d]",i);
      local_col_names.push_back(buf);
    }

    // Pass 1: signal any column deletions
    for (list<MatchUnit>::iterator it=col_merge.accum.begin();
	 it!=col_merge.accum.end(); 
	 it++) {
      MatchUnit& unit = *it;
      int pCol = unit.localUnit;
      int lCol = unit.pivotUnit;
      int rCol = unit.remoteUnit;
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
	change.subject = *it;
	int idx = it-local_cols.begin();
	change.mode = ORDER_CHANGE_DELETE;
	local_cols.erase(it);
	local_col_names.erase(local_col_names.begin()+idx);
	change.indicesAfter = local_cols;
	change.namesAfter = local_col_names;
	output.changeColumn(change);
      }
    }

    // Pass 2: check order
    vector<int> shuffled_cols;
    for (list<MatchUnit>::iterator it=col_merge.accum.begin();
	 it!=col_merge.accum.end(); 
	 it++) {
      MatchUnit& unit = *it;
      int pCol = unit.localUnit;
      int lCol = unit.pivotUnit;
      int rCol = unit.remoteUnit;
      bool deleted = unit.deleted;
      if (lCol!=-1 && !deleted) {
	shuffled_cols.push_back(lCol);
      }
      //printf("[%d:%d:%d %d] ", lCol, pCol, rCol, deleted);
    }
    //printf("\n");

    // Pass 2: signal any column shuffling
    // 1 2 3 4
    // 2 3 4 1
    Mover move;
    vector<int> move_order;
    move.move(local_cols,shuffled_cols,move_order,0);
    // Should send messages for this case, but we're not ready
    // yet to exercise it.
    // For now, local order will remain unchanged.

    // Pass 3: signal any column insertions
    int at = 0;
    for (list<MatchUnit>::iterator it=col_merge.accum.begin();
	 it!=col_merge.accum.end(); 
	 it++) {
      MatchUnit& unit = *it;
      int pCol = unit.localUnit;
      int lCol = unit.pivotUnit;
      int rCol = unit.remoteUnit;
      bool deleted = unit.deleted;
      if (lCol==-1 && rCol!=-1 && !deleted) {
	OrderChange change;
	change.indicesBefore = local_cols;
	change.namesBefore = local_col_names;
	change.subject = at;
	change.mode = ORDER_CHANGE_INSERT;
	local_cols.insert(local_cols.begin()+at,-rCol-1);
	char buf[256];
	sprintf(buf,"{%d}",rCol);
	local_col_names.insert(local_col_names.begin()+at,buf);
	change.indicesAfter = local_cols;
	change.namesAfter = local_col_names;
	output.changeColumn(change);
	at++;
      }
      if (lCol!=-1 && !deleted) {
	at++;
      }
    }

    // Now process rows
    for (list<MatchUnit>::iterator it=row_merge.accum.begin();
	 it!=row_merge.accum.end(); 
	 it++) {
      MatchUnit& unit = *it;
      // ignoring row order for now ...
      mergeRow(pivot,local,remote,unit,output);
    }
    return;
  }

  // MERGE

  vector<string> header;
  for (list<MatchUnit>::iterator it=col_merge.accum.begin();
       it!=col_merge.accum.end(); 
       it++) {
    MatchUnit& unit = *it;
    int pCol = unit.localUnit;
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
      mergeRow(pivot,local,remote,unit,output);
    }
  }

  if (conflicts==0) {
    dbg_printf("No conflicts!\n");
    output.stripMarkup();
  }

  //dbg_printf("Got merged result (%dx%d)\n", result.width(), result.height());
  //CsvFile::write(result,"result.csv");
}



