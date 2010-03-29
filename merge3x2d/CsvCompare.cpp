
#include "CsvCompare.h"
#include "CsvWrite.h"

#include <ctype.h>

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
	if (sel.cell(0,y)!=-1) {
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
    s.valid = true;
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
  IntSheet asel, bsel;

  void compare(CsvSheet& a, CsvSheet& b) {
    asel.resize(1,a.height(),-1);
    bsel.resize(1,b.height(),-1);
    int rem = -1;
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
      }
      rem = remaining;
    }
  }

  void comparePass(CsvSheet& a, CsvSheet& b) {
    FeatMan feat, norm1, norm2;
    Stat astat, bstat;
    feat.apply(a,b,asel,bsel);
    norm1.apply(a,a,asel,asel);
    astat = norm1.flatten(asel);
    norm2.apply(b,b,bsel,bsel);
    bstat = norm2.flatten(bsel);
    double scale = 1;
    if (bstat.mean>0.01) {
      scale = astat.mean/bstat.mean;
    }
    printf("Rescaling norm2 by %g\n", scale);
    norm2.rowMatch.rescale(scale);
    CsvFile::write(feat.rowMatch,"match.csv");
    CsvFile::write(norm1.rowMatch,"norm1.csv");
    CsvFile::write(norm2.rowMatch,"norm2.csv");
    
    FloatSheet match = feat.rowMatch;
    for (int y=0; y<match.height(); y++) {
      if (bsel.cell(0,y)==-1) {
	int bestIndex = y;
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
	if (bestValue>ref/4) {
	  if (bestInc>bestValue/2) {
	    printf("%d->%d, remote row %d maps to local row %d\n",
		   y,bestIndex,y,bestIndex);
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
	}
      }
    }
  }
};

void CsvCompare::compare(CsvSheet& a, CsvSheet& b) {
  ComparePass pass;
  pass.compare(a,b);
}


