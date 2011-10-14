#ifndef COOPY_DIFF_H
#define COOPY_DIFF_H

#include <coopy/Options.h>

namespace coopy {
  namespace app {
    class Diff;
  }
}

class coopy::app::Diff {
public:

  int apply(const Options& options);
};

#endif
