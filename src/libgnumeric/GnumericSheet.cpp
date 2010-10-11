#include <coopy/Dbg.h>

#include <coopy/GnumericSheet.h>

extern "C" {
#include <coopy/gnumeric_link.h>
}

using namespace coopy::store;

using namespace std;

#define SHEET(x) ((GnumericSheetPtr)(x))

GnumericSheet::GnumericSheet(void *sheet) {
  w = h = 0;
  implementation = sheet;
  if (implementation!=NULL) {
    gnumeric_sheet_get_size(SHEET(implementation),&w,&h);
    dbg_printf("Sheet size is %d %d\n", w, h);
  }
}

GnumericSheet::~GnumericSheet() {
}

std::string GnumericSheet::cellString(int x, int y) const {
  char *txt = gnumeric_sheet_get_cell_as_string(SHEET(implementation),
						x, y);
  if (txt==NULL) return "";
  string result = txt;
  gnumeric_free_string(txt);
  return result;
}

bool GnumericSheet::cellString(int x, int y, const std::string& str) {
  return gnumeric_sheet_set_cell_as_string(SHEET(implementation),
					   x, y,
					   str.c_str()) == 0;
}


