#ifndef SSFOSSIL_CSVWRITE_INC
#define SSFOSSIL_CSVWRITE_INC

#include "CsvSheet.h"

namespace CsvFile {
  int write(const DataSheet& src, const char *fname);
};

#endif
