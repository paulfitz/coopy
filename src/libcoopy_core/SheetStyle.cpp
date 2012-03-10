#include <coopy/SheetStyle.h>

#include <ctype.h>

#include <string>
#include <map>

using namespace std;
using namespace coopy::store;

const SheetStyle SheetStyle::defaultStyle;

bool SheetStyle::setFromFilename(const char *fname) {
  string name = fname;
  delim = ",";
  if (name.length()>=4) {
    string ext = name.substr(name.length()-4);
    for (size_t i=0; i<ext.length(); i++) {
      ext[i] = tolower(ext[i]);
    }
    if (ext==".tsv") {
      delim = "\t";
    } else if (ext==".ssv") {
      delim = ";";
    } else if (ext==".csv") {
      delim = ",";
    } else if (ext==".wsv") {
      delim = " ";
    } else if (ext==".list") {
      delim = ",";
    } else {
      return false;
    }
  } else {
    return false;
  }
  return true;
}

void SheetStyle::setFromProperty(const Property& config) {
  if (config.check("delimiter")) {
    delim = config.get("delimiter").asString();
  }
  if (config.check("null_token")) {
    nullToken = config.get("null_token").asString();
  }
  if (config.check("have_null")) {
    haveNull = config.get("have_null").asInt()!=0;
  }
  if (config.check("avoid_collision")) {
    quoteCollision = config.get("avoid_collision").asInt()!=0;
  }
  if (config.check("mark_header")) {
    markHeader = config.get("mark_header").asInt()!=0;
  }  
}

class SeparatorHistory {
public:
  int ct;
  int best_ct;
  int best;
  map<int,int> votes;
  char ch;

  SeparatorHistory(char ch) : ch(ch) {
    ct = 0;
    best = -1;
    best_ct = 0;
  }

  void bump() {
    ct++;
  }

  void add() {
    if (votes.find(ct)==votes.end()) {
      votes[ct] = 1;
    }
    votes[ct]++;
    if (votes[ct]>best_ct) {
      best_ct = votes[ct];
      best = ct;
    }
    ct = 0;
  }
};

#define SEPARATOR_COMMA 0
#define SEPARATOR_TAB 1
#define SEPARATOR_SEMICOLON 2
#define SEPARATOR_COUNT 3

void SheetStyle::setFromInspection(const char *buffer, int len) {
  SeparatorHistory history[SEPARATOR_COUNT] = { ',', '\t', ';' };
  int rows = 0;
  bool quoted = false;
  bool content = false;
  for (int i=0; i<=len; i++) {
    char ch = '\n';
    if (i<len) {
      ch = buffer[i];
    }
    if (ch=='\"') {
      quoted = !quoted;
    } else if (!quoted) {
      if (ch!='\n'&&ch!='\r') {
	content = true;
      }
      if (ch=='\n') {
	rows++;
	if (content) {
	  for (int j=0; j<SEPARATOR_COUNT; j++) {
	    history[j].add();
	  }
	  content = false;
	}
      } else {
	for (int j=0; j<SEPARATOR_COUNT; j++) {
	  if (ch==history[j].ch) {
	    history[j].bump();
	  }
	}
      }
    }
  }

  delim = ",";
  int best_ct = 0;
  for (int j=0; j<SEPARATOR_COUNT; j++) {
    if (history[j].best!=0) {
      if (history[j].best_ct>best_ct) {
	best_ct = history[j].best_ct;
	delim = string() + history[j].ch;
      }
    }
  }
}

