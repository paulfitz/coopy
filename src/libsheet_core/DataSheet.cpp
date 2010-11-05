
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
  if (str==style.getNullToken()) {
    need_quote = !c.escaped;
  }
  //printf("encoding [%s] [%d]\n", str.c_str(), c.escaped);
  if (str=="" && c.escaped) {
    if (style.haveNullToken()) {
      return style.getNullToken();
    }
  }
  std::string result = "";
  if (need_quote) { result += '"'; }
  for (size_t i=0; i<str.length(); i++) {
    char ch = str[i];
    if (ch=='"') {
      result += '"';
    }
    result += ch;
  }
  if (need_quote) { result += '"'; }
  return result;
}
