
#include "CsvMerge.h"

using namespace std;

static bool _merge_debug = false;

class CsvMergeTests {
private:
  bool debug;
public:
  CsvSheet age_sheet(string location, string age) {
    CsvSheet work;
    work.clear();
    work.addField("name");
    work.addField("location");
    work.addField("age");
    work.addRecord();
    work.addField("Paul");
    work.addField(location.c_str());
    work.addField(age.c_str());
    work.addRecord();
    return work;
  }

  int run() {
    CsvSheet s0 = age_sheet("Montclair","10");
    CsvSheet s1 = age_sheet("Montclair","20");
    CsvSheet s2 = age_sheet("Glen Ridge","10");
    CsvSheet target = age_sheet("Glen Ridge","20");
    CsvMerge merger;
    int result = merger.apply(s0,s1,s2);
    if (result!=0) {
      printf("FAIL\n");
    } else {
      printf(">>> %s", merger.get().encode().c_str());
    }
    return 0;
  }
};

int CsvMerge::apply(CsvSheet& pivot, CsvSheet& v1, CsvSheet& v2) {

  // let's just do an easy case, for practice
  
#if 0
  printf("***0 %s***1 %s***2 %s",
	 pivot.encode().c_str(),
	 v1.encode().c_str(),
	 v2.encode().c_str());
#endif
  
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
  int rh = remote.height();
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


int CsvMerge::run_tests() {
  printf("Running tests\n");
  _merge_debug = true;
  CsvMergeTests tests;
  return tests.run();
}

