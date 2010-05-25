#include <coopy/SheetStyle.h>

#include <ctype.h>

#include <string>

using namespace std;

const SheetStyle SheetStyle::defaultStyle;

void SheetStyle::setFromFilename(const char *fname) {
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
    }
  }
}

void SheetStyle::setFromInspection(const char *buffer, int len) {
  int comma_ct = 0;
  int comma_prev = -1;
  int tab_ct = 0;
  int tab_prev = -1;
  int semicolon_ct = 0;
  int semicolon_prev = -1;
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
	if (content) {
	  if (comma_prev==-1) comma_prev = comma_ct;
	  if (tab_prev==-1) tab_prev = tab_ct;
	  if (semicolon_prev==-1) semicolon_prev = semicolon_ct;
	  if (comma_prev!=comma_ct) comma_prev = -2;
	  if (tab_prev!=tab_ct) tab_prev = -2;
	  if (semicolon_prev!=semicolon_ct) comma_prev = -2;
	  //printf("comma %d tab %d semicolon %d\n", 
	  //comma_ct, tab_ct, semicolon_ct);
	  comma_ct = 0;
	  tab_ct = 0;
	  semicolon_ct = 0;
	  content = false;
	}
      } else if (ch==',') {
	comma_ct++;
      } else if (ch=='\t') {
	tab_ct++;
      } else if (ch==';') {
	semicolon_ct++;
      }
    }
  }
  if (comma_prev>0) {
    delim = ",";
  } else if (tab_prev>0) {
    delim = "\t";
  } else if (semicolon_prev>0) {
    delim = ";";
  } else {
    delim = ",";
  }
}

