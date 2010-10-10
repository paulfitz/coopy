
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

  void read(const TextSheet& sheet, int x, int y) {
    lst.clear();
    indices.clear();
    for (int i=x; i<sheet.width(); i++) {
      string v = sheet.cell(i,y);
      if (v=="") break;
      lst.push_back(v);
      indices.push_back(i-x);
    }
  }
  
  string toString() {
    return vector2string(lst);
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

  for (int i=0; i<patch.height(); i++) {
    dbg_printf("[%d] ", i);
    string cmd0 = patch.cell(0,i);
    string cmd1 = patch.cell(1,i);
    bool fail = false;
    if (cmd0=="dtbl") {
      dbg_printf("Processed header\n");
    } else if (cmd0=="config") {
      string key = patch.cell(1,i);
      string val = patch.cell(2,i);
      dbg_printf("Set config variable %s -> %s\n", key.c_str(), val.c_str());
    } else if (cmd0=="column") {
      if (cmd1=="name") {
	names.read(patch,3,i);
	dbg_printf("Set column names to %s\n", names.toString().c_str());
      } else if (cmd1=="move") {
	PatchColumnNames names2;
	names2.read(patch,3,i);
	OrderChange change;
	string mover = names2.inferMove(names,&change.subject,&change.object);
	dbg_printf("Moving columns to %s (%s moves // subj %d obj %d)\n", 
		   names2.toString().c_str(),
		   mover.c_str(),
		   change.subject,
		   change.object);
	change.indicesBefore = names.indices;
	change.namesBefore = names.lst;
	change.indicesAfter = names2.indices;
	change.namesAfter = names2.lst;
	change.mode = ORDER_CHANGE_MOVE;
	patcher->changeColumn(change);
      } else {
	fail = true;
      }
    } else {
      fail = true;
    }
    if (fail) {
      fprintf(stderr,"%s %s ?\n", cmd0.c_str(), cmd1.c_str());
      fail = false;
    }
  }

  return true;
}


