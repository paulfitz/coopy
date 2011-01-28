
#include <coopy/DataSheet.h>

using namespace coopy::store;

std::string DataSheet::encode(const SheetStyle& style) const {
  std::string delim = style.getDelimiter();
  std::string result = "";
  for (int y=0;y<height();y++) {
    for (int x=0;x<width();x++) {
      if (x>0) {
	result += delim;
      }
      result += encodeCell(cellSummary(x,y),style);
    }
    result += "\r\n"; // use Windows encoding, since UNIX is more forgiving
  }
  return result;
}

std::string DataSheet::encodeCell(const SheetCell& c, 
				  const SheetStyle& style) {
  std::string str = c.text;
  std::string delim = style.getDelimiter();
  bool need_quote = false;
  for (size_t i=0; i<str.length(); i++) {
    char ch = str[i];
    if (ch=='"'||ch=='\''||ch==delim[0]||ch=='\r'||ch=='\n'||ch=='\t'||ch==' ') {
      need_quote = true;
      break;
    }
  }
  std::string nil = style.getNullToken();
  /*
  if (str==nil) {
    need_quote = !c.escaped;
  }
  */
  //printf("encoding [%s] [%d]\n", str.c_str(), c.escaped);
  if (str=="" && c.escaped) {
    if (style.haveNullToken()) {
      return style.getNullToken();
    }
  }
  std::string result = "";
  if (!c.escaped) {
    if (style.quoteCollidingText()) {
      int score = 0;
      for (score=0; score<(int)str.length(); score++) {
	if (str[score]!='_') {
	  break;
	}
      }
      if (str.substr(score,str.length())==nil) {
	str = std::string("_") + str;
      }
    }
  }
  if (need_quote) { result += '"'; }
  std::string line_buf = "";
  for (size_t i=0; i<str.length(); i++) {
    char ch = str[i];
    if (ch=='"') {
      result += '"';
    }
    if (ch!='\r'&&ch!='\n') {
      if (line_buf.length()>0) {
	result += line_buf;
	line_buf = "";
      }
      result += ch;
    } else {
      if (style.shouldTrimEnd()) {
	line_buf+=ch;
      } else {
	result+=ch;
      }
    }
  }
  if (need_quote) { result += '"'; }
  return result;
}


bool DataSheet::copyData(const DataSheet& src) {
  if (!canWrite()) {
    fprintf(stderr,"Copy failed, cannot write to target\n");
    return false;
  }
  if (width()!=src.width()||height()!=src.height()) {
    if (canResize()) {
      resize(src.width(),src.height());
    }
  }
  if (width()!=src.width()||height()!=src.height()) {
    fprintf(stderr,"Copy failed, src and target are not the same size\n");
    return false;
  }
  for (int i=0; i<src.height(); i++) {
    for (int j=0; j<src.width(); j++) {
      setCell(j,i,src.getCell(j,i));
    }
  }
  return true;
}

bool DataSheet::applyRowCache(const RowCache& cache, int row) {
  if (row==-1) {
    fprintf(stderr,"Sheet requires a row\n");
    return false;
  }
  while (row>=height()) {
    insertRow(RowRef(-1));
    //dbg_printf("adding row..\n");
  }
  for (int i=0; i<(int)cache.cells.size(); i++) {
    if (cache.flags[i]) {
      setCell(i,row,cache.cells[i]);
    }
  }
  return true;
}


Poly<SheetRow> DataSheet::insertRow() {
  //printf("attaching sheet for insertion %s\n", desc().c_str());
  SheetRow *row = new CacheSheetRow(this,height());
  COOPY_ASSERT(row);
  return Poly<SheetRow>(row,true);
}

