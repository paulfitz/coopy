#include <stdio.h>
#include <string.h>

#include <coopy/Dbg.h>
#include <coopy/CsvSheet.h>
#include <coopy/GnumericSheet.h>

extern "C" {
#include "coopy/gnumeric_link.h"
}

using namespace coopy::store;
using namespace std;

// This code is just used in regression tests, so it is ok
// to be brain-dead-ed-ly inefficient
string ingest(const char *fname, string def) {
    FILE *fin1 = fopen(fname,"r");
    if (fin1==NULL) {
      fprintf(stderr,"Failed to open %s\n", fname);
      return def;
    }
    int ch = 0;
    string result = "";
    while (ch!=-1) {
      ch = fgetc(fin1);
      if (ch!=-1 && ch!='\r') {
	result += ch;
      }
    }
    fclose(fin1);
    return result;
}

int main(int argc, char *argv[]) {
  bool compare = false;
  while (argc>1) {
    if (string(argv[1])=="--verbose") {
      coopy_set_verbose(true);
      argc--;
      argv++;
    }
    else if (string(argv[1])=="--compare") {
      compare = true;
      argc--;
      argv++;
    } else {
      break;
    }
  }
  if (argc<3+compare?1:0) {
    printf("Call as:\n");
    printf("  ss2html [--verbose] src.xls dest.html\n");
    printf("  ss2html [--verbose] [--compare] src.xls dest.html ref.html\n");
    return 1;
  }
  const char *template_name = argv[1];
  const char *target_name = argv[2];

  gnumeric_init();
  GnumericWorkbookPtr book = gnumeric_load(template_name);
  gnumeric_save(book,target_name,"Gnumeric_html:html40frag");
  //gnumeric_save(book,target_name,"Gnumeric_html:html32");
  gnumeric_free(book);
  gnumeric_fini();

  if (compare) {
    string str1 = ingest(target_name,"1");
    string str2 = ingest(argv[3],"2");
    return (str1==str2)?0:1;
  }

  return 0;
}

