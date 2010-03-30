
#include "CsvCompare.h"
#include "CsvWrite.h"

#include <ctype.h>

#include <map>
#include <list>

using namespace std;

typedef std::string Feature;

class FVal {
public:
  int ct;
  int index;

  FVal() {
    ct = 0;
    index = -1;
  }

  void setIndex(int idx) {
    if (index==-1) {
      index = idx;
    } else {
      if (index!=idx) {
	index = -2;
      }
    }
  }
};


class FeatMan {
public:
  int xcurr, ycurr;
  int ct;
  FloatSheet rowMatch;
  bool query;
  int vigor;

  map<Feature,FVal> f;

  FeatMan() {
    query = false;
    vigor = 0;
  }

  void setVigor(int vigor) {
    this->vigor = vigor;
  }

  void queryBit(string txt) {
    if (f.find(Feature(txt))==f.end()) {
      // miss!
    } else {
      FVal& val = f[Feature(txt)];
      if (val.index>=0) {
	rowMatch.cell(val.index,ycurr)++;
      }
    }
    ct++;
    summarize();
  }

  void addBit(string txt) {
    if (f.find(Feature(txt))==f.end()) {
      f[Feature(txt)] = FVal();
    }
    FVal& val = f[Feature(txt)];
    //val.index.insert(ycurr);
    val.setIndex(ycurr);
    ct++;
    summarize();
  }

  void add(string txt, bool query) {
    int len = txt.length();
    for (int k=1; k<10; k++) {
      for (int i=0; i<len-k; i++){
	string part;
	string low;
	part = txt.substr(i,k+1);
	low = part;
	for (int c=0; c<low.length(); c++) {
	  low[c] = tolower(low[c]);
	}
	if (query) {
	  queryBit(part);
	  if (low!=part) queryBit(low);
	} else {
	  addBit(part);
	  if (low!=part) addBit(low);
	}
      }
    }
    /*
    } else if (vigor==1) {
      string low = txt;
      for (int c=0; c<low.length(); c++) {
	low[c] = tolower(low[c]);
      }
      int len = txt.length();
      for (int i=0; i<len-2; i++) {
	for (int j=i+1; j<len-1; j++) {
	  for (int k=j+1; k<len; k++) {
	    string part = string("") + low[i] + low[j] + low[k];
	    if (query) {
	      queryBit(part);
	    } else {
	      addBit(part);
	    }
	  }
	}
      }
    }
    */
  }

  void apply(CsvSheet& a, IntSheet& asel, bool query) {
    this->query = query;
    int w = a.width();
    int h = a.height();
    ct = 0;
    for (int y=0; y<h; y++) {
      if (asel.cell(0,y)==-1) {
	for (int x=0; x<w; x++) {
	  string txt = a.cell(x,y);
	  xcurr = x;
	  ycurr = y;
	  add(txt,query);
	}
      }
    }
    if (vigor==1) {
      for (int y=0; y<h; y++) {
	if (asel.cell(0,y)==-1) {
	  for (int x=0; x<w-1; x++) {
	    string txt = a.cell(x,y);
	    txt += a.cell(x+1,y);
	    xcurr = x;
	    ycurr = y;
	    add(txt,query);
	  }
	}
      }
    }
    summarize(true);
  }

  void apply(CsvSheet& a, CsvSheet& b, IntSheet& asel, IntSheet& bsel) {
    rowMatch.resize(a.height(),b.height(),0);
    apply(a,asel,false);
    apply(b,bsel,true);
  }

  Stat flatten(IntSheet& sel) {
    int w = rowMatch.width();
    int h = rowMatch.height();
    double mean = 0;
    int ct = 0;
    for (int y=0; y<h; y++) {
      if (y<w) {
	float tmp = rowMatch.cell(0,y);
	rowMatch.cell(0,y) = rowMatch.cell(y,y);
	rowMatch.cell(y,y) = tmp;
	if (sel.cell(0,y)==-1) {
	  mean += rowMatch.cell(0,y);
	  ct++;
	}
      } else {
	rowMatch.cell(0,y) = 0;
      }
    }
    if (ct>0) { mean /= ct; }
    Stat s;
    s.mean = mean;
    s.stddev = 0;
    s.valid = (ct>10);
    printf("Mean is %g (count %d)\n", s.mean, ct);
    //Stat s1 = rowMatch.normalize(0,0,0.1,false);
    //Stat s2 = rowMatch.normalize(1,w);
    //printf("mean self-match %g +- %g\n", s1.mean, s1.stddev);
    //printf("mean non-match %g +- %g\n", s2.mean, s2.stddev);
    return s;
  }

