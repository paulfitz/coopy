#ifndef SSFOSSIL_CSVFILE_INC
#define SSFOSSIL_CSVFILE_INC

#include <coopy/CsvWrite.h>

namespace CsvFile {
  int read(const char *src, CsvSheet& dest);
  //int write(DataSheet& src, const char *fname);
};

#endif
