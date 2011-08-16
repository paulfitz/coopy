#ifndef SSFOSSIL_CSVWRITE_INC
#define SSFOSSIL_CSVWRITE_INC

#include <coopy/DataSheet.h>
#include <coopy/Property.h>

namespace coopy {
  namespace store {
    /**
     *
     * CSV file I/O.
     *
     */
    namespace CsvFile {
      int write(const DataSheet& src, const char *fname);
      int write(const DataSheet& src, const Property& config);
    }
  }
}

#endif
