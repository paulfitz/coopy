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
  bool quoteCollision;
  bool trimEnd;
  bool markHeader;
public:
  SheetStyle() {
    delim = ",";
    nullToken = "NULL";
    haveNull = true;
    markHeader = false;
    quoteCollision = true;
    trimEnd = true; // needed for a Gnumeric bug in parsing CSV like this:
    //1,2,3
    //"hi","there
    //you
    //","oops"
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

  bool quoteCollidingText() const {
    return quoteCollision;
  }

  bool shouldTrimEnd() const {
    return trimEnd;
  }

  bool shouldMarkHeader() const {
    return markHeader;
  }

  void setMarkHeader(bool flag) {
    markHeader = flag;
  }

  static const SheetStyle defaultStyle;
};

#endif
