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
  std::string eol;
  std::string nullToken;
  bool haveNull;
  bool quoteCollision;
  bool trimEnd;
  bool markHeader;
  bool eolAtEof;
public:
  SheetStyle() {
    delim = ",";
    nullToken = "NULL";

    eol =  "\r\n"; // use Windows encoding, since UNIX is more forgiving
    // (victim logic, I know...)

    eolAtEof = true;

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

  void setDelimiter(const std::string& delim) {
    this->delim = delim;
  }

  std::string getEol() const {
    return eol;
  }

  void setEol(const std::string& eol) {
    this->eol = eol;
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

  bool shouldEolAtEof() const {
    return eolAtEof;
  }

  void setMarkHeader(bool flag) {
    markHeader = flag;
  }

  bool setEolAtEof(bool flag) {
    eolAtEof = flag;
  }

  static const SheetStyle defaultStyle;
};

#endif
