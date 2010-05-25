#ifndef COOPY_SHORTTEXTBOOK
#define COOPY_SHORTTEXTBOOK

#include <coopy/TextBook.h>

class ShortTextBook : public TextBook {
public:
  TextSheet sheet;

  virtual std::vector<std::string> getNames() {
    std::vector<std::string> result;
    result.push_back("single");
  }

  virtual bool readSheet(const std::string& name, TextSheet& sheet) {
    if (name=="single") {
      this->sheet = sheet;
      return true;
    }
    sheet.clear();
    return false;
  }
};

#endif
