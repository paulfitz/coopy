
#include <coopy/DataSheet.h>
#include <coopy/Sha1Generator.h>

using namespace coopy::store;

std::string DataSheet::encode(const SheetStyle& style) const {
  std::string delim = style.getDelimiter();
  std::string eol = style.getEol();
  std::string result = "";
  int last = height()-1;
  for (int y=0;y<height();y++) {
    std::string line = "";
    for (int x=0;x<width();x++) {
      if (x>0) {
	line += delim;
      }
      line += encodeCell(cellSummary(x,y),style);
    }
    int len = line.length();
    if (style.shouldEolAtEof()||y!=last) {
      line += eol;
    }
    if (style.shouldMarkHeader()) {
      SheetSchema *schema = getSchema();
      if (schema!=NULL) {
	if (schema->headerHeight()>=0) {
	  if (schema->headerHeight()==y) {
	    if (len<3) len = 3;
	    if (len>79) len = 79;
	    for (int i=0; i<len; i++) {
	      line += '-';
	    }
	    line += eol;
	  }
	}
      }
    }
    result += line;
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
  printf("Apply row %d\n", row);
  if (row==-1) {
    fprintf(stderr,"Sheet requires a row\n");
    return false;
  }
  while (row>=height()) {
    insertRow(RowRef(-1));
    dbg_printf("adding row.. %d\n", height());
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

std::string DataSheet::getHash(bool cache) const {
  DataSheet *mod = (DataSheet *)this;
  if (!cache) mod->hash_cache = "";
  if (hash_cache!="") {
    dbg_printf("(sha1 %ld %s %s)\n", (long int)this, hash_cache.c_str(), desc().c_str());
    return hash_cache;
  }
  dbg_printf("Computing sha1\n");
  Sha1Generator sha1;
  for (int y=0;y<height();y++) {
    std::string txt;
    for (int x=0;x<width();x++) {
      SheetCell cell = cellSummary(x,y);
      txt += cell.toString();
    }
    sha1.add(txt);
  }
  std::string key = sha1.finish();
  dbg_printf("sha1 %ld %s %s\n", (long int)this, key.c_str(), desc().c_str());
  if (cache) {
    mod->hash_cache = key;
  }
  return key;
}

