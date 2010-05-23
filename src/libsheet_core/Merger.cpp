#include <coopy/Merger.h>
#include <coopy/Dbg.h>

#include <stdlib.h>

using namespace std;

void Merger::addRow(const char *tag,
		    const vector<string>& row,
		    const string& blank) {
  CsvSheet& target = result;
  target.addField(tag);
  for (int i=0; i<row.size(); i++) {
    if (row[i]!=blank) {
      target.addField(row[i].c_str());
    } else {
      target.addField("");
    }
  }
  target.addRecord();
}

void Merger::mergeRow(TextSheet& pivot, TextSheet& local, TextSheet& remote,
		      MatchUnit& row_unit, bool diff) {
  
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
      addRow("[local]",expandLocal,blank);
      addRow("[conflicting]",expandRemote,blank);
    } else {
      if (lRow!=-1 && rRow!=-1) {
	addRow("",expandMerge,blank);
      } else if (lRow!=-1) {
	addRow("",expandMerge,blank); // local add
      } else if (rRow!=-1) {
	addRow("[add]",expandMerge,blank); // remote add
      }
    }
  } else {
    if (conflict) {
      printf("Cannot produce a diff when there are data conflicts\n");
      exit(1);
    }
    if (address!=lastAddress) {
      addRow("[for]",address,blank);
      lastAddress = address;
    }
    if (address!=lastAddress || action!=lastAction) {
      addRow("[do]",action,blank);
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
	addRow("[-]",expandLocal,blank);
      }
      if (lRow==-1) {
	addRow("[+++]",expandMerge,blank);
      } else {
	if (rRow==-1) {
	  addRow("[---]",expandLocal,blank);
	} else {
	  addRow("[+]",expandMerge,blank);
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
		   const OrderResult& col_remote) {

  dbg_printf("Merging row order...\n");
  row_merge.merge(row_local,row_remote);

  dbg_printf("Merging column order...\n");
  col_merge.merge(col_local,col_remote);

  conflicts = 0;

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
  addRow("[conflict]",header,"");

  for (list<MatchUnit>::iterator it=row_merge.accum.begin();
       it!=row_merge.accum.end(); 
       it++) {
    MatchUnit& unit = *it;
    //int _l = unit.localUnit;
    //int _p = unit.pivotUnit;
    //int _r = unit.remoteUnit;
    bool deleted = unit.deleted;
    if (!deleted) {
      mergeRow(pivot,local,remote,unit,false);
    }
  }

  if (conflicts==0) {
    dbg_printf("No conflicts!\n");
    CsvSheet tmp = result;
    result.clear();
    for (int y=1; y<tmp.height(); y++) {
      for (int x=1; x<tmp.width(); x++) {
	result.addField(tmp.cell(x,y).c_str());
      }
      result.addRecord();
    }
  }

  dbg_printf("Got merged result (%dx%d)\n", result.width(), result.height());
  //CsvFile::write(result,"result.csv");
}



void Merger::diff(TextSheet& pivot, TextSheet& local, TextSheet& remote,
		  const OrderResult& row_local,
		  const OrderResult& row_remote,
		  const OrderResult& col_local,
		  const OrderResult& col_remote) {

  row_merge.merge(row_local,row_remote);
  col_merge.merge(col_local,col_remote);
  conflicts = 0;
  current_row = 0;
  last_row = -1;
  addition = 0;
  lastAddress.clear();
  lastAction.clear();

  for (list<MatchUnit>::iterator it=row_merge.accum.begin();
       it!=row_merge.accum.end(); 
       it++) {
    MatchUnit& unit = *it;
    // ignoring row order for now ...
    mergeRow(pivot,local,remote,unit,true);
  }
}

