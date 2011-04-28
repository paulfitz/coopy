#include <coopy/Dbg.h>
#include <coopy/MergeOutputAccum.h>

using namespace std;
using namespace coopy::store;
using namespace coopy::cmp;

bool MergeOutputAccum::addRow(const char *tag,
			      const std::vector<SheetCell>& row,
			      const std::string& blank) {
  dbg_printf("ADDING ROW of len %d\n", (int)row.size());
  result.addField(tag,false);
  for (size_t i=0; i<row.size(); i++) {
    if (row[i].text!=blank) {
      result.addField(row[i].text.c_str(),row[i].escaped);
    } else {
      result.addField("",true);
    }
  }
  result.addRecord();
  return true;
}

bool MergeOutputAccum::stripMarkup() { 
  CsvSheet tmp = result;
  result.clear();
  for (int y=1; y<tmp.height(); y++) {
    for (int x=1; x<tmp.width(); x++) {
      const CsvSheet::pairCellType& p = tmp.pcell(x,y);
      result.addField(p.first.c_str(),p.second);
    }
    result.addRecord();
  }
  return true; 
}
