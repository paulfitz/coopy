#include <coopy/MergeOutputIndex.h>
#include <coopy/Dbg.h>

using namespace coopy::store;
using namespace coopy::cmp;

bool MergeOutputIndex::mergeStart() {
  SimpleSheetSchema ss;
  ss.setSheetName("links");
  ss.addColumn("category",ColumnType("TEXT"));
  ss.addColumn("pivot",ColumnType("INTEGER"));
  ss.addColumn("local",ColumnType("INTEGER"));
  ss.addColumn("remote",ColumnType("INTEGER"));
  links = getBook()->provideSheet(ss);

  if (!links.isValid()) {
    fprintf(stderr,"* Could not generate links sheet\n");
    exit(1);
    return false;
  }

  return true;
}

bool MergeOutputIndex::mergeDone() {
  return true;
}

bool MergeOutputIndex::declareLink(const LinkDeclare& decl) {
  Poly<SheetRow> pRow = links.insertRow();
  
  dbg_printf("LINK %d %d %d %d\n",
	     decl.mode,
	     decl.rc_id_pivot,
	     decl.rc_id_local,
	     decl.rc_id_remote);
  SheetRow& row = *pRow;
  std::string mode = "none";
  switch (decl.mode) {
  case LINK_DECLARE_ROW:
    mode = "row";
    break;
  case LINK_DECLARE_COLUMN:
    mode = "column";
    break;
  }
  row.setCell(0,SheetCell(mode,false));
  row.setCell(1,SheetCell(decl.rc_id_pivot));
  row.setCell(2,SheetCell(decl.rc_id_local));
  row.setCell(3,SheetCell(decl.rc_id_remote));
  row.flush();
  return true;
}
