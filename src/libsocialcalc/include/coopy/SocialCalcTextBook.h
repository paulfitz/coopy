#ifndef COOPY_SOCIALCALCTEXTBOOK
#define COOPY_SOCIALCALCTEXTBOOK

#include <coopy/TextBook.h>
#include <coopy/CsvSheet.h>
#include <coopy/TextBookFactory.h>
#include <coopy/Dbg.h>
//#include <coopy/JsWrap.h>
#include <coopy/SocialCalcSheet.h>

namespace coopy {
  namespace store {
    namespace socialcalc {
      class SocialCalcTextBook;
    }
  }
}

class coopy::store::socialcalc::SocialCalcTextBook : public TextBook {
public:
  std::string name;
  int provides;
  SocialCalcSheet sheet;
  //coopy::js::JsWrap js;
  //jsval *jssheet;

  SocialCalcTextBook() : name(coopy_get_default_table_name()) {
    provides = 0;
    //jssheet = 0/*NULL*/;
  }

  virtual ~SocialCalcTextBook() {
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

  static bool write(TextBook *book, const Property& config);

  bool write(const Property& config);

  virtual PolySheet provideSheet(const SheetSchema& schema) {
    if (provides==0) {
      provides++;
      name = schema.getSheetName();
      if (sheet.applySchema(schema)) {
	PolySheet s = readSheet(name);
	if (s.isValid()) {
	  //sheet.setSchema(Poly<SheetSchema>(schema.clone(),true));
	  return s;
	}
	return s;
      }
    }
    return PolySheet();
  }

  virtual std::string desc() const {
    return "SocialCalcTextBook";
  }

  virtual bool namedSheets() const {
    return false;
  }
};

#endif
