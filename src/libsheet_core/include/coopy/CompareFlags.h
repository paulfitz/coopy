#ifndef COOPY_COMPAREFLAGS
#define COOPY_COMPAREFLAGS

#include <stdio.h>
#include <vector>
#include <string>

namespace coopy {
  namespace cmp {
    class CompareFlags;
  }
  namespace store {
    // forward declarations
    class DataSheet;
    class DataBook;
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
  coopy::store::DataSheet *mapping;
  coopy::store::DataBook *mapping_book;
  bool pivot_sides_with_local;
  bool use_order;

  CompareFlags() {
    head_trimmed = false;
    tail_trimmed = false;
    out = stdout;
    trust_ids = false;
    mapping = 0 /*NULL*/;
    mapping_book = 0 /*NULL*/;
    pivot_sides_with_local = false;
    use_order = true;
  }
};

#endif

