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
  fprintf(out,"\n");
  return true;
}


bool MergeOutputVerboseDiff::declareNames(const vector<string>& names, 
					  bool final) {
  fprintf(out,"declareNames: %s (%s)\n",
	 vector2string(names).c_str(),
	 final?"final":"not final");
  fprintf(out,"\n");
  return true;
}

bool MergeOutputVerboseDiff::setSheet(const char *name) { 
  fprintf(out,"setSheet [%s]\n\n", stringer_encoder(name).c_str());
  return true;
}
