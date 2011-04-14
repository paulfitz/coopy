#ifndef COOPY_SCHEMA_SNIFFER
#define COOPY_SCHEMA_SNIFFER

#include <coopy/SheetSchema.h>
#include <coopy/DataSheet.h>

namespace coopy {
  namespace store {
    class SchemaSniffer;
  }
}

class coopy::store::SchemaSniffer {
private:
  SimpleSheetSchema fallback;
  SheetSchema *schema;
  const DataSheet *sheet;
  std::string name;
public:

  SchemaSniffer() {
    sheet = 0;
    schema = 0;
  }

  SchemaSniffer(const DataSheet& sheet, const char *name = NULL, bool delayed = false) {
    this->sheet = 0;
    schema = 0;
    setSheet(sheet,name,delayed);
  }

  void setSheet(const DataSheet& sheet, const char *name = NULL, bool delayed = false) {
    this->sheet = &sheet;
    schema = 0;
    if (name) {
      this->name = name;
    }
    if (!delayed) {
      sniff();
    }
  }

  void sniff(bool force = false); 

  virtual SheetSchema *suggestSchema();
};

#endif
