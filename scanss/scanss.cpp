
#include "CsvMerge.h"
#include "CsvFile.h"
#include "CsvStat.h"

int main(int argc, char *argv[]) {
  if (argc!=2) {
    printf("Please supply the name of a csv file\n");
    return 1;
  }
  CsvSheet ss;
  CsvFile::read(argv[1],ss);
  CsvStat stat;
  stat.evaluate(ss);
  printf("Removing first row, reevaluating...\n");
  ss.removeRow(0);
  stat.evaluate(ss);
  CsvFile::write(ss,"junk.csv");
  
  return 0;
}

