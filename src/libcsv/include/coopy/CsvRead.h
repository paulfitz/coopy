#ifndef SSFOSSIL_CSVREAD_INC
#define SSFOSSIL_CSVREAD_INC

#include <coopy/CsvSheet.h>
#include <coopy/Reader.h>

namespace coopy {
  namespace store {
    class CsvSheetReader;

    namespace CsvFile {
      int read(const char *src, CsvSheet& dest, const Property& config);
      int read(coopy::format::Reader& reader, CsvSheet& dest,
	       const Property& config);

      int read(const char *src, CsvSheet& dest);
      int read(coopy::format::Reader& reader, CsvSheet& dest);

      int read(const char *src, 
	       CsvSheetReader& dest, 
	       const Property& config);

      int read(const char *data, int len,
	       CsvSheetReader& dest, 
	       const Property& config);
    }
  }
}

/**
 *
 * Factory for CSV objects while loading CSV files.
 *
 */
class coopy::store::CsvSheetReader {
public:
  virtual CsvSheet *nextSheet(const char *name, bool named) = 0;
};

#endif
