#include <coopy/MergeOutputVerboseDiff.h>
#include <coopy/SheetStyle.h>
#include <coopy/DataSheet.h>

#include <stdio.h>
#include <stdlib.h>

#define WANT_MAP2STRING
#define WANT_VECTOR2STRING
#include <coopy/Stringer.h>

using namespace std;
using namespace coopy::store;
using namespace coopy::cmp;


template <class T>
string verbose_encoder(const std::vector<std::string>& lst,
		       const std::map<std::string,T> & mp) {
  string x;
  for (vector<string>::const_iterator it = lst.begin();
       it!=lst.end();
       it++) {
    if (mp.find(*it)!=mp.end()) {
      if (x.length()>0) {
	x += " ";
      }
      x += *it;
      x += ":";
      x += stringer_encoder(mp.find(*it)->second);
    }
  }
  return x;
}

/*
string index_encoder(const std::vector<std::string>& lst,
		     const std::vector<int> & mp) {
  string x;
  int at = 0;
  for (vector<string>::const_iterator it = lst.begin();
       it!=lst.end();
       it++) {
    if (mp[at]) {
      if (x.length()>0) {
	x += " ";
      }
      x += *it;
    }
    at++;
  }
  return x;
}
*/


bool MergeOutputVerboseDiff::changeColumn(const OrderChange& change) {
  fprintf(out,"changeColumn: %s\n", change.modeString().c_str());
  fprintf(out,"     subject: %d\n", change.subject);
  fprintf(out," indexBefore: %s\n", vector2string(change.indicesBefore).c_str());
  fprintf(out,"  indexAfter: %s\n", vector2string(change.indicesAfter).c_str());
  fprintf(out," namesBefore: %s\n", vector2string(change.namesBefore).c_str());
  fprintf(out,"  namesAfter: %s\n", vector2string(change.namesAfter).c_str());
  fprintf(out,"\n");
  return true;
}

bool MergeOutputVerboseDiff::changeRow(const RowChange& change) {
  fprintf(out,"changeRow: %s\n", change.modeString().c_str());
  fprintf(out,"    conds: %s\n", verbose_encoder(change.names,change.cond).c_str());
  fprintf(out,"     vals: %s\n", verbose_encoder(change.names,change.val).c_str());
  fprintf(out,"  indexes: %s\n", verbose_encoder(change.names,change.indexes).c_str());
  fprintf(out,"    names: %s\n", vector2string(change.names).c_str());
  fprintf(out,"      all: %s\n", vector2string(change.allNames).c_str());
  fprintf(out,"sequent'l: %s\n", change.sequential?"true":"false");
  if (change.context.beforeCount()!=0) {
    fprintf(out,"  context: covers %d row before this one\n", change.context.beforeCount());
    fprintf(out,"  context: %s\n", verbose_encoder(change.allNames,change.context.getRow(-1)).c_str());
  }
  fprintf(out,"\n");
  return true;
}


bool MergeOutputVerboseDiff::changeName(const NameChange& change) {
  const vector<string>& names = change.names;
  bool final = change.final;
  bool constant = change.constant;
  fprintf(out,"declareNames: %s\n",
	  vector2string(names).c_str());
  fprintf(out,"       final: %d\n", final?1:0);
  fprintf(out,"    constant: %d\n", constant?1:0);
  fprintf(out,"\n");
  return true;
}

bool MergeOutputVerboseDiff::setSheet(const char *name) { 
  fprintf(out,"setSheet [%s]\n\n", stringer_encoder(string(name)).c_str());
  return true;
}