  void summarize(bool force = false) {
    if (ct%100000==0 || force) {
      printf("%s %d features\n", query?"Queried":"Added",ct);
    }
  }

};


class ComparePass {
public:
  int vigor;
  IntSheet asel, bsel;

  void compare(CsvSheet& a, CsvSheet& b) {
    asel.resize(1,a.height(),-1);
    bsel.resize(1,b.height(),-1);
    int rem = -1;
    vigor = 0;
    for (int i=0; i<20; i++) {
      printf("\n\nPass %d\n", i);
      comparePass(a,b);
      int processed = 0;
      int remaining = 0;
      for (int j=0; j<bsel.height(); j++) {
	if (bsel.cell(0,j)==-1) {
	  remaining++;
	} else {
	  processed++;
	}
      }
      if (remaining == 0) {
	printf("Everything allocated\n");
	break;
      }
      if (rem==remaining) {
	printf("No progress\n");
	break;
	/*
	if (vigor==1) {
	  printf("Nothing left to try\n");
	  break;
	} else {
	  printf("Try with vigor\n");
	  vigor = 1;
	}
	*/
      }
      rem = remaining;
    }
  }

  void comparePass(CsvSheet& a, CsvSheet& b) {
    FeatMan feat, norm1, norm2;
    Stat astat, bstat;
    feat.setVigor(vigor);
    norm1.setVigor(vigor);
    norm2.setVigor(vigor);
    feat.apply(a,b,asel,bsel);
    norm1.apply(a,a,asel,asel);
    printf("Checking [local] statistics\n");
    astat = norm1.flatten(asel);
    norm2.apply(b,b,bsel,bsel);
    printf("Checking [remote] statistics\n");
    bstat = norm2.flatten(bsel);
    double scale = 1;
    if (bstat.valid && astat.valid) {
      if (bstat.mean>0.01) {
	scale = astat.mean/bstat.mean;
      }
      printf("Rescaling norm2 by %g\n", scale);
      norm2.rowMatch.rescale(scale);
    }
    //CsvFile::write(feat.rowMatch,"match.csv");
    //CsvFile::write(norm1.rowMatch,"norm1.csv");
    //CsvFile::write(norm2.rowMatch,"norm2.csv");
    
    FloatSheet match = feat.rowMatch;
    for (int y=0; y<match.height(); y++) {
      if (bsel.cell(0,y)==-1) {
	int bestIndex = -1;
	double bestValue = 0;
	double bestInc = 0;
	for (int x=0; x<match.width(); x++) {
	  double val = match.cell(x,y);
	  if (val>bestValue) {
	    bestIndex = x;
	    bestInc = val - bestValue;
	    bestValue = val;
	  }
	}
	double ref = norm2.rowMatch.cell(0,y);
	bool ok = false;
	if (bestValue>ref/4 ||
	    (bestValue>(bestValue-bestInc)*10 && bestValue>ref/8)) {
	  if (bestInc>bestValue/2 && bestIndex>=0) {
	    printf("%d->%d, remote row %d maps to local row %d (%d %g %g : %g)\n",
		   y,bestIndex,y,bestIndex,
		   bestIndex, bestValue, bestInc, ref);
	    printf("  [remote] %s\n", b.cell(0,y).c_str());
	    printf("  [local] %s\n", a.cell(0,bestIndex).c_str());
	    ok = true;
	    bsel.cell(0,y) = bestIndex;
	    if (asel.cell(0,bestIndex)!=-1 && asel.cell(0,bestIndex)!=-y) {
	      printf("IGNORING COLLISION!\n");
	      printf("This case has not been dealt with yet\n");
	    }
	    asel.cell(0,bestIndex) = y;
	  }
	}
	if (!ok) {
	  printf("%d->?, do not know what to make of remote row %d (%d %g %g : %g)\n",
		 y, y, bestIndex, bestValue, bestInc, ref);
	  printf("  [remote] %s\n", b.cell(0,y).c_str());
	  if (bestIndex>=0) {
	    printf("  [local] [MISS] %s\n", a.cell(0,bestIndex).c_str());
	  }
	}
      }
    }
  }
};

void CsvCompare::compare(CsvSheet& a, CsvSheet& b) {
  ComparePass pass;
  pass.compare(a,b);
  printf("Copying results\n");
  rows_b2a = pass.bsel;
  rows_a2b = pass.asel;
}


class RowUnit {
public:
  int pivotRow;
  int localRow;
  int remoteRow;

  RowUnit() {
    remoteRow = localRow = pivotRow = -1;
  }

  RowUnit(int pivot, int local, int remote) {
    pivotRow = pivot;
    localRow = local;
    remoteRow = remote;
  }
};


