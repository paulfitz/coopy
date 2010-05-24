#include <coopy/MergeOutputPatch.h>

using namespace std;

bool MergeOutputPatch::addRow(const char *tag,
			      const vector<string>& row,
			      const string& blank) {
  result.addField(tag);
  for (int i=0; i<row.size(); i++) {
    if (row[i]!=blank) {
      result.addField(row[i].c_str());
    } else {
      result.addField("");
    }
  }
  result.addRecord();
  return true;
}
