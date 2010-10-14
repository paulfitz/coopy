
#include <coopy/CsvTextBook.h>
#include <coopy/CsvFile.h>

using namespace coopy::store;
using namespace std;

bool CsvTextBook::read(const char *fname) {
  CsvSheet index;
  if (CsvFile::read(fname,index)!=0) {
    return false;
  }
  string root = fname;
  if (root.rfind("/")!=string::npos) {
    root = root.substr(0,root.rfind("/")+1);
  } else if (root.rfind("\\")!=string::npos) {
    root = root.substr(0,root.rfind("\\")+1);
  } else {
    root = "";
  }
  for (int y=0; y<index.height(); y++) {
    string key = index.cell(0,y);
    printf("key %s\n", key.c_str());
    string f = root + key + ".csv";
    printf("key %s -> %s\n", key.c_str(), f.c_str());
    name2index[key] = (int)sheets.size();
    sheets.push_back(CsvSheet());
    if (CsvFile::read(f.c_str(),sheets.back())!=0) {
      fprintf(stderr,"Failed to read %s referenced from %s\n", f.c_str(),
	      fname);
      return false;
    }
    names.push_back(key);
  }
  return true;
}

