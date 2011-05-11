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
  bool tmp;
  return cellString(x,y,tmp);
}


std::string GnumericSheet::cellString(int x, int y, 
				      bool& escaped) const {
  char *txt = gnumeric_sheet_get_cell_as_string(SHEET(implementation),
						x, y);
  escaped = (txt==NULL);
  if (txt==NULL) return "NULL";
  string result = txt;
  //printf("GOT [%s]\n", result.c_str());
  // if txt is blank, it seems like we should not free it?
  //if (txt[0]!='\0') {
  gnumeric_free_string(txt);
  //}
  return result;  
}


bool GnumericSheet::cellString(int x, int y, const std::string& str,
			       bool escaped) {
  //printf("Setting %d %d to %s\n", x, y, str.c_str());
  if (!escaped) {
    return gnumeric_sheet_set_cell_as_string(SHEET(implementation),
					     x, y,
					     str.c_str()) == 0;
  }
  return gnumeric_sheet_remove_cell(SHEET(implementation),
				    x, y);
}


ColumnRef GnumericSheet::moveColumn(const ColumnRef& src, 
				    const ColumnRef& base) {
  int i0 = src.getIndex();
  int i1 = base.getIndex();
  if (i0<0) return ColumnRef();
  if (i1<0) i1 = width();
  int i2 = i1;
  if (i2>i0) {
    i2--;
  }
  gnumeric_move_column(SHEET(implementation),i0,i2);

  return ColumnRef(i2);
}


bool GnumericSheet::deleteColumn(const ColumnRef& column) {
  int index = column.getIndex();
  if (index>=w) return true;
  bool ok = gnumeric_delete_column(SHEET(implementation),index) == 0;
  if (!ok) return false;
  w--;
  return true;
}

ColumnRef GnumericSheet::insertColumn(const ColumnRef& base) {
  //printf("inserting column...\n");
  int index = base.getIndex();
  if (index==-1) { 
    index = width();
  }
  if (index>=w) {
    w = index+1;
    return ColumnRef(index);
  }
  bool ok = gnumeric_insert_column(SHEET(implementation),index) == 0;
  if (!ok) { printf("fail\n"); return ColumnRef(); }
  w++;
  return ColumnRef(index);
}

RowRef GnumericSheet::insertRow(const RowRef& base) {
  //printf("inserting row\n");
  int index = base.getIndex();
  if (index==-1) {
    h++;
    return RowRef(h-1);
  }
  if (index>=h) {
    h = index+1;
    return RowRef(index);
  }
  bool ok = gnumeric_insert_row(SHEET(implementation),index) == 0;
  //if (!ok) { return RowRef(); }
  h++;
  //printf("inserted row\n");
  return RowRef(index);  
}

bool GnumericSheet::deleteRow(const RowRef& src) {
  int index = src.getIndex();
  if (index>=h) return true;
  bool ok = gnumeric_delete_row(SHEET(implementation),index) == 0;
  if (!ok) return false;
  h--;
  return true;
}

RowRef GnumericSheet::moveRow(const RowRef& src, const RowRef& base) {
  int i0 = src.getIndex();
  int i1 = base.getIndex();
  if (i0<0) return RowRef();
  if (i1<0) i1 = height();
  int i2 = i1;
  if (i2>i0) {
    i2--;
  }
  gnumeric_move_row(SHEET(implementation),i0,i2);

  return RowRef(i2);
}

bool GnumericSheet::deleteData() {
  bool ok = gnumeric_delete_data(SHEET(implementation)) == 0;
  h = 0;
  return ok;
}


#define APPEAR_CELL 0
#define APPEAR_ROW 1
#define APPEAR_COL 2

class GnumericAppearance : public Appearance {
public:
  GnumericSheetPtr sheet;
  int x, y;
  int mode;
  GnumericStylePtr style;

  static GnumericAppearance *forCell(GnumericSheetPtr sheet, int x, int y) {
    GnumericAppearance *r = new GnumericAppearance(sheet,x,y,APPEAR_CELL);
    COOPY_ASSERT(r!=NULL);
    return r;
  }

  static GnumericAppearance *forRow(GnumericSheetPtr sheet, int y) {
    GnumericAppearance *r = new GnumericAppearance(sheet,0,y,APPEAR_ROW);
    COOPY_ASSERT(r!=NULL);
    return r;
  }

  static GnumericAppearance *forCol(GnumericSheetPtr sheet, int x) {
    GnumericAppearance *r = new GnumericAppearance(sheet,x,0,APPEAR_COL);
    COOPY_ASSERT(r!=NULL);
    return r;
  }

  GnumericAppearance(GnumericSheetPtr sheet, int x, int y, int mode) : 
    sheet(sheet),x(x), y(y), mode(mode) {
  }

  bool begin() {
    style = gnumeric_sheet_get_style(sheet,x,y);
    return true;
  }

  virtual bool setForegroundRgb16(int r, int g, int b, 
				  const AppearanceRange& range) {    
    gnumeric_style_set_font_color(style,r,g,b);
    return true;
  }
  
  virtual bool setBackgroundRgb16(int r, int g, int b, 
				  const AppearanceRange& range) {
    gnumeric_style_set_back_color(style,r,g,b);
    return true;
  }

  virtual bool setWeightBold(bool flag, const AppearanceRange& range) {
    gnumeric_style_set_font_bold(style,flag);
    return true;
  }

  virtual bool setStrikethrough(bool flag, const AppearanceRange& range) {
    gnumeric_style_set_font_strike(style,flag);
    return true;
  }

  bool end() {
    switch (mode) {
    case APPEAR_CELL:
      gnumeric_sheet_set_style(sheet,style,x,y);
      break;
    case APPEAR_ROW:
      gnumeric_sheet_set_row_style(sheet,style,y);
      break;
    case APPEAR_COL:
      gnumeric_sheet_set_col_style(sheet,style,x);
      break;
    }
    return true;
  }
};

Poly<Appearance> GnumericSheet::getCellAppearance(int x, int y) {
  return Poly<Appearance>(GnumericAppearance::forCell(SHEET(implementation),x,y),
			  true);
}

Poly<Appearance> GnumericSheet::getRowAppearance(int y) {
  return Poly<Appearance>(GnumericAppearance::forRow(SHEET(implementation),y),
			  true);
}

Poly<Appearance> GnumericSheet::getColAppearance(int x) {
  return Poly<Appearance>(GnumericAppearance::forCol(SHEET(implementation),x),
			  true);
}