class RowOrder {
public:
  CsvCompare comp_local, comp_remote;
  list<RowUnit> rows;
  IntSheet xlocal, xremote;
  int start_local;
  int start_remote;
  CsvSheet result;

  void compare(CsvSheet& pivot, CsvSheet& local, CsvSheet& remote);

  void process(CsvSheet& pivot, CsvSheet& local, CsvSheet& remote,
	       int ilocal, int iremote,
	       int stop_local, int stop_remote);
};


void RowOrder::process(CsvSheet& pivot, CsvSheet& local, CsvSheet& remote,
		       int ilocal, int iremote,
		       int stop_local, int stop_remote) {
  while (true) {
    //printf("CsvCompare3 loop [local] %d [remote] %d\n", ilocal, iremote);
    if (ilocal>=stop_local &&
	iremote>=stop_remote) {
      break;
    }
    int _l = ilocal;
    if (ilocal<stop_local) {
      if (_l<local.height()) {
	if (xlocal.cell(0,_l)==0) {
	  int _lp = comp_local.b2a(_l);
	  if (_lp!=-1) {
	    int _lpr = comp_remote.a2b(_lp);
	    if (_lpr!=-1) {
	      process(pivot,local,remote,0,0,ilocal,_lpr);
	      printf("Local line %d exists in pivot at %d and in remote at %d\n", _l, _lp, _lpr);
	      rows.push_back(RowUnit(_lp,_l,_lpr));
	      xremote.cell(0,_lpr) = 1;
	      xlocal.cell(0,_l) = 1;
	    } else {
	      printf("Local line %d exists in pivot at %d, but not in remote - [DELETE]\n", _l, _lp);
	      xlocal.cell(0,_l) = 1;
	    }
	  } else {
	    printf("Local line %d not in pivot - [ADD]\n", _l);
	    rows.push_back(RowUnit(-1,_l,-1));
	    xlocal.cell(0,_l) = 1;
	  }
	}
      }
    } else {
      int _r = iremote;
      if (_r<remote.height()) {
	if (xremote.cell(0,_r)==0) {
	  int _rp = comp_remote.b2a(_r);
	  if (_rp!=-1) {
	    int _rpl = comp_local.a2b(_rp);
	    if (_rpl!=-1) {
	      // we will get this (assuming no collisions), skip...
	    } else {
	      // deleted locally
	    }
	  } else {
	    printf("Remote line %d not in pivot - [ADD]\n", _r);
	    rows.push_back(RowUnit(-1,-1,_r));
	    xremote.cell(0,_r) = 1;
	  }
	}
      }
    }
    if (ilocal<stop_local) {
      ilocal++;
    } else {
      iremote++;
    }
  }
}


void RowOrder::compare(CsvSheet& pivot, CsvSheet& local, CsvSheet& remote) {
  printf("==============================================\n");
  printf("= compare pivot and local\n");
  comp_local.compare(pivot,local);
  printf("==============================================\n");
  printf("= compare pivot and remote\n");
  comp_remote.compare(pivot,remote);
  printf("= pairwise comparisons done\n");
  printf("==============================================\n");
    
  xlocal.resize(1,local.height(),0);
  xremote.resize(1,remote.height(),0);

  start_local = 0;
  start_remote = 0;
  process(pivot,local,remote,0,0,local.height(),remote.height());

  // premature, but just for fun, lets generate a merge
  // (we haven't dealt with column manipulations yet though)
  for (list<RowUnit>::iterator it=rows.begin();
       it!=rows.end(); 
       it++) {
    RowUnit& unit = *it;
    int _l = unit.localRow;
    int _r = unit.remoteRow;
    if (_l!=-1&&_r!=-1) {
      bool ok = false;
      if (local.width()==remote.width()) {
	int i = 0;
	for (i=0; i<local.width(); i++) {
	  if (local.cell(i,_l)!=remote.cell(i,_r)) break;
	}
	if (i==local.width()) {
	  result.addField("[merge]");
	  result.addRow(local,_l);
	  ok = true;
	}
      }
      if (!ok) {
	result.addField("[local]");
	result.addRow(local,_l);
	result.addField("[remote]");
	result.addRow(remote,_r);
      }
    } else if (_l!=-1) {
      result.addField("[localADD]");
      result.addRow(local,_l);
    } else if (_r!=-1) {
      result.addField("[remoteADD]");
      result.addRow(remote,_r);
    }
  }
  //CsvFile::write(result,"result.csv");
}

void CsvCompare3::compare(CsvSheet& pivot, CsvSheet& local, CsvSheet& remote) {
  RowOrder order;
  order.compare(pivot,local,remote);
  cmp = order.result;
}
