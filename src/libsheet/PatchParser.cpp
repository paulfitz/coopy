
#include <coopy/PatchParser.h>
#include <coopy/CsvFile.h>
#include <coopy/Format.h>

#include <stdio.h>

using namespace coopy::cmp;
using namespace coopy::format;
using namespace coopy::store;
using namespace std;

bool PatchParser::apply() {
  if (reader==NULL) return false;
  if (patcher==NULL) return false;

  Format format = reader->getFormat();
  if (format.id!=FORMAT_PATCH_CSV) {
    fprintf(stderr,"Unsupported format\n");
    return false;
  }

  CsvSheet patch;
  if (CsvFile::read(*reader,patch)!=0) {
    fprintf(stderr,"Failed to read patch\n");
    return false;
  }

  for (int i=0; i<patch.height(); i++) {
    printf("[%d] \n", i);
  }

  return true;
}


