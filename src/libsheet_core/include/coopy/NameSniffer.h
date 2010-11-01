#ifndef COOPY_NAME_SNIFFER
#define COOPY_NAME_SNIFFER

#include <coopy/DataSheet.h>

namespace coopy {
  namespace store {
    class NameSniffer;
  }
}

class coopy::store::NameSniffer {
private:
  DataSheet& sheet;
public:
  NameSniffer(DataSheet& sheet) : sheet(sheet) {
  }

  virtual std::string suggestColumnName(int col);
};

#endif


