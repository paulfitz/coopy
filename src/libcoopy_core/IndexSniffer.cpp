#include <coopy/IndexSniffer.h>
#include <coopy/Dbg.h>
#include <coopy/EfficientMap.h>

using namespace coopy::store;
using namespace std;

void IndexSniffer::sniff() {

  int w = sheet.width();
  int h = sheet.height();
  int len = w;
  guessed = true;

  if (!cflags.bias_ids) {
    if (sniffer.hasSubset()) {
      vector<int> keys = sniffer.getSubset();
      flags.clear();
      for (int i=0; i<w; i++) {
	flags.push_back(0);
      }
      for (int i=0; i<(int)keys.size(); i++) {
	flags[keys[i]] = 1;
      }
      guessed = false;
      return;
    }
  }

  SheetSchema *schema = sheet.getSchema();
  if (schema!=NULL) {
    if (schema->providesPrimaryKeys()) {
      flags.clear();
      len = w;
      bool got_something = false;
      for (int i=0; i<w; i++) {
	ColumnInfo info = schema->getColumnInfo(i);
	flags.push_back(info.isPrimaryKey()?1:0);
	got_something = got_something || info.isPrimaryKey();
      }
      if (got_something) {
	guessed = false;
	return;
      } else {
	flags.clear();
      }
    }
  }

  // no useful schema? on to guesswork.
  vector<string> sofar;
  sofar.resize(h);
  for (int i=0; i<w; i++) {
    efficient_map<string,int> ct;
    int collide = 0;
    for (int j=0; j<h; j++) {
      bool escaped = false;
      string v = sheet.cellString(i,j,escaped);
      v += escaped?"*":" ";
      sofar[j] += v;
      //dbg_printf("checking %d %s\n", j, sofar[j].c_str());
      if (ct.find(sofar[j])==ct.end()) {
	ct[sofar[j]] = 1;
      } else {
	collide++;
	if (collide==1) {
	  dbg_printf("first collision is for %s\n", v.c_str());
	}
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
  flags.clear();
  if (len==0) len = w;
  for (int i=0; i<w; i++) {
    flags.push_back((i<len)?1:0);
  }
}
