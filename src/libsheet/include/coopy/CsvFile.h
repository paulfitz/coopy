#ifndef SSFOSSIL_CSVFILE_INC
#define SSFOSSIL_CSVFILE_INC

#include <coopy/CsvWrite.h>
#include <coopy/CsvSheet.h>
#include <coopy/Reader.h>

namespace coopy {
  namespace store {
    namespace CsvFile {
      int read(const char *src, CsvSheet& dest, const Property& config);
      int read(coopy::format::Reader& reader, CsvSheet& dest,
	       const Property& config);

      int read(const char *src, CsvSheet& dest);
      int read(coopy::format::Reader& reader, CsvSheet& dest);
    }
  }
}

#endif
