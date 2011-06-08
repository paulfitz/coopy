#include <coopy/SchemaSniffer.h>
#include <coopy/NameSniffer.h>
#include <coopy/IndexSniffer.h>
#include <coopy/Dbg.h>
#include <coopy/CompareFlags.h>

#include <stdlib.h>

using namespace coopy::store;
using namespace coopy::cmp;

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
  CompareFlags flags;
  NameSniffer nameSniffer(*sheet,flags);
  vector<string> names = nameSniffer.suggestNames();
  vector<ColumnType> ct = nameSniffer.suggestTypes();
  IndexSniffer indexSniffer(*sheet,flags,nameSniffer);
  vector<int> indexes = indexSniffer.suggestIndexes();
  bool haveKey = false;
  bool shouldSetKey = true;
  for (int i=0; i<(int)indexes.size(); i++) {
    if (indexes[i]>0) {
      haveKey = true;
    }
  }
  for (int i=0; i<(int)ct.size(); i++) {
    if (ct[i].primaryKeySet) {
      shouldSetKey = false;
      break;
    }
  }
  if (haveKey&&shouldSetKey) {
    for (int i=0; i<(int)indexes.size(); i++) {
      ct[i].primaryKeySet = true;
      ct[i].primaryKey = (indexes[i]>0);
    }
  }
  for (int i=0; i<(int)names.size(); i++) {
    ColumnType base = ct[i];
    if (haveKey) {
    }
    fallback.addColumn(names[i].c_str(),ct[i]);
  }
  char col[3] = "A2";
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
  fallback.setHasSheetName(sheet->hasSheetName());
  fallback.setHeaderHeight(nameSniffer.getHeaderHeight());
  schema = &fallback;
  fallback.setGuess(nameSniffer.isFake());
}

SheetSchema *SchemaSniffer::suggestSchema() {
  return schema;
}
