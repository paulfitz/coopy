#include <coopy/GnumericTextBookFactory.h>

using namespace coopy::store;
using namespace std;

TextBook *GnumericTextBookFactory::open(AttachConfig& config, 
					AttachReport& report) {
  string ext = config.ext;
  report.applicable = true;
  if (config.shouldCreate) {
    report.errorCreateNotImplemented("gnumeric");
    return NULL;
  }
  GnumericTextBook *book = new GnumericTextBook;
  if (book==NULL) return book;
  if (!book->load(config.fname.c_str())) {
    delete book;
    book = NULL;
  }
  if (book!=NULL) {
    report.success = true;
  }
  return book;
}


