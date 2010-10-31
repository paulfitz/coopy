
#include <coopy/CsvTextBook.h>
#include <coopy/CsvFile.h>

using namespace coopy::store;
using namespace std;

bool CsvTextBook::read(const char *fname) {
  CsvSheet index;
  if (CsvFile::read(fname,index)!=0) {
    fprintf(stderr,"Failed to read %s\n", fname);
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
    string cmd = index.cell(0,y);
    if (cmd=="table") {
      string key = index.cell(1,y);
      //printf("key %s\n", key.c_str());
      string f = root + key + ".csv";
      dbg_printf("Adding %s [%s]\n", key.c_str(), f.c_str());
      name2index[key] = (int)sheets.size();
      CsvSheet *data = new CsvSheet;
      if (data==NULL) {
	fprintf(stderr,"Failed to allocated data sheet\n");
	return false;
      }
      if (CsvFile::read(f.c_str(),*data)!=0) {
	fprintf(stderr,"Failed to read %s referenced from %s\n", f.c_str(),
		fname);
	delete data;
	return false;
      }
      PolySheet sheet(data,true);
      sheets.push_back(sheet);
      names.push_back(key);
    }
  }
  return true;
}

bool CsvTextBook::open(const Property& config) {
  if (!config.check("file")) return false;
  return read(config.get("file").asString().c_str());
}

