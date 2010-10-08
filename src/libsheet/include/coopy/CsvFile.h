#ifndef SSFOSSIL_CSVFILE_INC
#define SSFOSSIL_CSVFILE_INC

#include <coopy/CsvWrite.h>
#include <coopy/Reader.h>

namespace coopy {
  namespace store {
    namespace CsvFile {
      int read(const char *src, CsvSheet& dest);
      int read(coopy::format::Reader& reader, CsvSheet& dest);
      //int write(DataSheet& src, const char *fname);
    }
  }
}

#endif
