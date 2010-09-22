
#include <stdio.h>

#include <coopy/CsvFile.h>
#include <coopy/CsvPatch.h>

using namespace coopy::store;
using namespace coopy::cmp;

int main(int argc, char *argv[]) {
  if (argc<3) {
    printf("Apply patch to a spreadsheet. Currently doesn't do much.\n");
    printf("  sspatch sheet.csv patch.csv [output.csv]\n");
    printf("Output defaults to standard output.\n");
    printf("Write output to original file by doing:.\n");
    printf("  sspatch sheet.csv patch.csv sheet.csv\n");
    return 1;
  }
  CsvSheet local, patch;
  if (CsvFile::read(argv[1],local)!=0) {
    return 1;
  }
  if (CsvFile::read(argv[2],patch)!=0) {
    return 1;
  }
  CsvPatch patcher;
  patcher.apply(local,patch);
  const CsvSheet& result = patcher.get();
  if (argc>=4) {
    if (CsvFile::write(result,argv[3])!=0) {
      return 1;
    }
  } else {
    SheetStyle style;
    std::string out = result.encode(style);
    printf("%s",out.c_str());
  }
  return 0;
}

