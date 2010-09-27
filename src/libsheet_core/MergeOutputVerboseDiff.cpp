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


string verbose_encoder(const std::vector<std::string>& lst,
		       const std::map<std::string,std::string> & mp) {
  string x;
  for (vector<string>::const_iterator it = lst.begin();
       it!=lst.end();
       it++) {
    if (it!=lst.begin()) {
      x += " ";
    }
    x += *it;
    x += ":";
    if (mp.find(*it)!=mp.end()) {
      x += stringer_encoder(mp.find(*it)->second);
    }
  }
  return x;
}


bool MergeOutputVerboseDiff::changeColumn(const OrderChange& change) {
  printf("changeColumn: \n");
  printf("     subject: %d\n", change.subject);
  printf(" indexBefore: %s\n", vector2string(change.indicesBefore).c_str());
  printf("  indexAfter: %s\n", vector2string(change.indicesAfter).c_str());
  printf(" namesBefore: %s\n", vector2string(change.namesBefore).c_str());
  printf("  namesAfter: %s\n", vector2string(change.namesAfter).c_str());
  printf("\n");
  return true;
}

bool MergeOutputVerboseDiff::changeRow(const RowChange& change) {
  printf("changeRow: \n");
  //printf("  names: %s\n", verbose_encoder(change.names).c_str());
  printf("    conds: %s\n", verbose_encoder(change.names,change.cond).c_str());
  printf("     vals: %s\n", verbose_encoder(change.names,change.val).c_str());
  printf("\n");
  return true;
}


bool MergeOutputVerboseDiff::declareNames(const vector<string>& names, 
					  bool final) {
  printf("declareNames: %s (%s)\n",
	 vector2string(names).c_str(),
	 final?"final":"not final");
  printf("\n");
  return true;
}

bool MergeOutputVerboseDiff::setSheet(const char *name) { 
  printf("setSheet [%s]\n\n", stringer_encoder(name).c_str());
  return true;
}
