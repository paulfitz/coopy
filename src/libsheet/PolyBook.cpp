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
