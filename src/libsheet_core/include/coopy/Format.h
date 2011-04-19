#ifndef COOPY_FORMAT_INC
#define COOPY_FORMAT_INC

#include <string>

namespace coopy {
  namespace format {
    class Format;

    enum {
      FORMAT_UNKNOWN,
      FORMAT_PATCH_CSV,
      FORMAT_PATCH_TDIFF,
      FORMAT_PATCH_HUMAN,
      FORMAT_BOOK_SQLITE,
      FORMAT_BOOK_CSVS,
    };
  }
}


class coopy::format::Format {
public:
  int id;
  std::string name;
  
  Format() {
    id = FORMAT_UNKNOWN;
    name = "unknown";
  }
};

#endif

