#ifndef COOPY_SCHEMA_SNIFFER
#define COOPY_SCHEMA_SNIFFER

#include <coopy/SheetSchema.h>
#include <coopy/DataSheet.h>
#include <coopy/NameSniffer.h>

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
  NameSniffer *ns;
  std::string name;
public:

  SchemaSniffer() {
    sheet = 0;
    schema = 0;
    ns = 0;
  }

  SchemaSniffer(const DataSheet& sheet, const char *name = NULL, bool delayed = false) {
    this->sheet = 0;
    schema = 0;
    ns = 0;
    setSheet(sheet,name,delayed);
  }

  virtual ~SchemaSniffer() {
    if (ns) {
      delete ns;
      ns = 0;
    }
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
  void resniff(coopy::store::SchemaSniffer& alt); 

  virtual SheetSchema *suggestSchema();
};

#endif
