#include <coopy/MergeOutputRowOps.h>
#include <coopy/SheetSchema.h>

using namespace std;
using namespace coopy::cmp;
using namespace coopy::store;

bool MergeOutputRowOps::changeRow(const RowChange& change) {
  const CompareFlags& flags = getFlags();
  if (!ops.isValid()) {
    SimpleSheetSchema ss;
    if (sheet_name=="") {
      sheet_name = "sheet";
    }
    ss.setSheetName(sheet_name.c_str());
    ss.addColumn("name",ColumnType("TEXT"));
    ss.addColumn("op",ColumnType("TEXT"));
    if (flags.ids.size()>0) {
      ids = flags.ids;
    } else {
      ids = change.allNames;
    }
    for (int i=0; i<(int)ids.size(); i++) {
      ss.addColumn(ids[i].c_str());
    }
    if (getOutputBook()!=NULL) {
      ops = getOutputBook()->provideSheet(ss);
    }
    
    if (!ops.isValid()) {
      fprintf(stderr,"* Could not generate row operations sheet\n");
      exit(1);
      return false;
    }

    ops.deleteData();
  }

  Poly<SheetRow> row = ops.insertRow();
  row->setCell(0,SheetCell(sheet_name.c_str(),false));
  row->setCell(1,SheetCell(change.modeString().c_str(),false));
  for (int i=0; i<(int)ids.size(); i++) {
    string id = ids[i];
    if (change.cond.find(id)!=change.cond.end()) {
      row->setCell(i+2,change.cond.find(id)->second);
    }
  }
  row->flush();

  return true;
}
