
#include "CsvCompare.h"
#include "CsvWrite.h"

#include <map>
#include <set>

using namespace std;



// used under GPL 2.0, see:
// http://www.azillionmonkeys.com/qed/hash.html

#include <stdint.h> /* Replace with <stdint.h> if appropriate */
#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                       +(uint32_t)(((const uint8_t *)(d))[0]) )
#endif

uint32_t SuperFastHash (const char * data, int len) {
uint32_t hash = len, tmp;
int rem;

    if (len <= 0 || data == NULL) return 0;

    rem = len & 3;
    len >>= 2;

    /* Main loop */
    for (;len > 0; len--) {
        hash  += get16bits (data);
        tmp    = (get16bits (data+2) << 11) ^ hash;
        hash   = (hash << 16) ^ tmp;
        data  += 2*sizeof (uint16_t);
        hash  += hash >> 11;
    }

    /* Handle end cases */
    switch (rem) {
        case 3: hash += get16bits (data);
                hash ^= hash << 16;
                hash ^= data[sizeof (uint16_t)] << 18;
                hash += hash >> 11;
                break;
        case 2: hash += get16bits (data);
                hash ^= hash << 11;
                hash += hash >> 17;
                break;
        case 1: hash += *data;
                hash ^= hash << 10;
                hash += hash >> 1;
    }

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}

// end use



/*
class Feature {
public:
  string str;

  Feature() {
    str = "";
  }

  bool operator==(const Feature& alt) {
    return str==alt.str;
  }

  bool operator!=(const Feature& alt) {
    return !((*this)==alt);
  }
};

class FeatureHash {
public:
  size_t operator() (const Feature& f) {
    return SuperFastHash(f.str.c_str(),f.str.length());
  }
};
*/

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

  map<Feature,FVal> f;

  FeatMan() {
    query = false;
  }

  void queryBit(string txt) {
    /*
    if (f.find(Feature(txt))==f.end()) {
      // miss!
    } else {
      FVal& val = f[Feature(txt)];
      if (val.index.size()!=1) {
	// non-unique match
      } else {
	// match!
      for (set<int>::iterator it = val.index.begin();
	   it != val.index.end();
	   it++) {
	//printf("Relate %d -> %d\n", ycurr, (*it));
	rowMatch.cell((*it),ycurr)++;
      }
      //}
    }
  */
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
	part = txt.substr(i,k+1);
	if (query) {
	  queryBit(part);
	} else {
	  addBit(part);
	}
      }
    }
  }

  void apply(CsvSheet& a, bool query) {
    this->query = query;
    int w = a.width();
    int h = a.height();
    ct = 0;
    for (int y=0; y<h; y++) {
      for (int x=0; x<w; x++) {
	string txt = a.cell(x,y);
	xcurr = x;
	ycurr = y;
	add(txt,query);
      }
    }
    summarize(true);
  }

  void summarize(bool force = false) {
    if (ct%100000==0 || force) {
      printf("%s %d features\n", query?"Queried":"Added",ct);
    }
  }

};


void CsvCompare::compare(CsvSheet& a, CsvSheet& b) {
  FeatMan feat;
  feat.rowMatch.resize(a.height(),b.height(),0);
  feat.apply(a,false);
  feat.apply(b,true);
  CsvFile::write(feat.rowMatch,"match.csv");
}


