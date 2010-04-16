#include <stdio.h>
#include "CsvSheet.h"
#include "CsvFile.h"

#include <string>
using namespace std;

static void replace(string& str, const string& old, const string& rep) {
  int pos = 0;
  while((pos = str.find(old, pos)) != std::string::npos) {
    str.replace(pos, old.length(), rep);
    pos += rep.length();
  }
}

// assume text is in, and should remain, utf8
string ml_encode(string x) {
  replace(x,"&amp;","&");
  replace(x,"&lt;","<");
  replace(x,"&rt;",">");
  replace(x,"&","&amp;");
  replace(x,"<","&lt;");
  replace(x,">","&rt;");
  return x;
}

int main(int argc, char *argv[]) {
  const char *fname = "-";
  if (argc>1) {
    fname = argv[1];
  }
  CsvSheet sheet;
  if (CsvFile::read(fname,sheet)!=0) {
    printf("Failed to read input\n");
    return 1;
  }

  if (sheet.width()>=1 && sheet.height()>=1) {
    printf("<table class=\"csvtable\">\n");
    for (int i=0; i<sheet.height(); i++) {
      printf("  <tr>");
      for (int j=0; j<sheet.width(); j++) {
	printf(" <td>%s</td>", ml_encode(sheet.cell(j,i)).c_str());
      }
      printf(" </tr>\n");
    }
    printf("</table>\n");
  }

  return 0;
}
