#include <coopy/GnumericTextBook.h>
#include <coopy/GnumericSheet.h>

#include <algorithm>

using namespace std;
using namespace coopy::store;
using namespace coopy::store::gnumeric;

extern "C" {
#include <coopy/gnumeric_link.h>
}

GnumericTextBook::GnumericTextBook() {
  gnumeric_init();
  implementation = 0 /*NULL*/;
  dirtyNames = true;
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

bool GnumericTextBook::create() {
  clear();
  implementation = gnumeric_create();
  return (implementation!=NULL);
}

  
bool GnumericTextBook::load(const char *fname) {
  clear();
  dbg_printf("Trying for gnumeric %s\n", fname);
  implementation = gnumeric_load(fname);
  return (implementation!=NULL);
}

void GnumericTextBook::updateNames() {
  if (!dirtyNames) return;
  names.clear();
  GnumericWorkbookPtr book = (GnumericWorkbookPtr)implementation;
  int ct = gnumeric_get_sheet_count(book);
  for (int i=0; i<ct; i++) {
    GnumericSheetPtr sheet = gnumeric_get_sheet(book,i);
    if (sheet==NULL) { 
      names.push_back("Untitled"); 
    } else {
      names.push_back(gnumeric_sheet_get_name(sheet));
    }
  }
  dirtyNames = false;
}

std::vector<std::string> GnumericTextBook::getNames() {
  updateNames();
  return names;
}

PolySheet GnumericTextBook::readSheet(const std::string& name) {
  GnumericSheetPtr ptr = gnumeric_get_sheet_by_name((GnumericWorkbookPtr)implementation, name.c_str());
  if (ptr==NULL) return PolySheet();
  GnumericSheet *sheet = new GnumericSheet(ptr);
  return PolySheet(sheet,true);
}

PolySheet GnumericTextBook::readSheetByIndex(int index) {
  GnumericSheetPtr ptr = gnumeric_get_sheet((GnumericWorkbookPtr)implementation,
					    index);
  if (ptr==NULL) return PolySheet();
  GnumericSheet *sheet = new GnumericSheet(ptr);
  return PolySheet(sheet,true);
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

bool GnumericTextBook::addSheet(const SheetSchema& schema) {
  if (implementation==NULL) return false;
  dbg_printf("gnumerictextbook::addsheet [%s]\n", schema.getSheetName().c_str());
  string name = schema.getSheetName();
  getNames();
  if (find(names.begin(),names.end(),name)!=names.end()) {
    fprintf(stderr,"failed to add sheet\n");
    return false;
  }
  GnumericWorkbookPtr book = (GnumericWorkbookPtr)implementation;
  GnumericSheetPtr sheet = gnumeric_add_sheet(book,name.c_str());
  if (sheet==NULL) {
    fprintf(stderr,"failed to add sheet\n");
    return false;
  }
  dirtyNames = true;
  /*
  GnumericSheet s(sheet);
  for (int i=0; i<schema.getColumnCount(); i++) {
    ColumnInfo ci = schema.getColumnInfo(i);
    string cname = ci.getName();
    if (cname=="" || cname=="*") {
      fprintf(stderr,"Invalid/unknown column name\n");
      return false;
    }
    s.cellString(i,0,cname);
  }
  */

  /*
  int ct = schema.getColumnCount();
  GnumericSheet s(sheet);
  for (int i=1; i<ct; i++) {
    //printf("INS\n");
    s.insertColumn(ColumnRef(-1));
  }
  //dbg_printf("%d columns requested, %d prepared\n", ct, s.width());
  */
  return true;
 }



