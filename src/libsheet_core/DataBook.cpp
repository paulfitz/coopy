#include <coopy/DataBook.h>

#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace coopy::store;

bool DataBook::operator==(const DataBook& alt) const {
  DataBook *b1 = (DataBook *)this;
  DataBook *b2 = (DataBook *)&alt;
  vector<string> names = b1->getNames();
  vector<string> altNames = b2->getNames();
  if (names.size()!=1 || altNames.size()!=1) {
    fprintf(stderr,"Comparison not yet implemented in %s:%d\n", __FILE__,
	    __LINE__);
    exit(1);
  }

  PolySheet s1 = b1->readSheet(names[0]);
  PolySheet s2 = b2->readSheet(altNames[0]);
  if (s1.width()!=s2.width() || s1.height()!=s2.height()) {
    return false;
  }
  for (int j=0; j<s1.height(); j++) {
    for (int i=0; i<s1.width(); i++) {
      if (s1.cellSummary(i,j)!=s2.cellSummary(i,j)) {
	return false;
      }
    }
  }

  
  return true;
}


