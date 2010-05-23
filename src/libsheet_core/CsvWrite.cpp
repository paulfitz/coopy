#include <stdio.h>
#include <stdlib.h>

#include <coopy/CsvWrite.h>

int CsvFile::write(const DataSheet& src, const char *fname) {
  FILE *fp;
  fp = fopen(fname,"wb");
  if (!fp) {
    fprintf(stderr,"could not open %s\n", fname);
    exit(1);
  }
  SheetStyle style;
  style.setFromFilename(fname);
  std::string delim = style.getDelimiter();
  std::string result = src.encode(delim);
  fwrite(result.c_str(),1,result.length(),fp);
  fclose(fp);
  return 0;
}


