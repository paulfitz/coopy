#ifndef COOPY_DATABOOK
#define COOPY_DATABOOK

#include <vector>
#include <string>

namespace coopy {
  namespace store {
    class DataBook;
  }
}

class coopy::store::DataBook {
public:
  virtual ~DataBook() {}

  virtual std::vector<std::string> getNames() = 0;
};

#endif
