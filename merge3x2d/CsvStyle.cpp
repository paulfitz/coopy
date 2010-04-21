#include "CsvStyle.h"

#include <string>
using namespace std;

#include <ctype.h>

void CsvStyle::setFromFilename(const char *fname) {
  string name = fname;
  string ext = name.substr(name.length()-4);
  for (int i=0; i<ext.length(); i++) {
    ext[i] = tolower(ext[i]);
  }
  if (ext==".tsv") {
    delim = "\t";
  } else if (ext==".ssv") {
    delim = ";";
  } else if (ext==".csv") {
    delim = ",";
  } else {
    delim = ",";
  }
}

