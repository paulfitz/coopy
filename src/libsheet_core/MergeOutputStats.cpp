#include <coopy/MergeOutputStats.h>
#include <coopy/SheetSchema.h>

using namespace std;
using namespace coopy::cmp;
using namespace coopy::store;

bool MergeOutputStats::flush() {
  if (active) {
    SimpleSheetSchema ss;
    if (sheet_name=="") {
      sheet_name = "sheet";
    }
    ss.setSheetName(sheet_name.c_str());
    ss.addColumn("nature",ColumnType("TEXT"));
    ss.addColumn("operation",ColumnType("TEXT"));
    ss.addColumn("count",ColumnType("INTEGER"));
    if (getOutputBook()!=NULL) {
      ops = getOutputBook()->provideSheet(ss);
    }
    if (!ops.isValid()) {
      fprintf(stderr,"* Could not generate stats sheet\n");
      exit(1);
      return false;
    }
    ops.deleteData();

    const CompareFlags& flags = getFlags();
    Poly<SheetRow> row;

    if (flags.canSchema()) {
      row = ops.insertRow();
      row->setCell(0,SheetCell("column",false));
      row->setCell(1,SheetCell("all",false));
      row->setCell(2,SheetCell::makeInt(ct_col));
      row->flush();
      row = ops.insertRow();
      row->setCell(0,SheetCell("column",false));
      row->setCell(1,SheetCell("insert",false));
      row->setCell(2,SheetCell::makeInt(ct_col_insert));
      row->flush();
      row = ops.insertRow();
      row->setCell(0,SheetCell("column",false));
      row->setCell(1,SheetCell("delete",false));
      row->setCell(2,SheetCell::makeInt(ct_col_delete));
      row->flush();
      row = ops.insertRow();
      row->setCell(0,SheetCell("column",false));
      row->setCell(1,SheetCell("move",false));
      row->setCell(2,SheetCell::makeInt(ct_col_move));
      row->flush();
      row = ops.insertRow();
      row->setCell(0,SheetCell("column",false));
      row->setCell(1,SheetCell("rename",false));
      row->setCell(2,SheetCell::makeInt(ct_col_rename));
      row->flush();
    }

    if (flags.canInsert()||flags.canDelete()||flags.canUpdate()) {
      row = ops.insertRow();
      row->setCell(0,SheetCell("row",false));
      row->setCell(1,SheetCell("all",false));
      row->setCell(2,SheetCell::makeInt(ct_row));
      row->flush();
      row = ops.insertRow();
      row->setCell(0,SheetCell("row",false));
      row->setCell(1,SheetCell("insert",false));
      row->setCell(2,SheetCell::makeInt(ct_row_insert));
      row->flush();
      row = ops.insertRow();
      row->setCell(0,SheetCell("row",false));
      row->setCell(1,SheetCell("delete",false));
      row->setCell(2,SheetCell::makeInt(ct_row_delete));
      row->flush();
      row = ops.insertRow();
      row->setCell(0,SheetCell("row",false));
      row->setCell(1,SheetCell("move",false));
      row->setCell(2,SheetCell::makeInt(ct_row_move));
      row->flush();
      row = ops.insertRow();
      row->setCell(0,SheetCell("row",false));
      row->setCell(1,SheetCell("update",false));
      row->setCell(2,SheetCell::makeInt(ct_row_update));
      row->flush();
    }
  }
  ops.clear();
  active = false;
  ct_col = ct_col_insert = ct_col_delete = ct_col_move = ct_col_rename = 0;
  ct_row = ct_row_insert = ct_row_delete = ct_row_move = ct_row_update = 0;
  return true;
}


bool MergeOutputStats::changeColumn(const OrderChange& change) {
  active = true;
  ct_col++;
  switch (change.mode) {
  case ORDER_CHANGE_INSERT:
    ct_col_insert++;
    break;
  case ORDER_CHANGE_DELETE:
    ct_col_delete++;
    break;
  case ORDER_CHANGE_MOVE:
    ct_col_move++;
    break;
  case ORDER_CHANGE_RENAME:
    ct_col_rename++;
    break;
  }
  return true;
}

bool MergeOutputStats::changeRow(const RowChange& change) {
  if (change.mode == ROW_CHANGE_CONTEXT) return true;
  active = true;
  ct_row++;
  switch (change.mode) {
  case ROW_CHANGE_INSERT:
    ct_row_insert++;
    break;
  case ROW_CHANGE_DELETE:
    ct_row_delete++;
    break;
  case ROW_CHANGE_MOVE:
    ct_row_move++;
    break;
  case ROW_CHANGE_UPDATE:
    ct_row_update++;
    break;
  }
  return true;
}
