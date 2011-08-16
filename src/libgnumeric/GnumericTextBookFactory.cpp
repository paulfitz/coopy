#include <coopy/GnumericTextBookFactory.h>

using namespace coopy::store;
using namespace coopy::store::gnumeric;
using namespace std;

TextBook *GnumericTextBookFactory::open(AttachConfig& config, 
					AttachReport& report) {
  //config.show();
  string ext = config.ext;
  report.applicable = true;
  if (config.shouldCreate) {
    report.errorCreateNotImplemented("gnumeric");
    return NULL;
  }
  if (config.shouldWrite && !config.shouldRead) {
    if (config.prevBook!=NULL) {
      GnumericTextBook *prev = dynamic_cast<GnumericTextBook *>(config.prevBook);
      if (prev!=NULL) {
	//printf("FAST TRACK\n");
	prev->save(config.fname.c_str(),NULL);
	report.success = true;
	return prev;
      }
    }
  }
  GnumericTextBook *book = new GnumericTextBook;
  if (book==NULL) return book;
  bool ok = false;
  if (book->load(config.fname.c_str())) {
    ok = true;
  } else {
    if (config.canCreate) {
      if (book->create()) {
	ok = true;
      }
    }
  }
  if (!ok) {
    delete book;
    book = NULL;
  } else {
    if (config.prevBook!=NULL) {
      book->copy(*config.prevBook,config.options);
      if (config.shouldWrite) {
	book->save(config.fname.c_str(),NULL);
      }
    }
  }
  if (book!=NULL) {
    report.success = true;
  }
  return book;
}


