#include <coopy/GnumericTextBook.h>
#include <coopy/GnumericSheet.h>

using namespace std;
using namespace coopy::store;

extern "C" {
#include <coopy/gnumeric_link.h>
}

GnumericTextBook::GnumericTextBook() {
  gnumeric_init();
  implementation = 0 /*NULL*/;
}

GnumericTextBook::~GnumericTextBook() {
  clear();
  gnumeric_fini();
}

void GnumericTextBook::clear() {
  if (implementation!=NULL) {
    gnumeric_free((GnumericWorkbookPtr)implementation);
    implementation = NULL;
  }
}
  
bool GnumericTextBook::load(const char *fname) {
  clear();
  printf("Trying for gnumeric %s\n", fname);
  implementation = gnumeric_load(fname);
  return (implementation!=NULL);
}

std::vector<std::string> GnumericTextBook::getNames() {
  vector<string> fake;
  fake.push_back("fake");
  return fake;
}

PolySheet GnumericTextBook::readSheet(const std::string& name) {
  if (name=="fake") {
    GnumericSheet *sheet = 
      new GnumericSheet(gnumeric_get_sheet((GnumericWorkbookPtr)implementation,0));
    return PolySheet(sheet,true);
  }
  return PolySheet();
}

bool GnumericTextBook::save(const char *fname, const char *format) {
  if (implementation==NULL) return false;
  const char *fmt = NULL;
  if (format!=NULL) {
    if (string(format)!="-") {
      fmt = format;
    }
  }
  return gnumeric_save((GnumericWorkbookPtr)implementation,
		       fname,
		       fmt) == 0;
}



