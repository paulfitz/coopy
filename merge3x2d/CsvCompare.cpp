
#include "CsvCompare.h"
#include "CsvWrite.h"

#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>

#include <map>
#include <list>

using namespace std;


////////////////////////////////////////////////////////////////////////
//
// Row/Column neutral code
//

class OrderResult {
private:
  IntSheet _a2b, _b2a;
public:

  void setup(IntSheet& _a2b, IntSheet& _b2a) {
    this->_a2b = _a2b;
    this->_b2a = _b2a;
  }

  virtual int a2b(int x) const {
    return _a2b.cell(0,x);
  }

  virtual int b2a(int x) const {
    return _b2a.cell(0,x);
  }
};

class IdentityOrderResult : public OrderResult {
public:
  virtual int a2b(int x) const {
    return x;
  }

  virtual int b2a(int x) const {
    return x;
  }
};


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

class FMap {
public:
  int ct;
  int xcurr, ycurr;
  map<Feature,FVal> f;
  FloatSheet& rowMatch;
  bool query;

  FMap(FloatSheet& sheet) : rowMatch(sheet) {
    query = false;
    ct = 0;
  }

  FloatSheet& getMatch();

  //void setSize(int w, int h) {
  //rowMatch.resize(w,h,0);
  //  }

  void setCurr(int x, int y) {
    xcurr = x;
    ycurr = y;
  }

  void resetCount() {
    ct = 0;
    query = -1;
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
    this->query = query;
    int len = txt.length();
    for (int k=0; k<10; k++) {
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
    return s;
  }

  void summarize(bool force = false) {
    if (ct%100000==0 || force) {
      printf("%s %d features\n", query?"Queried":"Added",ct);
    }
  }

};


class MeasurePass {
public:
  CsvSheet& a;
  CsvSheet& b;
  IntSheet asel, bsel;
  FloatSheet match;

  MeasurePass(CsvSheet& a, CsvSheet& b) : a(a), b(b) {
  }

  void setSize(int alen, int blen) {
    asel.resize(1,alen,-1);
    bsel.resize(1,blen,-1);
    match.resize(alen,blen,0);
  }

  void clearMatch() {
    match.resize(match.width(),match.height(),0);
  }

