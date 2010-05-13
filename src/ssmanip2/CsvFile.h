#ifndef SSFOSSIL_CSVFILE_INC
#define SSFOSSIL_CSVFILE_INC

#include "CsvWrite.h"

namespace CsvFile {
  int read(const char *src, CsvSheet& dest);
  //int write(DataSheet& src, const char *fname);
};

#endif
