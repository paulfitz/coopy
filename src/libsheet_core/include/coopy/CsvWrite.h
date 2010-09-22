#ifndef SSFOSSIL_CSVWRITE_INC
#define SSFOSSIL_CSVWRITE_INC

#include <coopy/CsvSheet.h>

namespace coopy {
  namespace store {
    namespace CsvFile {
      int write(const DataSheet& src, const char *fname);
    }
  }
}

#endif
