#include <coopy/PolyBook.h>
#include <coopy/ShortTextBook.h>
#include <coopy/CsvFile.h>

#include <stdlib.h>

using namespace std;

bool PolyBook::read(const char *fname) {
  clear();
  string name = fname;
  if (name.length()>=4) {
    string ext = name.substr(name.length()-4);
    for (size_t i=0; i<ext.length(); i++) {
      ext[i] = tolower(ext[i]);
    }
    if (ext==".db") {
      // try to load an sqlite book
    }
    if (book==NULL) {
      ShortTextBook *b = new ShortTextBook();
      if (b==NULL) {
	fprintf(stderr,"Failed to allocate ShortTextBook\n");
	exit(1);
      }
      if (CsvFile::read(fname,b->sheet)!=0) {
	delete b;
	b = NULL;
      } else {
	book = b;
      }
    }
  }
  return book!=NULL;
}


bool PolyBook::write(const char *fname) {
  if (book==NULL) {
    fprintf(stderr,"Nothing to write\n");
    return false;
  }
  vector<string> names = getNames();
  if (names.size()!=1) {
    fprintf(stderr,"Unsupported number of sheets during write: %d\n",
	    names.size());
    return false;
  }
  CsvSheet sheet;
  bool ok = readSheet(names[0],sheet);
  if (!ok) { 
    fprintf(stderr,"Could not access sheet %s\n", names[0].c_str());
    return false;
  }
  return CsvFile::write(sheet,fname)==0;
}

