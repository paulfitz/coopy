
#include <stdio.h>

#include <coopy/SheetCompare.h>
#include <coopy/CsvFile.h>
#include <coopy/MergeOutputPatch.h>

int main(int argc, char *argv[]) {
  if (argc<3) {
    printf("Show difference between two CSV spreadsheets. Call as:\n");
    printf("  ssdiff local.csv modified.csv [output.csv]\n");
    printf("Output defaults to standard output.\n");
    return 1;
  }
  CsvSheet local, remote;
  if (CsvFile::read(argv[1],local)!=0) {
    return 1;
  }
  if (CsvFile::read(argv[2],remote)!=0) {
    return 1;
  }
  SheetCompare cmp;
  MergeOutputPatch patch;
  cmp.compare(local,local,remote,patch);
  const CsvSheet& result = patch.get();
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

