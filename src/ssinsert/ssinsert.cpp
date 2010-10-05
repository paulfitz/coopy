#include <stdio.h>
#include <string.h>

#include <coopy/CsvFile.h>

extern "C" {
#include "coopy/gnumeric_link.h"
}

using namespace coopy::store;
using namespace std;

int main(int argc, char *argv[]) {
  if (argc<4) {
    printf("Call as:\n");
    printf("  ssinsert template.xls data.csv target.xls\n");
    return 1;
  }
  const char *template_name = argv[1];
  const char *data_name = argv[2];
  const char *target_name = argv[3];

  gnumeric_init();
  GnumericWorkbookPtr book = gnumeric_load(template_name);
  CsvSheet data;
  CsvFile::read(data_name,data);
  SheetStyle style;
  string sdata = data.encode(style);
  char *start = (char*)(sdata.c_str());
  char *stop = (char*)(start+sdata.length());
  gnumeric_overlay_csv(book,start,stop);
  gnumeric_save(book,target_name);
  gnumeric_free(book);
  gnumeric_fini();
  return 0;
}
