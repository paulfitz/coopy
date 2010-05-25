#ifndef COOPY_SHORTTEXTBOOK
#define COOPY_SHORTTEXTBOOK

#include <coopy/TextBook.h>
#include <coopy/CsvSheet.h>

class ShortTextBook : public TextBook {
public:
  CsvSheet sheet;

  virtual std::vector<std::string> getNames() {
    std::vector<std::string> result;
    result.push_back("single");
    return result;
  }

  virtual bool readSheet(const std::string& name, TextSheet& sheet) {
    if (name=="single") {
      sheet.copy(this->sheet);
      return true;
    }
    sheet.clear();
    return false;
  }
};

#endif
