#include <coopy/MergeOutputFilter.h>

using namespace std;

using namespace coopy::cmp;

bool MergeOutputFilter::mergeAllDone() {
  /*
  for (int i=0; i<(int)sheet_order.size(); i++) {
    string name = sheet_order[i];
    SheetUnit& unit = sheet_units[name];
  }
  */

  if (desired_sheets.size()<2) {
    for (std::list<RowUnit>::iterator it=rows.begin(); it!=rows.end(); it++) {
      emitRow(*it);
    }
  } else {
    const CompareFlags& flags = getFlags();
    // could do with some optimization...
    for (int i=0; i<(int)flags.ordered_tables.size(); i++) {
      string name = flags.ordered_tables[i];
      for (std::list<RowUnit>::iterator it=rows.begin(); it!=rows.end(); it++) {
	if (it->sheet_name == name) {
	  emitRow(*it);
	}
      }
    }
  }
  chain->mergeDone();

  return chain->mergeAllDone();
}


bool MergeOutputFilter::emitRow(const RowUnit& row) {
  string name = row.sheet_name;
  if (name!=last_sheet_name) {
    if (last_sheet_name!="") {
      chain->mergeDone();
    }
    chain->setSheet(name.c_str());
    last_sheet_name = name;
    if (started_sheets.find(name)==started_sheets.end()) {
      emitPreamble(sheet_units[name]);
      started_sheets[name] = 1;
    }
  }
  return chain->changeRow(row.change);
}

bool MergeOutputFilter::emitPreamble(const SheetUnit& preamble) {
  if (preamble.have_name0) {
    chain->changeName(preamble.name0);
  }
  for (std::list<OrderChange>::const_iterator it = preamble.orders.begin();
       it != preamble.orders.end(); it++) {
    chain->changeColumn(*it);
  }
  if (preamble.have_name1) {
    chain->changeName(preamble.name1);
  }
  for (std::list<PoolChange>::const_iterator it = preamble.pools.begin();
       it != preamble.pools.end(); it++) {
    chain->changePool(*it);
  }
  return true;
}


bool MergeOutputFilter::mergeStart() {
  const CompareFlags& flags = getFlags();
  for (int i=0; i<(int)flags.ordered_tables.size(); i++) {
    desired_sheets[flags.ordered_tables[i]] = 1;
  }
  return chain->mergeStart();
}
