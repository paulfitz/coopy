
#include <coopy/CsvMerge.h>

#include <stdio.h>

using namespace std;
using namespace coopy::store;
using namespace coopy::cmp;

static bool _merge_debug = false;

int CsvMerge::apply(CsvSheet& pivot, CsvSheet& v1, CsvSheet& v2) {

  work.clear();
  if (pivot.height()==v1.height() && pivot.height()==v2.height()) {
    if (pivot.width()==v1.width() && pivot.width()==v2.width()) {
      //printf("May be an easy case\n");
      for (int y=0; y<pivot.height(); y++) {
	int m1 = 0;
	int m2 = 0;
	for (int x=0; x<pivot.width(); x++) {
	  if (v1.cell(x,y)==pivot.cell(x,y)) m1++;
	  if (v1.cell(x,y)==pivot.cell(x,y)) m2++;
	  if (v1.cell(x,y)!=v2.cell(x,y)) {
	    if (v1.cell(x,y)==pivot.cell(x,y)) {
	      work.addField(v2.cell(x,y).c_str());
	    } else if (v2.cell(x,y)==pivot.cell(x,y)) {
	      work.addField(v1.cell(x,y).c_str());
	    } else {
	      if (_merge_debug) {
		printf("Mismatch [%s] [%s] [%s]\n",
		       pivot.cell(x,y).c_str(),
		       v1.cell(x,y).c_str(),
		       v2.cell(x,y).c_str());
	      }
	      return -1;
	    }
	  } else {
	    work.addField(v1.cell(x,y).c_str());
	  }
	}
	work.addRecord();
      }
      return 0;
    }
  }
  return -1;
}

void CsvMerge::dumb_conflict(CsvSheet& local, CsvSheet& remote) {
  int lw = local.width();
  int lh = local.height();
  int rw = remote.width();
  //int rh = remote.height();
  int w = lw+rw+1;
  int h = lw+rw+1;
  work.clear();
  for (int i=0; i<h; i++) {
    for (int j=0; j<w; j++) {
      printf("%d %d\n", i, j);
      if (i==0) {
	if (j==0) {
	  work.addField("[conflict]");
	} else if (j<1+lw) {
	  work.addField("local");
	} else {
	  work.addField("remote");
	}
      } else {
	if (j==0) {
	  if (i<1+lh) {
	    work.addField("local");
	  } else {
	    work.addField("remote");
	  }
	} else if (j<1+lw) {
	  if (i<1+lh) {
	    work.addField(local.cell(j-1,i-1).c_str());
	  } else {
	    work.addField("");
	  }
	} else {
	  if (i>=1+lh) {
	    work.addField(remote.cell(j-lw-1,i-lh-1).c_str());
	  } else {
	    work.addField("");
	  }
	}
      }
    }
    work.addRecord();
  }
}

