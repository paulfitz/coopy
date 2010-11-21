#include <coopy/SchemaSniffer.h>
#include <coopy/NameSniffer.h>
#include <coopy/IndexSniffer.h>

using namespace coopy::store;

using namespace std;

void SchemaSniffer::sniff() {
  if (sheet.getSchema()!=NULL) {
    schema = sheet.getSchema();
    return;
  }
  NameSniffer nameSniffer(sheet);
  vector<string> names = nameSniffer.suggestNames();
  for (int i=0; i<(int)names.size(); i++) {
    fallback.addColumn(names[i].c_str());
  }
  while ((int)names.size()<sheet.width()) {
    fallback.addColumn("*");
  }
  fallback.setSheetName(name.c_str());
  fallback.setHeaderHeight(nameSniffer.getHeaderHeight());
  schema = &fallback;
}

SheetSchema *SchemaSniffer::suggestSchema() {
  return schema;
}
