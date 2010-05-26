#ifndef COOPY_COMPAREFLAGS
#define COOPY_COMPAREFLAGS

class CompareFlags {
public:
  bool head_trimmed;
  bool tail_trimmed;

  CompareFlags() {
    head_trimmed = false;
    tail_trimmed = false;
  }
};

#endif

