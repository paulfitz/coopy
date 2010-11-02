
#include <coopy/PatchParser.h>
#include <coopy/CsvFile.h>
#include <coopy/Format.h>
#include <coopy/Dbg.h>

#include <stdio.h>

#include <string>
#include <vector>
#include <algorithm>

#define WANT_VECTOR2STRING
#include <coopy/Stringer.h>

using namespace coopy::cmp;
using namespace coopy::format;
using namespace coopy::store;
using namespace std;

class PatchColumnNames {
public:
  vector<string> lst;
  vector<int> indices;

  void read(const DataSheet& sheet, int x, int y, int len = -1) {
    lst.clear();
    indices.clear();
    int last = sheet.width()-1;
    if (len>=0) {
      last = x+len-1;
    }
    for (int i=x; i<=last; i++) {
      string v = sheet.cellString(i,y);
      if (v==""&&len==-1) break;
      lst.push_back(v);
      indices.push_back(i-x);
    }
  }
  
  string toString() {
    return vector2string(lst);
  }

  string inferInsert(const PatchColumnNames& prior,
		     int *dest) {
    COOPY_ASSERT(lst.size()==prior.lst.size()+1);
    int i;
    for (i=0; i<(int)prior.lst.size(); i++) {
      if (lst[i]!=prior.lst[i]) {
	break;
      }
    }
    // the inserted element is named by lst[i]
    string mover = lst[i];
    indices = prior.indices;
    int subject = i;
    if (subject>=prior.lst.size()) {
      subject = -1;
    }
    int neg = 0;
    for (int j=0; j<(int)indices.size(); j++) {
      if (indices[j]<neg) {
	neg = indices[j];
      }
    }
    neg--;
    if (subject>=0) {
      indices.insert(indices.begin()+subject,neg);
    } else {
      indices.push_back(neg);
    }
    if (dest!=NULL) *dest = subject;
    return mover;
  }

  string inferDelete(const PatchColumnNames& prior,
		     int *src) {
    COOPY_ASSERT(lst.size()==prior.lst.size()-1);
    int i;
    for (i=0; i<(int)lst.size(); i++) {
      if (lst[i]!=prior.lst[i]) {
	break;
      }
    }
    // the deleted element is named by prior.lst[i]
    string mover = prior.lst[i];
    indices = prior.indices;
    int subject = i;
    indices.erase(indices.begin()+subject);
    if (src!=NULL) *src = subject;
    return mover;
  }

  string inferMove(const PatchColumnNames& prior,
		   int *src,
		   int *dest) {
    COOPY_ASSERT(lst.size()==prior.lst.size());
    int i;
    for (i=0; i<(int)lst.size(); i++) {
      if (lst[i]!=prior.lst[i]) {
	break;
      }
    }
    // The moved element is named by either lst[i] or prior.lst[i]
    // We are guaranteed at least one element after this one.
    string mover;
    if (prior.lst[i+1]==lst[i]) {
      mover = prior.lst[i];
    } else {
      mover = lst[i];
    }
    int isrc = -1;
    int idest = -1;
    vector<string>::const_iterator it = 
      find(prior.lst.begin(),prior.lst.end(),mover);
    int rem = it-prior.lst.begin();
    isrc = rem;
    vector<string>::const_iterator it2 = 
      find(lst.begin(),lst.end(),mover);
    int add = it2-lst.begin();
    if (add==((int)lst.size())-1) {
      idest = -1;
    } else {
      vector<string>::const_iterator it_post = it2;
      it_post++;
      vector<string>::const_iterator it_post_org = 
	find(prior.lst.begin(),prior.lst.end(),*it_post);
      idest = it_post_org-prior.lst.begin();
    }
    indices = prior.indices;
    int v = indices[rem];
    indices.erase(indices.begin()+rem);
    if (add>rem) add--;
    indices.insert(indices.begin()+add,v);
    if (src!=NULL) *src = isrc;
    if (dest!=NULL) *dest = idest;

    return mover;
  }
};


