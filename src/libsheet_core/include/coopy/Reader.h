#ifndef COOPY_READER_INC
#define COOPY_READER_INC

#include <string>

#include <coopy/Format.h>

namespace coopy {
  namespace format {
    class Reader;
  }
}

class coopy::format::Reader {
public:
  virtual ~Reader() {}

  // Return some bytes, not necessarily all.  
  // Returning a 0-length string signals end.
  virtual std::string read() = 0;
  
  virtual Format getFormat() = 0;
};

#endif
