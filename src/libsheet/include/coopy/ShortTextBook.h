#ifndef COOPY_SHORTTEXTBOOK
#define COOPY_SHORTTEXTBOOK

#include <coopy/TextBook.h>
#include <coopy/CsvSheet.h>
#include <coopy/TextBookFactory.h>

namespace coopy {
  namespace store {
    class ShortTextBook;
  }
}

class coopy::store::ShortTextBook : public TextBook {
public:
  std::string name;
  int provides;
  CsvSheet sheet;

  ShortTextBook() : name("sheet") {
    provides = 0;
  }

  virtual std::vector<std::string> getNames() {
    std::vector<std::string> result;
    result.push_back(name);
    return result;
  }

  virtual PolySheet readSheet(const std::string& name) {
    if (name==this->name) {
      return PolySheet(&sheet,false);
    }
    return PolySheet();
  }

  virtual bool open(const Property& config);

  virtual PolySheet provideSheet(const SheetSchema& schema) {
    if (provides==0) {
      provides++;
      name = schema.getSheetName();
      if (sheet.applySchema(schema)) {
	PolySheet s = readSheet(name);
	if (s.isValid()) {
	  sheet.setSchema(Poly<SheetSchema>(schema.clone(),true));
	  return s;
	}
	return s;
      }
    }
    return PolySheet();
  }

  virtual std::string desc() const {
    return "CsvBook";
  }

  virtual bool namedSheets() const {
    return false;
  }
};

#endif
