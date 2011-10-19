#include <coopy/PolySheet.h>
#include <coopy/SchemaSniffer.h>

using namespace coopy::store;

bool PolySheet::mustHaveSchema() {
  if (getSchema()) return true;
  if (!sheet) return false;
  SchemaSniffer ss(*sheet);
  SheetSchema *sc = ss.suggestSchema();
  COOPY_ASSERT(sc);
  setSchema(sc->clone(),true);
  return true;
}


