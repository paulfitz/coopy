#include <coopy/MergeOutputAccum.h>

using namespace std;

bool MergeOutputAccum::addRow(const char *tag,
			      const vector<string>& row,
			      const string& blank) {
  result.addField(tag);
  for (size_t i=0; i<row.size(); i++) {
    if (row[i]!=blank) {
      result.addField(row[i].c_str());
    } else {
      result.addField("");
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
      result.addField(tmp.cell(x,y).c_str());
    }
    result.addRecord();
  }
  return true; 
}
