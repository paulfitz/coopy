#ifndef COOPY_CSVTEXTBOOK
#define COOPY_CSVTEXTBOOK

#include <coopy/TextBook.h>
#include <coopy/CsvSheet.h>

#include <vector>
#include <map>

namespace coopy {
  namespace store {
    class CsvTextBook;
  }
}

class coopy::store::CsvTextBook : public TextBook {
public:
  std::vector<PolySheet> sheets;
  std::vector<std::string> names;
  std::map<std::string,int> name2index;

  virtual std::vector<std::string> getNames() {
    return names;
  }

  virtual PolySheet readSheet(const std::string& name) {
    if (name2index.find(name)!=name2index.end()) {
      return sheets[name2index[name]];
    }
    return PolySheet();
  }
  
  bool read(const char *fname);

  virtual bool open(const Property& config);
};

#endif
