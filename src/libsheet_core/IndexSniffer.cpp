#include <coopy/IndexSniffer.h>
#include <coopy/Dbg.h>
#include <coopy/EfficientMap.h>

using namespace coopy::store;
using namespace std;

void IndexSniffer::sniff() {

  int w = sheet.width();
  int h = sheet.height();
  len = w;
  guessed = true;

  vector<string> sofar;
  sofar.resize(h);
  for (int i=0; i<w; i++) {
    efficient_map<string,int> ct;
    int collide = 0;
    for (int j=0; j<h; j++) {
      bool escaped = false;
      string v = sheet.cellString(i,j,escaped);
      v += escaped?"*":" ";
      sofar[i] += v;
      if (ct.find(sofar[i])==ct.end()) {
	ct[sofar[i]] = 1;
      } else {
	collide++;
      }
    }
    if (collide==0) {
      dbg_printf("no collisions for %d\n", i);
      len = i+1;
      break;
    } else {
      dbg_printf("%d collisions for %d\n", collide, i);
    }
  }
  offsets.clear();
  if (len==0) len = w;
  for (int i=0; i<w; i++) {
    offsets.push_back((i<len)?1:0);
  }
}
