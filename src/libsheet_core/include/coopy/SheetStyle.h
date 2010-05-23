#ifndef SHEETSTYLE_INC
#define SHEETSTYLE_INC

#include <string>

class SheetStyle {
private:
  std::string delim;
public:
  SheetStyle() {
    delim = ",";
  }

  void setFromFilename(const char *fname);

  void setFromInspection(const char *buffer, int len);

  std::string getDelimiter() const {
    return delim;
  }

  static const SheetStyle defaultStyle;
};

#endif
