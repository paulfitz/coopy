#include <stdio.h>
#include <string.h>

#include <coopy/Dbg.h>
#include <coopy/CsvSheet.h>
#include <coopy/GnumericSheet.h>
#include <coopy/Stringer.h>

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

#include <coopy/unimain.h>
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

  if (!book) exit(1);

  FILE *fout = fopen(target_name,"w");
  if (!fout) exit(1);

  int len = gnumeric_get_sheet_count(book);
  for (int k=0; k<len; k++) {
    GnumericSheetPtr s = gnumeric_get_sheet(book,k);
    if (!s) exit(1);
    fprintf(fout,"<h2>%s</h2>\n",gnumeric_sheet_get_name(s));
    int w, h;
    gnumeric_sheet_get_size(s,&w,&h);
    fprintf(fout,"<table>\n");
    for (int y=0; y<h; y++) {
      fprintf(fout,"  <tr>");
      for (int x=0; x<w; x++) {
	fprintf(fout,"<td>");
	int r, g, b;
	gnumeric_sheet_get_cell_font_color(s,x,y,&r,&g,&b);
	bool color = false;
	if (r!=0||g!=0||b!=0) {
	  color = true;
	  fprintf(fout,"<font color='#%02x%02x%02x'>",
		  r,g,b);
	}
	char *str = gnumeric_sheet_get_cell_as_string(s,x,y);
	if (str) {
	  fprintf(fout,"%s",str);
	  gnumeric_free_string(str);
	} else {
	  fprintf(fout,"");
	}
	if (color) {
	  fprintf(fout,"</font>");
	}
	fprintf(fout,"</td>");
      }
      fprintf(fout,"</tr>\n");
    }
    fprintf(fout,"</table>\n");
  }

  fclose(fout);
  fout = NULL;

  // Exporter is insufficiently stable for using in tests
  // gnumeric_save(book,target_name,"Gnumeric_html:html40frag");


  gnumeric_free(book);
  gnumeric_fini();

  if (compare) {
    string str1 = ingest(target_name,"1");
    string str2 = ingest(argv[3],"2");
    return (str1==str2)?0:1;
  }

  return 0;
}

