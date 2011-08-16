#include <stdio.h>
#include <string.h>

#include <coopy/Dbg.h>
#include <coopy/CsvSheet.h>
#include <coopy/GnumericSheet.h>

extern "C" {
#include "coopy/gnumeric_link.h"
}

using namespace coopy::store;
using namespace coopy::store::gnumeric;
using namespace std;

int main_alt() {
  gnumeric_init();
  GnumericWorkbookPtr book2 = gnumeric_create();
  GnumericSheetPtr sheet2 = gnumeric_add_sheet(book2,"bob");
  gnumeric_sheet_set_cell_as_string(sheet2,0,0,"10.5");
  GnumericStylePtr p = gnumeric_sheet_get_style(sheet2,0,0);
  gnumeric_style_set_font_bold(p,1);
  gnumeric_sheet_set_style(sheet2,p,0,0);
  gnumeric_save(book2,"book2.xls",NULL);
  gnumeric_free(book2);
  gnumeric_fini();

  return 0;
}

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
    return main_alt();
  }
  const char *template_name = argv[1];
  const char *target_name = argv[2];

  gnumeric_init();

  GnumericWorkbookPtr book = gnumeric_load(template_name);
  CsvSheet data;
  data.addField("1",false);
  data.addField("2",false);
  data.addRecord();
  data.addField("3",false);
  data.addField("4",false);
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

