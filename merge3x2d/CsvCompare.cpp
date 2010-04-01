
#include "CsvCompare.h"
#include "CsvWrite.h"

#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>

//#include <map>
#include <list>

#include "OrderResult.h"

using namespace std;

#ifdef __GNUC__
#include <ext/hash_map>
using namespace __gnu_cxx;
#define map hash_map
//#include <unordered_map>
//#define map unordered_map
namespace __gnu_cxx {
  template <>
  struct hash<std::string> {
    size_t operator() (const std::string& x) const {
      return hash<const char*>()(x.c_str());
    }
  };
}
#else
#warning "Unfamiliar compiler, compiling without a hash map chosen - fix this"
#include <map>
#endif

////////////////////////////////////////////////////////////////////////
//
// Row/Column neutral code
//


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
    if (f.find(txt)==f.end()) {
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

  void add(string txt, bool query, int ctrl = 0) {
    this->query = query;
    int len = txt.length();
    for (int k=8-ctrl*2; k<10; k++) {
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

  static int getCtrlMax() { return 4; }

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
  virtual void measure(MeasurePass& pass, int ctrl=0) = 0;

  virtual int getCtrlMax() = 0;
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
    int ctrl = 0;
    int ctrlMax = main.getCtrlMax();
    for (int i=0; i<20; i++) {
      printf("\n\nPass %d\n", i);
      compare1(ctrl);
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
      printf("Not everything allocated, %d remain (a-total %d b-total %d)\n",
	     remaining,
	     main_pass.asel.height(),
	     main_pass.bsel.height());
      if (remaining<=(main_pass.bsel.height()-main_pass.asel.height())) {
	printf("No more could be allocated\n");
	break;
      }
      if (rem==remaining) {
	printf("No progress\n");
	if (ctrl<ctrlMax) {
	  printf("Increasing desperation\n");
	  ctrl++;
	} else {
	  break;
	}
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

  void compare1(int ctrl) {
    Stat astat, bstat;
    //feat.apply(a,b,asel,bsel);
    main.measure(main_pass,ctrl);
    //norm1.apply(a,a,asel,asel);
    anorm_pass.asel = main_pass.asel;
    anorm_pass.bsel = main_pass.asel;
    anorm.measure(anorm_pass,ctrl);
    printf("Checking [local] statistics\n");
    astat = anorm_pass.flatten();
    bnorm_pass.asel = main_pass.bsel;
    bnorm_pass.bsel = main_pass.bsel;
    bnorm.measure(bnorm_pass,ctrl);
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
	if (bestValue>ref/4) {// ||
	  //	    (bestValue>(bestValue-bestInc)*10 && bestValue>ref/8)) {
	  if (bestInc>bestValue/2 && bestIndex>=0) {
	    printf("%d->%d, remote unit %d maps to local unit %d (%d %g %g : %g)\n",
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


  virtual void measure(MeasurePass& pass, int ctrl) {
    int wa = pass.a.width();
    int wb = pass.b.width();
    int ha = pass.a.height();
    int hb = pass.b.height();

    printf("Column comparison\n");
    pass.clearMatch();
    int step = hb/pow(2,ctrl+4);
    if (step<1) step = 1;
    printf("Desperation %d, step size %d\n", ctrl, step);
    for (int rb=0; rb<hb; rb+=step) {
      int ra = comp.b2a(rb);
      if (ra!=-1) {
	FMap m(pass.match);
	int c = m.getCtrlMax();
	//m.resize(wa,wb);
	for (int i=0; i<wa; i++) {
	  m.setCurr(i,i);
	  m.add(pass.a.cell(i,ra),false,c);
	}
	for (int j=0; j<wb; j++) {
	  m.setCurr(j,j);
	  m.add(pass.b.cell(j,rb),true,c);
	}
      }
    }
  }

  virtual int getCtrlMax() {
    return 5;
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

  void apply(CsvSheet& a, IntSheet& asel, bool query, int ctrl) {
    int w = a.width();
    int h = a.height();
    m.resetCount();
    for (int y=0; y<h; y++) {
      if (asel.cell(0,y)==-1) {
	for (int x=0; x<w; x++) {
	  string txt = a.cell(x,y);
	  m.setCurr(x,y);
	  m.add(txt,query,ctrl);
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
	    m.add(txt,query,ctrl);
	  }
	}
      }
    }
    m.summarize(true);
  }

  void apply(CsvSheet& a, CsvSheet& b, IntSheet& asel, IntSheet& bsel,
	     int ctrl) {
    match.resize(a.height(),b.height(),0);
    apply(a,asel,false,ctrl);
    apply(b,bsel,true,ctrl);
  }

  virtual void measure(MeasurePass& pass, int ctrl) {
    apply(pass.a, pass.b, pass.asel, pass.bsel, ctrl);
    pass.match = match;
  }

  virtual int getCtrlMax() {
    return FMap::getCtrlMax();
  }
};




class MatchUnit {
public:
  int pivotUnit;
  int localUnit;
  int remoteUnit;

  MatchUnit() {
    remoteUnit = localUnit = pivotUnit = -1;
  }

  MatchUnit(int pivot, int local, int remote) {
    pivotUnit = pivot;
    localUnit = local;
    remoteUnit = remote;
  }
};


class OrderMerge {
public:
  OrderResult order_local, order_remote;
  list<MatchUnit> accum;
  IntSheet xlocal, xremote;
  int start_local;
  int start_remote;

  void process(int ilocal, int iremote,
	       int stop_local, int stop_remote);

  void merge(const OrderResult& nlocal,
	     const OrderResult& nremote) {
    order_local = nlocal;
    order_remote = nremote;
    xlocal.resize(1,order_local.blen(),0);
    xremote.resize(1,order_remote.blen(),0);
    start_local = 0;
    start_remote = 0;
    process(0,0,order_local.blen(),order_remote.blen());
  }
};

void OrderMerge::process(int ilocal, int iremote,
			 int stop_local, int stop_remote) {
  while (true) {
    if (ilocal>=stop_local &&
	iremote>=stop_remote) {
      break;
    }
    int _l = ilocal;
    if (ilocal<stop_local) {
      if (_l<xlocal.height()) {
	if (xlocal.cell(0,_l)==0) {
	  int _lp = order_local.b2a(_l);
	  if (_lp!=-1) {
	    int _lpr = order_remote.a2b(_lp);
	    if (_lpr!=-1) {
	      process(0,0,ilocal,_lpr);
	      printf("Local unit %d exists in pivot at %d and in remote at %d\n", _l, _lp, _lpr);
	      accum.push_back(MatchUnit(_lp,_l,_lpr));
	      xremote.cell(0,_lpr) = 1;
	      xlocal.cell(0,_l) = 1;
	    } else {
	      printf("Local unit %d exists in pivot at %d, but not in remote - [DELETE]\n", _l, _lp);
	      xlocal.cell(0,_l) = 1;
	    }
	  } else {
	    printf("Local unit %d not in pivot - [ADD]\n", _l);
	    accum.push_back(MatchUnit(-1,_l,-1));
	    xlocal.cell(0,_l) = 1;
	  }
	}
      }
    } else {
      int _r = iremote;
      if (_r<xremote.height()) {
	if (xremote.cell(0,_r)==0) {
	  int _rp = order_remote.b2a(_r);
	  if (_rp!=-1) {
	    int _rpl = order_local.a2b(_rp);
	    if (_rpl!=-1) {
	      // we will get this (assuming no collisions), skip...
	    } else {
	      // deleted locally
	    }
	  } else {
	    printf("Remote unit %d not in pivot - [ADD]\n", _r);
	    accum.push_back(MatchUnit(-1,-1,_r));
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



class Merger {
public:
  OrderMerge row_merge;
  OrderMerge col_merge;
  int conflicts;

  CsvSheet result;

  void merge(CsvSheet& pivot, CsvSheet& local, CsvSheet& remote,
	     const OrderResult& nrow_local,
	     const OrderResult& nrow_remote,
	     const OrderResult& ncol_local,
	     const OrderResult& ncol_remote);

  void mergeRow(CsvSheet& pivot, CsvSheet& local, CsvSheet& remote,
		int _p, int _l, int _r);

  void addRow(const char *tag,
	      const vector<string>& row,
	      const string& blank) {
    CsvSheet& target = result;
    target.addField(tag);
    for (int i=0; i<row.size(); i++) {
      if (row[i]!=blank) {
	target.addField(row[i].c_str());
      } else {
	target.addField("");
      }
    }
    target.addRecord();
  }

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

void Merger::mergeRow(CsvSheet& pivot, CsvSheet& local, CsvSheet& remote,
		     int pRow, int lRow, int rRow) {
  string blank = "__NOT_SET__CSVCOMPARE_SSFOSSIL";
  vector<string> expandLocal, expandRemote, expandPivot, expandMerge;
  for (list<MatchUnit>::iterator it=col_merge.accum.begin();
       it!=col_merge.accum.end(); 
       it++) {
    MatchUnit& unit = *it;
    int pCol = unit.pivotUnit;
    int lCol = unit.localUnit;
    int rCol = unit.remoteUnit;
    if (lRow!=-1 && lCol!=-1) {
      //printf("access local %d %d (size %d %d)\n", lCol, lRow, 
      //local.width(), local.height());
      expandLocal.push_back(local.cell(lCol,lRow));
    } else {
      expandLocal.push_back(blank);
    }
    if (rRow!=-1 && rCol!=-1) {
      //printf("access remote %d %d\n", rCol, rRow);
      expandRemote.push_back(remote.cell(rCol,rRow));
    } else {
      expandRemote.push_back(blank);
    }
    if (pRow!=-1 && pCol!=-1) {
      //printf("access pivot %d %d\n", pCol, pRow);
      expandPivot.push_back(pivot.cell(pCol,pRow));
    } else {
      expandPivot.push_back(blank);
    }
  }
  //printf("Onwards\n");
  bool conflict = false;
  expandMerge = expandLocal;
  for (int i=0; i<expandLocal.size(); i++) {
    string& _l = expandMerge[i];
    string& _r = expandRemote[i];
    string& _p = expandPivot[i];
    if (_l!=_r) {
      if (_l==blank) {
	_l = _r;
      } else {
	if (_r!=blank) {
	  // two assertions, do they conflict?
	  // if pivot is the same as either, then no.
	  if (_p==_l||_p==_r) {
	    if (_p==_l) { _l = _r; }
	  } else {
	    conflict = true;
	    break;
	  }
	}
      }
    }
  }

  if (conflict) {
    conflicts++;
    addRow("[local]",expandLocal,blank);
    addRow("[conflicting]",expandRemote,blank);
  } else {
    if (lRow!=-1 && rRow!=-1) {
      addRow("",expandMerge,blank);
    } else if (lRow!=-1) {
      addRow("",expandMerge,blank); // local add
    } else if (rRow!=-1) {
      addRow("[add]",expandMerge,blank); // remote add
    }
  }
}

void Merger::merge(CsvSheet& pivot, CsvSheet& local, CsvSheet& remote,
		   const OrderResult& row_local,
		   const OrderResult& row_remote,
		   const OrderResult& col_local,
		   const OrderResult& col_remote) {

  printf("Merging row order...\n");
  row_merge.merge(row_local,row_remote);

  printf("Merging column order...\n");
  col_merge.merge(col_local,col_remote);

  conflicts = 0;

  vector<string> header;
  for (list<MatchUnit>::iterator it=col_merge.accum.begin();
       it!=col_merge.accum.end(); 
       it++) {
    MatchUnit& unit = *it;
    int pCol = unit.localUnit;
    int lCol = unit.pivotUnit;
    int rCol = unit.remoteUnit;
    if (lCol!=-1&&rCol!=-1) {
      header.push_back("");
    } else if (lCol!=-1) {
      header.push_back(""); // local add
    } else if (rCol!=-1) {
      header.push_back("[add]"); // remote add
    }
  }
  addRow("[conflict]",header,"");

  for (list<MatchUnit>::iterator it=row_merge.accum.begin();
       it!=row_merge.accum.end(); 
       it++) {
    MatchUnit& unit = *it;
    int _l = unit.localUnit;
    int _p = unit.pivotUnit;
    int _r = unit.remoteUnit;
    mergeRow(pivot,local,remote,_p,_l,_r);
  }

  if (conflicts==0) {
    printf("No conflicts!\n");
    CsvSheet tmp = result;
    result.clear();
    for (int y=1; y<tmp.height(); y++) {
      for (int x=1; x<tmp.width(); x++) {
	result.addField(tmp.cell(x,y).c_str());
      }
      result.addRecord();
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
