#include <coopy/MergeOutputIndex.h>
#include <coopy/Dbg.h>

using namespace coopy::store;
using namespace coopy::cmp;

bool MergeOutputIndex::mergeStart() {
  SimpleSheetSchema ss;
  ss.setSheetName("links");
  ss.addColumn("frame",ColumnType("TEXT"));
  ss.addColumn("category",ColumnType("TEXT"));
  ss.addColumn("pivot",ColumnType("INTEGER"));
  ss.addColumn("local",ColumnType("INTEGER"));
  ss.addColumn("remote",ColumnType("INTEGER"));
  ss.addColumn("deleted",ColumnType("INTEGER"));
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
  std::string mode = decl.column?"column":"row";
  std::string frame = "none";
  switch (decl.mode) {
  case LINK_DECLARE_MERGE:
    frame = "merge";
    break;
  case LINK_DECLARE_LOCAL:
    frame = "local";
    break;
  case LINK_DECLARE_REMOTE:
    frame = "remote";
    break;
  }
  row.setCell(0,SheetCell(frame,false));
  row.setCell(1,SheetCell(mode,false));
  row.setCell(2,SheetCell(decl.rc_id_pivot));
  row.setCell(3,SheetCell(decl.rc_id_local));
  row.setCell(4,SheetCell(decl.rc_id_remote));
  row.setCell(5,SheetCell(decl.rc_deleted?1:0));
  row.flush();
  return true;
}
