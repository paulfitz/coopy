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

ColumnRef PolySheet::insertColumn(const ColumnRef& base, 
				  const ColumnInfo& info) {
  COOPY_ASSERT(sheet);

  SheetSchema *s = getSchema();
  if (info.getName()!="") {
    if (!sheet->hasExternalColumnNames()) {
      if (!s) {
	mustHaveSchema();
	s = getSchema();
      }
    }
  }
  ColumnRef result = sheet->insertColumn(base,info);
  if (!result.isValid()) return result;
  if (info.getName()!="") {
    if (!sheet->hasExternalColumnNames()) {
      int at = -1;
      if (s) {
	at = s->headerHeight() - 1;
      }
      if (at>=0) {
	sheet->cellString(result.getIndex(),at,info.getName());
      }
    }
  }
  if (s) {
    if (!s->isShadow()) {
      ColumnRef result2 = s->insertColumn(base,info);
      COOPY_ASSERT(result2.getIndex()==result.getIndex());
    }
    if (info.hasName()) {
      if (!sheet->hasExternalColumnNames()) {
	if (s->headerHeight()>0) {
	  /*
	  printf("[[[ PLACING %s at %d,%d of %d,%d ]]]\n%s\n...\n%s\n...\n",
		 info.getName().c_str(),
		 result.getIndex(),s->headerHeight()-1,
		 sheet->width(), sheet->height(),
		 sheet->toString().c_str(),
		 desc().c_str());
	  */
	  sheet->cellString(result.getIndex(),s->headerHeight()-1,
			    info.getName());
	}
      }
    }
  }
  return result;
}




