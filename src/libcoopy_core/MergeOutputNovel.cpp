#include <coopy/MergeOutputNovel.h>

using namespace coopy::cmp;
using namespace coopy::store;
using namespace std;

#define FULL_COLOR (65535)
#define HALF_COLOR (65535/2)

bool MergeOutputNovel::declareLink(const LinkDeclare& decl) {
  // ignore columns
  if (decl.column) return true;

  dbg_printf("LINK %d %d %d // %s %s %s\n",
	     decl.rc_id_pivot,
	     decl.rc_id_local,
	     decl.rc_id_remote,
	     decl.rc_str_pivot.c_str(),
	     decl.rc_str_local.c_str(),
	     decl.rc_str_remote.c_str());

  PolySheet sheet = getSheet();
  sheet.hideHeaders();
  string col_name = "_ROW_";
  if (noted.find(sheet_name)==noted.end()) {
    sheet.insertColumn(ColumnRef(0),ColumnInfo(col_name));
    noted[sheet_name] = 1;
    if (sheet.width()>0) {
      for (int y=0; y<sheet.height(); y++) {
	sheet.cellString(0,y,""); // for consistency
      }
    }
  }

  if (decl.rc_id_local>=0) {
    if (decl.rc_id_remote>=0) {
      int y = decl.rc_id_local;
      if (y>=sheet.height()) {
	fprintf(stderr,"Out of range row? %d\n", y);
	return false;
      }
      if (sheet.width()<1) {
	fprintf(stderr,"No space in sheet\n");
	return false;
      }
      if (sheet.cellString(0,y)==col_name) return true;
      sheet.cellSummary(0,y,SheetCell(decl.rc_id_remote+2));
      Poly<Appearance> appear = sheet.getRowAppearance(y);
      if (appear.isValid()) {
	appear->begin();
	appear->setBackgroundRgb16(HALF_COLOR,
				   HALF_COLOR,
				   FULL_COLOR,
				   AppearanceRange::full());
	appear->setWeightBold(true,AppearanceRange::full());
	appear->end();
      }
    }
  }
  return true;
}
