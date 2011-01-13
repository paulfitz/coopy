#ifndef COOPY_COMPAREFLAGS
#define COOPY_COMPAREFLAGS

#include <stdio.h>
#include <vector>
#include <string>

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
  std::vector<std::string> ids;
  std::vector<int> ids_local;
  std::vector<int> ids_remote;
  bool trust_ids;

  CompareFlags() {
    head_trimmed = false;
    tail_trimmed = false;
    out = stdout;
    trust_ids = false;
  }
};

#endif

