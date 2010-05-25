
#include <stdio.h>

#include <coopy/CsvFile.h>
#include <coopy/SheetCompare.h>
#include <coopy/MergeOutputAccum.h>

int main(int argc, char *argv[]) {
  if (argc!=5) {
    printf("Merge spreadsheets, with sensible treatement of changes:\n");
    printf("  ssmerge parent.csv local.csv remote.csv output.csv\n");
    printf("Note: parent.csv should be a 'common ancestor' of the other two.\n");
    printf("The merged or conflicting result will be placed in the specified output file.\n");
    return 1;
  }
  CsvSheet local, remote, parent;
  MergeOutputAccum accum;
  if (CsvFile::read(argv[1],parent)!=0) {
    return 1;
  }
  if (CsvFile::read(argv[2],local)!=0) {
    return 1;
  }
  if (CsvFile::read(argv[3],remote)!=0) {
    return 1;
  }
  SheetCompare cmp;
  cmp.compare(parent,local,remote,accum);
  if (CsvFile::write(accum.get(),argv[4])!=0) {
    return 1;
  }
  return 0;
}

