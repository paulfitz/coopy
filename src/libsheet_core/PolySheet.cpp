#include <coopy/PolySheet.h>
#include <coopy/SchemaSniffer.h>

using namespace coopy::store;

bool PolySheet::mustHaveSchema() {
  if (getSchema()) return true;
  if (!sheet) return false;
  if (!may_sniff) return false;
  SchemaSniffer ss(*sheet);
  SheetSchema *sc = ss.suggestSchema();
  COOPY_ASSERT(sc);
  setSchema(sc->clone(),true);
  return true;
}

SheetSchema *PolySheet::preApplyInfo(const ColumnInfo& info) {
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
  return s;
}

bool PolySheet::postApplyInfo(const ColumnInfo& info, 
			      const ColumnRef& result) {
  SheetSchema *s = getSchema();
  /*
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
  */
  if (s) {
    if (info.hasName()) {
      if (!sheet->hasExternalColumnNames()) {
	if (s->headerHeight()>0) {
	  dbg_printf("Clobber %s with %s\n",
		     sheet->cellString(result.getIndex(),s->headerHeight()-1).c_str(),
		     info.getName().c_str());
			    
	  sheet->cellString(result.getIndex(),s->headerHeight()-1,
			    info.getName());
	}
      }
    }
  }
  return true;
}

ColumnRef PolySheet::insertColumn(const ColumnRef& base, 
				  const ColumnInfo& info) {
  SheetSchema *s = preApplyInfo(info);
  ColumnRef result = sheet->insertColumn(base,info);
  if (!result.isValid()) return result;
  if (s) {
    if (!s->isShadow()) {
      ColumnRef result2 = s->insertColumn(base,info);
      COOPY_ASSERT(result2.getIndex()==result.getIndex());
    }
  }
  postApplyInfo(info,result);
  return result;
}


bool PolySheet::modifyColumn(const ColumnRef& base, 
			     const ColumnInfo& info) {
  COOPY_ASSERT(sheet);
  SheetSchema *s = preApplyInfo(info);
  bool result = sheet->modifyColumn(base,info);
  if (!result) return false;
  if (s) {
    if (!s->isShadow()) {
      bool result2 = s->modifyColumn(base,info);
      COOPY_ASSERT(result2);
    }
  }
  postApplyInfo(info,base);
  return true;
}