bool PatchParser::apply() {
  if (reader==NULL) return false;
  if (patcher==NULL) return false;

  Format format = reader->getFormat();
  if (format.id!=FORMAT_PATCH_CSV) {
    fprintf(stderr,"Unsupported format\n");
    return false;
  }

  CsvSheet patch;
  if (CsvFile::read(*reader,patch)!=0) {
    fprintf(stderr,"Failed to read patch\n");
    return false;
  }

  PatchColumnNames names;
  vector<string> selector;
  int len = 0;
  int row_index = -1;
  bool ok = true;

  bool configSent = false;
  bool configSet = false;
  ConfigChange cc;
  for (int i=0; i<patch.height(); i++) {
    dbg_printf("[%d] ", i);
    string cmd0 = patch.cell(0,i);
    string cmd1 = patch.cell(1,i);
    bool fail = false;
    if (cmd0!="config" && configSet && !configSent) {
      patcher->changeConfig(cc);
    }
    if (cmd0=="dtbl") {
      dbg_printf("Processed header\n");
    } else if (cmd0=="config") {
      string key = patch.cell(1,i);
      string val = patch.cell(2,i);
      config.put(key.c_str(),val.c_str());
      dbg_printf("Set config variable %s -> %s\n", key.c_str(), val.c_str());
      if (key=="order") {
	if (val=="named") {
	  cc.ordered = false;
	  cc.complete = false;
	  cc.trustNames = true;
	} else if (val=="map") {
	  cc.ordered = true;
	  cc.complete = true;
	  cc.trustNames = false;
	} else {
	  fprintf(stderr,"key configuration unrecognized\n");
	  exit(1);
	}
	configSet = true;
      }
    } else if (cmd0=="column") {
      OrderChange change;
      PatchColumnNames names2;
      names2.read(patch,3,i);
      len = (int)names2.lst.size();
      change.indicesBefore = names.indices;
      change.namesBefore = names.lst;
      change.namesAfter = names2.lst;
      if (cmd1=="name") {
	dbg_printf("Set column names to %s\n", names2.toString().c_str());
	patcher->declareNames(names2.lst,false);
      } else if (cmd1=="move") {
	string mover = names2.inferMove(names,&change.subject,&change.object);
	dbg_printf("Moving columns to %s (%s moves // subj %d obj %d)\n", 
		   names2.toString().c_str(),
		   mover.c_str(),
		   change.subject,
		   change.object);
	change.indicesAfter = names2.indices;
	change.mode = ORDER_CHANGE_MOVE;
	patcher->changeColumn(change);
      } else if (cmd1=="insert") {
	string mover = names2.inferInsert(names,&change.subject);
	dbg_printf("Inserting columns to %s (%s insert // subj %d)\n", 
		   names2.toString().c_str(),
		   mover.c_str(),
		   change.subject);
	change.indicesAfter = names2.indices;
	change.mode = ORDER_CHANGE_INSERT;
	patcher->changeColumn(change);	
      } else if (cmd1=="delete") {
	string mover = names2.inferDelete(names,&change.subject);
	dbg_printf("Deleting columns from %s (%s delete // subj %d)\n", 
		   names2.toString().c_str(),
		   mover.c_str(),
		   change.subject);
	change.indicesAfter = names2.indices;
	change.mode = ORDER_CHANGE_DELETE;
	patcher->changeColumn(change);	
      } else {
	fail = true;
      }
      if (!fail) {
	names = names2;
      }
    } else if (cmd0=="row") {
      RowChange change;
      change.names = names.lst;
      PatchColumnNames names2;
      names2.read(patch,3,i,len);
      if (cmd1=="select") {
	dbg_printf("Selecting %s\n", names2.toString().c_str());
	selector = names2.lst;
      } else if (cmd1=="update") {
	dbg_printf("Updating to %s\n", names2.toString().c_str());	
	change.mode = ROW_CHANGE_UPDATE;
	for (int i=0; i<len; i++) {
	  string sel = selector[i];
	  if (sel!="*") {
	    change.cond[change.names[i]] = sel;
	  }
	  string val = names2.lst[i];
	  if (val!="*") {
	    change.val[change.names[i]] = val;
	  }
	}
	ok = patcher->changeRow(change);
      } else if (cmd1=="insert") {
	dbg_printf("Inserting %s\n", names2.toString().c_str());
	change.mode = ROW_CHANGE_INSERT;
	for (int i=0; i<len; i++) {
	  string val = names2.lst[i];
	  if (val!="*") {
	    change.val[change.names[i]] = val;
	  }
	}
	patcher->changeRow(change);	
      } else if (cmd1=="delete") {
	dbg_printf("Deleting %s\n", names2.toString().c_str());
	change.mode = ROW_CHANGE_DELETE;
	for (int i=0; i<len; i++) {
	  string val = names2.lst[i];
	  if (val!="*") {
	    change.cond[change.names[i]] = val;
	  }
	}
	ok = patcher->changeRow(change);	
      } else {
	fail = true;
      }
    } else {
      fail = true;
    }
    if (fail) {
      dbg_printf("%s %s ?\n", cmd0.c_str(), cmd1.c_str());
      if (!coopy_is_verbose()) {
	fprintf(stderr,"%s %s ?\n", cmd0.c_str(), cmd1.c_str());
      }
      fail = false;
    } else if (!ok) {
      dbg_printf("*** %s %s failed\n", cmd0.c_str(), cmd1.c_str());
      if (!coopy_is_verbose()) {
	fprintf(stderr,"*** %s %s failed\n", cmd0.c_str(), cmd1.c_str());
      }
      fail = false;
    }
  }

  return true;
}


