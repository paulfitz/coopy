#include <coopy/CsvSheet.h>

#include <stdio.h>
#include <stdlib.h>

using namespace coopy::store;

void CsvSheet::addField(const char *s) {
  if (th>=1&&tw>=w) {
    fprintf(stderr,"Spurious field in sheet\n");
    exit(1);
  }
  rec.push_back(s);
  tw++;
}
