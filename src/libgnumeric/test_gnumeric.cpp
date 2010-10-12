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

int main(int argc, char *argv[]) {
  if (argc>1) {
    if (string(argv[1])=="--verbose") {
      coopy_set_verbose(true);
      argc--;
      argv++;
    }
  }
  if (argc<3) {
    printf("Call as:\n");
    printf("  test_gnumeric template.xls target.xls\n");
    return 1;
  }
  const char *template_name = argv[1];
  const char *target_name = argv[2];

  gnumeric_init();
  GnumericWorkbookPtr book = gnumeric_load(template_name);
  CsvSheet data;
  data.addField("1");
  data.addField("2");
  data.addRecord();
  data.addField("3");
  data.addField("4");
  data.addRecord();
  SheetStyle style;
  string sdata = data.encode(style);
  char *start = (char*)(sdata.c_str());
  char *stop = (char*)(start+sdata.length());
  gnumeric_overlay_csv(book,start,stop);

  GnumericSheet sheet(gnumeric_get_sheet(book,0));
  for (int y=0; y<sheet.height(); y++) {
    for (int x=0; x<sheet.width(); x++) {
      printf("[%s] ", sheet.cellString(x,y).c_str());
    }
    printf("\n");
  }
  printf("\n");
  // try the following, with one column formated as text, the other as number
  sheet.cellString(0,0,"00100");
  sheet.cellString(1,0,"00100");
  for (int y=0; y<sheet.height(); y++) {
    for (int x=0; x<sheet.width(); x++) {
      printf("[%s] ", sheet.cellString(x,y).c_str());
    }
    printf("\n");
  }
  printf("\n");
  gnumeric_move_column(gnumeric_get_sheet(book,0),0,1);
  for (int y=0; y<sheet.height(); y++) {
    for (int x=0; x<sheet.width(); x++) {
      printf("[%s] ", sheet.cellString(x,y).c_str());
    }
    printf("\n");
  }
  gnumeric_save(book,target_name,NULL);
  gnumeric_free(book);
  gnumeric_fini();
  return 0;
}

