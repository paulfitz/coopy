#ifndef CSVSTYLE_INC
#define CSVSTYLE_INC

#include <string>

class CsvStyle {
private:
  std::string delim;
public:
  CsvStyle() {
    delim = ",";
  }

  void setFromFilename(const char *fname);

  void setFromInspection(const char *buffer, int len);

  std::string getDelimiter() const {
    return delim;
  }

  static const CsvStyle defaultStyle;
};

#endif
