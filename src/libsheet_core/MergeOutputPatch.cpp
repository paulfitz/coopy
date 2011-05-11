#include <coopy/MergeOutputPatch.h>

using namespace std;
using namespace coopy::store;
using namespace coopy::cmp;

bool MergeOutputPatch::addRow(const char *tag,
			      const std::vector<SheetCell>& row,
			      const std::string& blank) {
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


bool MergeOutputPatch::mergeAllDone() {
  SheetStyle style;
  fprintf(out,"%s",get().encode(style).c_str());
  return true;
}
