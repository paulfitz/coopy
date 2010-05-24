#ifndef COOPY_DATABOOK
#define COOPY_DATABOOK

#include <vector>
#include <string>

class DataBook {
public:
  virtual std::vector<std::string> getNames() = 0;
};

#endif
