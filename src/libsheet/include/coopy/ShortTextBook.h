#ifndef COOPY_SHORTTEXTBOOK
#define COOPY_SHORTTEXTBOOK

#include <coopy/TextBook.h>
#include <coopy/CsvSheet.h>

namespace coopy {
  namespace store {
    class ShortTextBook;
  }
}

class coopy::store::ShortTextBook : public TextBook {
public:
  CsvSheet sheet;

  virtual std::vector<std::string> getNames() {
    std::vector<std::string> result;
    result.push_back("single");
    return result;
  }

  virtual PolySheet readSheet(const std::string& name) {
    if (name=="single") {
      return PolySheet(&sheet,false);
    }
    return PolySheet();
  }

  virtual bool open(const Property& config);
};

#endif
