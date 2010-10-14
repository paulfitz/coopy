#include <coopy/GnumericTextBookFactory.h>

using namespace coopy::store;
using namespace std;

bool GnumericTextBookFactory::check(const char *fname, 
				    const char *ext,
				    const char *data) {
  // toy test
  return (string(ext)==".xls"||string(ext)==".xlsx");
}

TextBook *GnumericTextBookFactory::load(const char *fname) {
  GnumericTextBook *book = new GnumericTextBook;
  if (book==NULL) return book;
  if (!book->load(fname)) {
    delete book;
    book = NULL;
  }
  return book;
}

