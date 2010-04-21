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

  std::string getDelimiter() const {
    return delim;
  }
};

#endif
