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
  const DataSheet& sheet;
  std::string name;
public:
  SchemaSniffer(const DataSheet& sheet, const char *name = NULL, bool delayed = false) : sheet(sheet) {
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
