#ifndef COOPY_COMPAREFLAGS
#define COOPY_COMPAREFLAGS

#include <stdio.h>

namespace coopy {
  namespace cmp {
    class CompareFlags;
  }
}

class coopy::cmp::CompareFlags {
public:
  bool head_trimmed;
  bool tail_trimmed;
  FILE *out;

  CompareFlags() {
    head_trimmed = false;
    tail_trimmed = false;
    out = stdout;
  }
};

#endif

