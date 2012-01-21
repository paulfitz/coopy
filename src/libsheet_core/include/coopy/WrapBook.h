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

  WrapBook() {
    name = "";
  }

  WrapBook(DataSheet& sheet, bool owned) : sheet(&sheet,owned),
    name(coopy_get_default_table_name()) {
  }

  WrapBook(PolySheet sheet) : sheet(sheet), 
    name(coopy_get_default_table_name()) {
  }

  bool attach(DataSheet& sheet, bool owned=false) {
    sheet = PolySheet(&sheet,owned);
    name = coopy_get_default_table_name();
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
