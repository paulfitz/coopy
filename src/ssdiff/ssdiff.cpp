
#include <stdio.h>

#include <coopy/CsvFile.h>
#include <coopy/CsvCompare.h>

int main(int argc, char *argv[]) {
  if (argc<3) {
    printf("Show difference between two CSV spreadsheets. Call as:\n");
    printf("  csvdiff local.csv modified.csv [output.csv]\n");
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
  CsvCompare cmp;
  cmp.compare(local,local,remote,true);
  const CsvSheet& result = cmp.get();
  if (argc>=4) {
    if (CsvFile::write(cmp.get(),argv[3])!=0) {
      return 1;
    }
  } else {
    std::string out = result.encode();
    printf("%s",out.c_str());
  }
  return 0;
}

