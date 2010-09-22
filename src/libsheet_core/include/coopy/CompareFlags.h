#ifndef COOPY_COMPAREFLAGS
#define COOPY_COMPAREFLAGS

namespace coopy {
  namespace cmp {
    class CompareFlags;
  }
}

class coopy::cmp::CompareFlags {
public:
  bool head_trimmed;
  bool tail_trimmed;

  CompareFlags() {
    head_trimmed = false;
    tail_trimmed = false;
  }
};

#endif

