
#include <stdio.h>

#include <coopy/CsvFile.h>
#include <coopy/CsvCompare.h>

int main(int argc, char *argv[]) {
  if (argc!=5) {
    printf("Merge CSV spreadsheets, call as:\n");
    printf("  csvmerge parent.csv local.csv remote.csv output.csv\n");
    printf("Where parent.csv should be a CSV file that is an ancestor of the other two\n");
    printf("The merged or conflicting result will be placed in the specified output file\n");
    return 1;
  }
  CsvSheet local, remote, parent;
  if (CsvFile::read(argv[1],parent)!=0) {
    return 1;
  }
  if (CsvFile::read(argv[2],local)!=0) {
    return 1;
  }
  if (CsvFile::read(argv[3],remote)!=0) {
    return 1;
  }
  CsvCompare3 cmp;
  cmp.compare(parent,local,remote);
  if (CsvFile::write(cmp.get(),argv[4])!=0) {
    return 1;
  }
  return 0;
}

