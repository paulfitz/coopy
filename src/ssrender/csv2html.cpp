#include <stdio.h>
#include <getopt.h>

#include <coopy/Dbg.h>
#include <coopy/CsvRender.h>
#include <coopy/PolyBook.h>

using namespace std;
using namespace coopy::store;

int main(int argc, char *argv[]) {
  bool verbose = false;
  bool decorate = false;
  bool full = false;

  while (true) {
    int option_index = 0;
    static struct option long_options[] = {
      {"full", 0, 0, 'f'},
      {"decorate", 0, 0, 'd'},
      {"verbose", 0, 0, 'v'},
      {0, 0, 0, 0}
    };

    int c = getopt_long(argc, argv, "",
			long_options, &option_index);
    if (c==-1) break;
    switch (c) {
    case 'v':
      verbose = true;
      break;
    case 'f':
      full = true;
      decorate = true;
      break;
    case 'd':
      decorate = true;
      break;

    default:
      fprintf(stderr, "Unrecognized option\n");
      return 1;
    }
  }

  if (optind<argc-1) {
    fprintf(stderr, "Options not understood\n");
    return 1;
  }
  argc -= optind;
  argv += optind;

  if (argc<1) {
    printf("Generate HTML for a table:\n");
    printf("  ss2html [--decorate] [--full] table.csv\n");
    printf("  ss2html [--decorate] [--full] table.xls\n");
    return 1;
  }

  if (verbose) {
    coopy_set_verbose(verbose);
  }

  const char *fname = "-";
  if (argc>0) {
    fname = argv[0];
  }
  PolyBook book;
  if (!book.read(fname)) { 
    fprintf(stderr,"Failed to read input\n");
    return 1;
  }
  int sz = (int)book.getNames().size();
  if (sz>1) {
    fprintf(stderr,"Too many sheets\n");
    return 1;
  }
  if (sz==0) {
    fprintf(stderr,"No sheet available\n");
    return 1;
  }

  CsvRender render;
  render.setDecorate(decorate);
  render.setFull(full);
  PolySheet sheet = book.readSheetByIndex(0);
  string result = render.renderHtml(sheet);
  printf("%s", result.c_str());

  return 0;
}
