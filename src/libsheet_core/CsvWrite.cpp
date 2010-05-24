#include <stdio.h>
#include <stdlib.h>

#include <coopy/CsvWrite.h>

int CsvFile::write(const DataSheet& src, const char *fname) {
  if (std::string(fname)=="-") {
    SheetStyle style;
    std::string result = src.encode(style);
    printf("%s",result.c_str());
  } else {
    FILE *fp;
    fp = fopen(fname,"wb");
    if (!fp) {
      fprintf(stderr,"could not open %s\n", fname);
      exit(1);
    }
    SheetStyle style;
    style.setFromFilename(fname);
    std::string result = src.encode(style);
    fwrite(result.c_str(),1,result.length(),fp);
    fclose(fp);
  }
  return 0;
}


