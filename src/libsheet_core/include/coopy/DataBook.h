#ifndef COOPY_DATABOOK
#define COOPY_DATABOOK

#include <coopy/PolySheet.h>

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

  virtual PolySheet readSheet(const std::string& name) = 0;

  virtual PolySheet readSheetByIndex(int index) {
    std::vector<std::string> names = getNames();
    if (index>=(int)names.size()) return PolySheet();
    return readSheet(names[index]);
  }

  virtual bool save(const char *fname, const char *format) {
    return false;
  }

  virtual bool inplace() const {
    return false;
  }
};

#endif
