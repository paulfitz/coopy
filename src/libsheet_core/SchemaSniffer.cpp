#include <coopy/SchemaSniffer.h>
#include <coopy/NameSniffer.h>
#include <coopy/IndexSniffer.h>
#include <coopy/Dbg.h>

#include <stdlib.h>

using namespace coopy::store;

using namespace std;

void SchemaSniffer::sniff(bool force) {
  dbg_printf("Sniff schema\n");
  COOPY_ASSERT(sheet!=NULL);
  if (!force) {
    if (sheet->getSchema()!=NULL) {
      if (sheet->getSchema()->getColumnCount()!=0) {
	schema = sheet->getSchema();
	return;
      }
    }
  }
  NameSniffer nameSniffer(*sheet);
  vector<string> names = nameSniffer.suggestNames();
  vector<ColumnType> ct = nameSniffer.suggestTypes();
  for (int i=0; i<(int)names.size(); i++) {
    fallback.addColumn(names[i].c_str(),ct[i]);
  }
  char col[2] = "A";
  int at = (int)names.size();
  while ((int)fallback.getColumnCount()<sheet->width()) {
    fallback.addColumn(col,ct[at]);
    at++;
    if (col[0]=='Z') {
      fprintf(stderr,"SchemaSniffer.cpp is inadequately inventive\n");
      exit(1);
    }
    col[0]++;
  }
  fallback.setSheetName(name.c_str());
  fallback.setHeaderHeight(nameSniffer.getHeaderHeight());
  schema = &fallback;
  fallback.setGuess(nameSniffer.isFake());
}

SheetSchema *SchemaSniffer::suggestSchema() {
  return schema;
}
