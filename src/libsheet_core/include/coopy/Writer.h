#ifndef COOPY_WRITER_INC
#define COOPY_WRITER_INC

#include <string>

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
};

#endif
