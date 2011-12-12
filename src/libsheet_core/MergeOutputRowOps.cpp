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
      ss.addColumn((ids[i] + "0").c_str());
    }
    for (int i=0; i<(int)ids.size(); i++) {
      ss.addColumn((ids[i] + "1").c_str());
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
  int at = 2;
  for (int i=0; i<(int)ids.size(); i++) {
    string id = ids[i];
    if (change.cond.find(id)!=change.cond.end()) {
      row->setCell(at,change.cond.find(id)->second);
    }
    at++;
  }
  for (int i=0; i<(int)ids.size(); i++) {
    string id = ids[i];
    if (change.val.find(id)!=change.val.end()) {
      row->setCell(at,change.val.find(id)->second);
    }
    at++;
  }
  row->flush();

  return true;
}
