#ifndef SHEETSTYLE_INC
#define SHEETSTYLE_INC

#include <string>

#include <coopy/Property.h>

namespace coopy {
  namespace store {
    class SheetStyle;
  }
}

class coopy::store::SheetStyle {
private:
  std::string delim;
  std::string nullToken;
  bool haveNull;
public:
  SheetStyle() {
    delim = ",";
    nullToken = "NULL";
    haveNull = false;
  }

  bool setFromFilename(const char *fname);

  void setFromInspection(const char *buffer, int len);

  void setFromProperty(const Property& config);

  std::string getDelimiter() const {
    return delim;
  }

  std::string getNullToken() const {
    return nullToken;
  }

  bool haveNullToken() const {
    return haveNull;
  }

  static const SheetStyle defaultStyle;
};

#endif
