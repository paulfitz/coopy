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
private:
  std::string unread;
public:
  virtual ~Reader() {}

  // Return some bytes, not necessarily all.  
  // Returning a 0-length string signals end.
  virtual std::string read() = 0;

  // Get a line.  
  std::string readLine(bool& eof);
  
  virtual Format getFormat() = 0;
};

#endif