  Stat flatten() {
    int w = match.width();
    int h = match.height();
    double mean = 0;
    int ct = 0;
    for (int y=0; y<h; y++) {
      if (y<w) {
	float tmp = match.cell(0,y);
	match.cell(0,y) = match.cell(y,y);
	match.cell(y,y) = tmp;
	if (asel.cell(0,y)==-1) {
	  mean += match.cell(0,y);
	  ct++;
	}
      } else {
	match.cell(0,y) = 0;
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

  OrderResult getOrder() {
    OrderResult order;
    order.setup(asel,bsel);
    return order;
  }
};

class Measure {
public:
  virtual void setup(MeasurePass& pass) = 0;
  virtual void measure(MeasurePass& pass) = 0;
};


class MeasureMan {
public:
  Measure& main;
  Measure& anorm;
  Measure& bnorm;
  MeasurePass& main_pass;
  MeasurePass& anorm_pass;
  MeasurePass& bnorm_pass;
  bool rowLike;

  MeasureMan(Measure& main,
	     MeasurePass& main_pass,
	     Measure& anorm,
	     MeasurePass& anorm_pass,
	     Measure& bnorm,
	     MeasurePass& bnorm_pass,
	     bool rowLike) : main(main), 
			     main_pass(main_pass),
			     anorm(anorm), 
			     anorm_pass(anorm_pass),
			     bnorm(bnorm),
			     bnorm_pass(bnorm_pass),
			     rowLike(rowLike) // just for debugging
  {
  }
	     
  void compare() {
    main.setup(main_pass);
    anorm.setup(anorm_pass);
    bnorm.setup(bnorm_pass);

    int rem = -1;
    for (int i=0; i<20; i++) {
      printf("\n\nPass %d\n", i);
      compare1();
      //comparePass(a,b);
      int processed = 0;
      int remaining = 0;
      for (int j=0; j<main_pass.bsel.height(); j++) {
	if (main_pass.bsel.cell(0,j)==-1) {
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

  string cell(CsvSheet& a, int x, int y) {
    if (rowLike) {
      return a.cell(x,y);
    }
    return a.cell(y,x);
  }

  void compare1() {
    Stat astat, bstat;
    //feat.apply(a,b,asel,bsel);
    main.measure(main_pass);
    //norm1.apply(a,a,asel,asel);
    anorm_pass.asel = main_pass.asel;
    anorm_pass.bsel = main_pass.asel;
    anorm.measure(anorm_pass);
    printf("Checking [local] statistics\n");
    astat = anorm_pass.flatten();
    bnorm_pass.asel = main_pass.bsel;
    bnorm_pass.bsel = main_pass.bsel;
    bnorm.measure(bnorm_pass);
    printf("Checking [remote] statistics\n");
    bstat = bnorm_pass.flatten();
    double scale = 1;
    if (bstat.valid && astat.valid) {
      if (bstat.mean>0.01) {
	scale = astat.mean/bstat.mean;
      }
      printf("Rescaling bnorm by %g\n", scale);
      bnorm_pass.match.rescale(scale);
    }
    //CsvFile::write(feat.rowMatch,"match.csv");
    //CsvFile::write(norm1.rowMatch,"norm1.csv");
    //CsvFile::write(norm2.rowMatch,"norm2.csv");
    
    FloatSheet match = main_pass.match;
    IntSheet& asel = main_pass.asel;
    IntSheet& bsel = main_pass.bsel;
    CsvSheet& a = main_pass.a;
    CsvSheet& b = main_pass.b;
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
	double ref = bnorm_pass.match.cell(0,y);
	bool ok = false;
	if (bestValue>ref/4 ||
	    (bestValue>(bestValue-bestInc)*10 && bestValue>ref/8)) {
	  if (bestInc>bestValue/2 && bestIndex>=0) {
	    printf("%d->%d, remote row %d maps to local unit %d (%d %g %g : %g)\n",
		   y,bestIndex,y,bestIndex,
		   bestIndex, bestValue, bestInc, ref);
	    printf("  [remote] %s\n", cell(b,0,y).c_str());
	    printf("  [local] %s\n", cell(a,0,bestIndex).c_str());
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
	  printf("%d->?, do not know what to make of remote unit %d (%d %g %g : %g)\n",
		 y, y, bestIndex, bestValue, bestInc, ref);
	  printf("  [remote] %s\n", cell(b,0,y).c_str());
	  if (bestIndex>=0) {
	    printf("  [local] [MISS] %s\n", cell(a,0,bestIndex).c_str());
	  }
	}
      }
    }
  }

};



////////////////////////////////////////////////////////////////////////
//
// Column specific code
//


class ColMan : public Measure {
public:
  const OrderResult& comp;

  ColMan(const OrderResult& comp) : comp(comp) {
  }

  virtual void setup(MeasurePass& pass) {
    pass.setSize(pass.a.width(),pass.b.width());
  }


  virtual void measure(MeasurePass& pass) {
    int wa = pass.a.width();
    int wb = pass.b.width();
    int ha = pass.a.height();
    int hb = pass.b.height();

    printf("Column comparison\n");
    pass.clearMatch();
    for (int rb=0; rb<hb; rb++) {
      int ra = comp.b2a(rb);
      if (ra!=-1) {
	FMap m(pass.match);
	//m.resize(wa,wb);
	for (int i=0; i<wa; i++) {
	  m.setCurr(i,i);
	  m.add(pass.a.cell(i,ra),false);
	}
	for (int j=0; j<wb; j++) {
	  m.setCurr(j,j);
	  m.add(pass.b.cell(j,rb),true);
	}
      }
    }
  }

};



////////////////////////////////////////////////////////////////////////
//
// Row specific code
//


class RowMan : public Measure {
public:
  int vigor;
  FMap m;
  FloatSheet match;

  RowMan() : m(match) {
    vigor = 0;
  }

  void setVigor(int vigor) {
    this->vigor = vigor;
  }

  virtual void setup(MeasurePass& pass) {
    pass.setSize(pass.a.height(),pass.b.height());
  }

  void apply(CsvSheet& a, IntSheet& asel, bool query) {
    int w = a.width();
    int h = a.height();
    m.resetCount();
    for (int y=0; y<h; y++) {
      if (asel.cell(0,y)==-1) {
	for (int x=0; x<w; x++) {
	  string txt = a.cell(x,y);
	  m.setCurr(x,y);
	  m.add(txt,query);
	}
      }
    }
    if (vigor==1) {
      for (int y=0; y<h; y++) {
	if (asel.cell(0,y)==-1) {
	  for (int x=0; x<w-1; x++) {
	    string txt = a.cell(x,y);
	    txt += a.cell(x+1,y);
	    m.setCurr(x,y);
	    m.add(txt,query);
	  }
	}
      }
    }
    m.summarize(true);
  }

  void apply(CsvSheet& a, CsvSheet& b, IntSheet& asel, IntSheet& bsel) {
    match.resize(a.height(),b.height(),0);
    apply(a,asel,false);
    apply(b,bsel,true);
  }

  virtual void measure(MeasurePass& pass) {
    apply(pass.a, pass.b, pass.asel, pass.bsel);
    pass.match = match;
  }
};




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


class Merger {
public:
  OrderResult row_local, row_remote;
  OrderResult col_local, col_remote;
  list<RowUnit> rows;
  IntSheet xlocal, xremote;
  int start_local;
  int start_remote;

  /*
  IntSheet col_xlocal, col_xremote;
  int col_start_local;
  int col_start_remote;
  */

  CsvSheet result;

  void merge(CsvSheet& pivot, CsvSheet& local, CsvSheet& remote,
	     const OrderResult& nrow_local,
	     const OrderResult& nrow_remote,
	     const OrderResult& ncol_local,
	     const OrderResult& ncol_remote);

  void process(CsvSheet& pivot, CsvSheet& local, CsvSheet& remote,
	       int ilocal, int iremote,
	       int stop_local, int stop_remote);

  void addRow(CsvSheet& target,
	      const char *tag,
	      CsvSheet& src,
	      int index,
	      int len) {
    target.addField(tag);
    for (int i=0; i<len; i++) {
      if (i<src.width()) {
	target.addField(src.cell(i,index).c_str());
      } else {
	target.addField("");
      }
    }
    target.addRecord();
  }
};


void Merger::process(CsvSheet& pivot, CsvSheet& local, CsvSheet& remote,
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
	  int _lp = row_local.b2a(_l);
	  if (_lp!=-1) {
	    int _lpr = row_remote.a2b(_lp);
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
	  int _rp = row_remote.b2a(_r);
	  if (_rp!=-1) {
	    int _rpl = row_local.a2b(_rp);
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


void Merger::merge(CsvSheet& pivot, CsvSheet& local, CsvSheet& remote,
		   const OrderResult& nrow_local,
		   const OrderResult& nrow_remote,
		   const OrderResult& ncol_local,
		   const OrderResult& ncol_remote) {

  row_local = nrow_local;
  row_remote = nrow_remote;
    
  xlocal.resize(1,local.height(),0);
  xremote.resize(1,remote.height(),0);

  start_local = 0;
  start_remote = 0;
  process(pivot,local,remote,0,0,local.height(),remote.height());

  printf("Merging...\n");

  // premature, but just for fun, lets generate a merge
  // (we haven't dealt with column manipulations yet though)
  for (list<RowUnit>::iterator it=rows.begin();
       it!=rows.end(); 
       it++) {
    RowUnit& unit = *it;
    int _l = unit.localRow;
    int _r = unit.remoteRow;
    int len = 10;
    if (_l!=-1&&_r!=-1) {
      bool ok = false;
      if (local.width()==remote.width()) {
	int i = 0;
	for (i=0; i<local.width(); i++) {
	  if (local.cell(i,_l)!=remote.cell(i,_r)) break;
	}
	if (i==local.width()) {
	  addRow(result,"[merge]",local,_l,len);
	  ok = true;
	}
      }
      if (!ok) {
	addRow(result,"[local]",local,_l,len);
	addRow(result,"[remote]",remote,_r,len);
      }
    } else if (_l!=-1) {
      addRow(result,"[localAdd]",local,_l,len);
    } else if (_r!=-1) {
      addRow(result,"[remoteADD]",remote,_r,len);
    }
  }
  printf("Got merged result (%dx%d)\n", result.width(), result.height());
  //CsvFile::write(result,"result.csv");
}



void CsvCompare::compare(CsvSheet& local, CsvSheet& remote) {
  printf("Two-way compare no longer implemented\n");
  exit(1);
}

void CsvCompare3::compare(CsvSheet& pivot, CsvSheet& local, CsvSheet& remote) {
  IdentityOrderResult id;

  //RowOrder order;
  //order.compare(pivot,local,remote);

  // Prepare for p2l row comparison
  MeasurePass p2l_row_pass_local(pivot,local);
  MeasurePass p2l_row_pass_norm1(pivot,pivot);
  MeasurePass p2l_row_pass_norm2(local,local);

  RowMan p2l_row_local;
  RowMan p2l_row_norm1;
  RowMan p2l_row_norm2;

  MeasureMan p2l_row_man(p2l_row_local,p2l_row_pass_local,
			 p2l_row_norm1,p2l_row_pass_norm1,
			 p2l_row_norm2,p2l_row_pass_norm2,
			 1);

  // Compare p2l rows
  p2l_row_man.compare();


  // Prepare for p2r row comparison
  MeasurePass p2r_row_pass_local(pivot,remote);
  MeasurePass p2r_row_pass_norm1(pivot,pivot);
  MeasurePass p2r_row_pass_norm2(remote,remote);

  RowMan p2r_row_local;
  RowMan p2r_row_norm1;
  RowMan p2r_row_norm2;

  MeasureMan p2r_row_man(p2r_row_local,p2r_row_pass_local,
			 p2r_row_norm1,p2r_row_pass_norm1,
			 p2r_row_norm2,p2r_row_pass_norm2,
			 1);

  p2r_row_man.compare();

  // Now, column comparison

  MeasurePass p2l_col_pass_local(pivot,local);
  MeasurePass p2l_col_pass_norm1(pivot,pivot);
  MeasurePass p2l_col_pass_norm2(local,local);

  OrderResult p2l_row_order = p2l_row_pass_local.getOrder();
  ColMan p2l_col_local(p2l_row_order);
  ColMan p2l_col_norm1(id);
  ColMan p2l_col_norm2(id);

  MeasureMan p2l_col_man(p2l_col_local,p2l_col_pass_local,
			 p2l_col_norm1,p2l_col_pass_norm1,
			 p2l_col_norm2,p2l_col_pass_norm2,
			 0);

  p2l_col_man.compare();

  MeasurePass p2r_col_pass_local(pivot,remote);
  MeasurePass p2r_col_pass_norm1(pivot,pivot);
  MeasurePass p2r_col_pass_norm2(remote,remote);

  OrderResult p2r_row_order = p2r_row_pass_local.getOrder();
  ColMan p2r_col_local(p2r_row_order);
  ColMan p2r_col_norm1(id);
  ColMan p2r_col_norm2(id);

  MeasureMan p2r_col_man(p2r_col_local,p2r_col_pass_local,
			 p2r_col_norm1,p2r_col_pass_norm1,
			 p2r_col_norm2,p2r_col_pass_norm2,
			 0);

  p2r_col_man.compare();


  OrderResult p2l_col_order = p2l_col_pass_local.getOrder();
  OrderResult p2r_col_order = p2r_col_pass_local.getOrder();

  Merger merger;
  merger.merge(pivot,local,remote,
	       p2l_row_order,
	       p2r_row_order,
	       p2l_col_order,
	       p2r_col_order);

  cmp = merger.result;
}
