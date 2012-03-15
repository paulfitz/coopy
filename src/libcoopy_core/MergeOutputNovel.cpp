#include <coopy/MergeOutputNovel.h>
#include <coopy/Stringer.h>

#include <ctype.h>

using namespace coopy::cmp;
using namespace coopy::store;
using namespace std;

#define FULL_COLOR (65535)
#define HALF_COLOR (65535/2)

// utf-unfriendly for now.
static std::string trim(const std::string &s1) {
  string s;
  bool have_text = false;
  bool have_space = false;
  for (int i=0; i<(int)s1.length(); i++) {
    char ch = s1[i];
    if (ch<=32||ch>=128) ch = ' ';
    if (ch==' ') {
      if (have_text) {
	have_space = true;
      }
      have_text = false;
    } else {
      if (have_space) {
	s += ' ';
	have_space = false;
      }
      s += tolower(ch);
      have_text = true;
    }
  }
  if (s.length()>0) {
    if (s[s.length()-1]=='/') {
      s = s.substr(0,s.length()-1);
    }
  }
  Stringer::replace(s,"www.","");
  Stringer::replace(s,"https://","http://");
  return s;
}

static bool looseCompareUrl(const SheetCell& url, const SheetCell& b) {
  if (!url.meta.isValid()) return false;
  if (!url.meta->isUrl()) return false;
  if (trim(url.meta->getUrl())==trim(b.text)) return true;
  if (!b.meta.isValid()) return false;
  if (!b.meta->isUrl()) return false;
  if (trim(url.meta->getUrl())==trim(b.meta->getUrl())) return true;
  if (trim(url.meta->getUrl())==trim(b.meta->getText())) return true;
  return false;
}


static bool looseCompare(const SheetCell& a, const SheetCell& b) {
  if (a==b) return true;
  if (a.escaped&&b.escaped) return true;
  string ta = trim(a.text);
  string tb = trim(b.text);
  if (a.escaped&&tb=="") return true;
  if (b.escaped&&ta=="") return true;
  if (ta==tb) return true;
  if (looseCompareUrl(a,b)) return true;
  if (looseCompareUrl(b,a)) return true;
  return false;
}

bool MergeOutputNovel::formatSheet() {
  bool success = true;
  PolySheet sheet = getSheet();
  sheet.hideHeaders();
  if (noted.find(sheet_name)==noted.end()) {
    noted[sheet_name] = 1;
    int w = sheet.width();
    for (int x=w;x>=0;x--) {
      string name = "";
      if (x==0) name = base_col_name;
      if (remote_name.find(x-1)!=remote_name.end()) {
	name = string("("+remote_name[x-1]+")");
      }
      ColumnRef col;
      int nx = x;
      if (nx>=w) nx = -1;
      if (name!="") {
	col = sheet.insertColumn(ColumnRef(nx),ColumnInfo(name));
      } else {
	col = sheet.insertColumn(ColumnRef(nx));
      }
      if (!col.isValid()) {
	fprintf(stderr,"Problem creating column\n");
	success = false;
	break;
      }
      for (int y=0; y<sheet.height(); y++) {
	sheet.cellString(x,y,""); // for consistency
      }
    }
  }
  return success;
}


bool MergeOutputNovel::declareLink(const LinkDeclare& decl) {

  if (!decl.column) {
    if (col_map.size()==0) {
      dbg_printf("Columns match\n");
      PolySheet sheet = getSheet();
      for (int i=0; i<sheet.width(); i++) {
	col_map[i] = i;
	CompareFlags flags;
	NameSniffer names(sheet,flags);
	remote_name[i] = names.suggestColumnName(i);
      }
    }
    formatSheet();
  }

  dbg_printf("LINK %d // %d %d %d // %s %s %s\n",
	     decl.column,
	     decl.rc_id_pivot,
	     decl.rc_id_local,
	     decl.rc_id_remote,
	     decl.rc_str_pivot.c_str(),
	     decl.rc_str_local.c_str(),
	     decl.rc_str_remote.c_str());

  if (decl.column) {
    //int xoff = 1;
    //PolySheet sheet = getSheet();
    int x = decl.rc_id_local;
    if (x>=0) {
      int x2 = decl.rc_id_remote;
      if (x2>=0) {
	col_map[x] = x2;
	remote_name[x] = decl.rc_str_remote;
	/*
	x += xoff;
	if (sheet.height()>0) {
	  sheet.cellString(x,0,sheet.cellString(x,0) + "::" +
			   SheetCell(x2+1).toString());
	}
	Poly<Appearance> appear = sheet.getColAppearance(x);
	if (appear.isValid()) {
	  appear->begin();
	  appear->setBackgroundRgb16(HALF_COLOR,
				     FULL_COLOR,
				     HALF_COLOR,
				     AppearanceRange::full());
	  appear->end();
	}
	*/
      }
    }
    return true;
  }

  PolySheet sheet = getSheet();
  sheet.hideHeaders();

  if (decl.rc_id_local>=0) {
    if (decl.rc_id_remote>=0) {
      int y = decl.rc_id_local;
      if (y>=sheet.height()) {
	fprintf(stderr,"Out of range row? %d\n", y);
	return false;
      }
      if (sheet.width()<1) {
	fprintf(stderr,"No space in sheet\n");
	return false;
      }
      if (sheet.cellString(0,y)==base_col_name) return true;
      sheet.cellSummary(0,y,SheetCell(decl.rc_id_remote+2));
      Poly<Appearance> appear = sheet.getRowAppearance(y);
      if (appear.isValid()) {
	appear->begin();
	appear->setBackgroundRgb16(HALF_COLOR,
				   FULL_COLOR,
				   HALF_COLOR,
				   AppearanceRange::full());
	appear->end();
      }
      for (int i=0; i<sheet.width(); i++) {
	int yy = decl.rc_id_remote;
	if (col_map.find(i)==col_map.end()) continue;
	int xx = col_map[i];
	SheetCell cell = decl.remote.cellSummary(xx,yy);
	//printf("WORKING on %d (%d %d : %s)\n", i, xx, yy, cell.toString().c_str());
	sheet.cellSummary(i*2+2,y,cell);
	if (!looseCompare(cell,sheet.cellSummary(i*2+1,y))) {
	  for (int k=i*2+1; k<=i*2+2; k++) {
	    Poly<Appearance> appear = sheet.getCellAppearance(k,y);
	    if (appear.isValid()) {
	      appear->begin();
	      appear->setBackgroundRgb16(FULL_COLOR,
					 HALF_COLOR,
					 HALF_COLOR,
					 AppearanceRange::full());
	      appear->end();
	    }
	  }
	}
      }
    }
  }
  return true;
}
