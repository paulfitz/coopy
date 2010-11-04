#ifndef SHEETSTYLE_INC
#define SHEETSTYLE_INC

#include <string>

namespace coopy {
  namespace store {
    class SheetStyle;
  }
}

class coopy::store::SheetStyle {
private:
  std::string delim;
public:
  SheetStyle() {
    delim = ",";
  }

  bool setFromFilename(const char *fname);

  void setFromInspection(const char *buffer, int len);

  std::string getDelimiter() const {
    return delim;
  }

  static const SheetStyle defaultStyle;
};

#endif
