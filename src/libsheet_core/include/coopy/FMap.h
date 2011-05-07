#ifndef COOPY_FMAP
#define COOPY_FMAP

#include <string>

#include <coopy/FVal.h>
#include <coopy/Dbg.h>
#include <coopy/EfficientMap.h>


namespace coopy {
  namespace cmp {
    typedef std::string Feature;
    template <class FVal> class FPolyMap;
    typedef FPolyMap<FSingleVal> FMap;
    typedef FPolyMap<FMultiVal> FMultiMap;
  }
}


template <class FVal>
class coopy::cmp::FPolyMap {
public:
  typedef efficient_map<Feature,FVal> Cache;
  int ct;
  int xcurr, ycurr;
  Cache f;
  coopy::store::SparseFloatSheet& rowMatch;
  bool query;

  FPolyMap(coopy::store::SparseFloatSheet& sheet) : rowMatch(sheet) {
    query = false;
    ct = 0;
  }

  coopy::store::FloatSheet& getMatch();

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
    /*
    if (f.find(txt)==f.end()) {
      // miss!
    } else {
      FVal& val = f[Feature(txt)];
      if (val.index>=0) {
	rowMatch.cell(val.index,ycurr)++;
      }
    }
    */
    typename Cache::iterator it = f.find(txt);
    if (it!=f.end()) {
      it->second.apply(rowMatch,ycurr);
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
    //if (ctrl==0) {
    if (query) {
      queryBit(txt);
    } else {
      addBit(txt);
    }
    //} else {
    if (ctrl!=0) {
      txt = std::string("^") + txt + "$";
      int len = txt.length();
      for (int k=8-ctrl*2; k<10; k++) {
	for (int i=0; i<len-k; i++){
	  std::string part;
	  std::string low;
	  part = txt.substr(i,k+1);
	  low = part;
	  for (size_t c=0; c<low.length(); c++) {
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

