#ifndef COOPY_WRAPBOOK
#define COOPY_WRAPBOOK

#include <coopy/TextBook.h>
#include <coopy/PolySheet.h>
#include <coopy/Dbg.h>

namespace coopy {
  namespace store {
    class WrapBook;
  }
}

class coopy::store::WrapBook : public TextBook {
public:
  std::string name;
  PolySheet sheet;

  WrapBook(DataSheet& sheet, bool owned=false) : sheet(&sheet,owned),
    name(coopy_get_default_table_name()) {
  }

  virtual std::vector<std::string> getNames() {
    std::vector<std::string> result;
    result.push_back(name);
    return result;
  }

  virtual PolySheet readSheet(const std::string& name) {
    if (name==this->name) {
      return sheet;
    }
    return PolySheet();
  }

  virtual std::string desc() const {
    return "WrapBook";
  }

  virtual bool namedSheets() const {
    return false;
  }
};

#endif
