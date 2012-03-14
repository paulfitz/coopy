#include <coopy/MergeOutputEditList.h>

using namespace coopy::store;
using namespace coopy::cmp;
using namespace std;

bool MergeOutputEditList::setSheet(const char *name) {
  outputField("sheet");
  outputField(name);
  outputRecord();
  sheet_name = name;
  return true;
}

bool MergeOutputEditList::changeColumn(const OrderChange& change) {
  switch (change.mode) {
  case ORDER_CHANGE_INSERT:
    outputField("column");
    outputField("");
    outputField("insert");
    outputField(SheetCell(change.identityToIndexAfter(change.object)));
    outputRecord();
    break;
  default:
    fprintf(stderr,"Sorry, only bare min of 'edit' format implemented yet for a specific task\n");
    exit(1);
    break;
  }
  return true;
}


bool MergeOutputEditList::changeRow(const RowChange& change) {
  switch (change.mode) {
  case ROW_CHANGE_UPDATE:
    outputField("row");
    outputField(change.lRow);
    outputField("update");
    for (RowChange::txt2cell::const_iterator it = change.val.begin();
	 it!=change.val.end(); it++) {
      string name = it->first;
      SheetCell cell = it->second;
      outputField(SheetCell(col_map[name]));
      outputField(cell);
    }
    outputRecord();
    break;
  default:
    fprintf(stderr,"Sorry, only bare min of 'edit' format implemented yet for a specific task\n");
    exit(1);
    break;
  }
  return true;
}

bool MergeOutputEditList::makeSafe() {
  PolySheet sheet = getSheet();
  if (!sheet.isValid()) return false;
  if (x<0) {
    sheet.deleteData();
    x = 0;
    y = 0;
  }
  while (sheet.width()<=x) {
    ColumnRef col = sheet.insertColumn(ColumnRef(-1));
    if (!col.isValid()) return false;
  }
  while (sheet.height()<=y) {
    RowRef row = sheet.insertRow(RowRef(-1));
    if (!row.isValid()) return false;
  }
  return true;
}

bool MergeOutputEditList::outputField(const coopy::store::SheetCell& cell) {
  if (!makeSafe()) return false;
  PolySheet sheet = getSheet();
  sheet.cellSummary(x,y,cell);
  x++;
  return true;
}

bool MergeOutputEditList::outputRecord() {
  x = 0;
  y++;
  return true;
}


bool MergeOutputEditList::changeName(const NameChange& change) {
  for (int i=0; i<(int)change.names.size(); i++) {
    col_map[change.names[i]] = i;
  }
  return true;
}

