#ifndef COOPY_FMAP
#define COOPY_FMAP

#include <string>

#include <coopy/FVal.h>
#include <coopy/Dbg.h>
#include <coopy/EfficientMap.h>

typedef std::string Feature;

class FMap {
public:
  int ct;
  int xcurr, ycurr;
  efficient_map<Feature,FVal> f;
  SparseFloatSheet& rowMatch;
  bool query;

  FMap(SparseFloatSheet& sheet) : rowMatch(sheet) {
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

  void queryBit(std::string txt) {
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

  void addBit(std::string txt) {
    if (f.find(Feature(txt))==f.end()) {
      f[Feature(txt)] = FVal();
    }
    FVal& val = f[Feature(txt)];
    //val.index.insert(ycurr);
    val.setIndex(ycurr);
    ct++;
    summarize();
  }

  void add(std::string txt, bool query, int ctrl = 0) {
    this->query = query;
    if (ctrl==0) {
      if (query) {
	queryBit(txt);
      } else {
	addBit(txt);
      }
    } else {
      int len = txt.length();
      for (int k=8-ctrl*2; k<10; k++) {
	for (int i=0; i<len-k; i++){
	  std::string part;
	  std::string low;
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
  }

  static int getCtrlMax() { return 4; }

  void summarize(bool force = false) {
    if (ct%10000==0 || force) {
      dbg_printf("%s %d features\n", query?"Queried":"Added",ct);
    }
  }

};


#endif

