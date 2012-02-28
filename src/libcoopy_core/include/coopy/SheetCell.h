#ifndef COOPY_SHEETCELL_INC
#define COOPY_SHEETCELL_INC

#include <string>

namespace coopy {
  namespace store {
    class SheetCell;
  }
}

class coopy::store::SheetCell {
public:
  std::string text;
  bool escaped;

  SheetCell() {
    escaped = true;
  }

  SheetCell(const char *text, bool escaped) : text(text), escaped(escaped) {
  }

  SheetCell(const std::string& text, bool escaped) : text(text), 
    escaped(escaped) {
  }

  SheetCell(int x);

  bool operator==(const SheetCell& alt) const {
    if (escaped!=alt.escaped) return false;
    return text==alt.text;
  }

  bool operator!=(const SheetCell& alt) const {
    if (escaped!=alt.escaped) return true;
    return text!=alt.text;
  }
  
  std::string toString() const {
    if (!escaped) {
      return text;
    }
    return std::string("{") + text + "}";
  }

  //operator std::string() const {
  //return toString();
  //}

  int asInt() const;
  
  static SheetCell makeInt(int x);
};

#endif


