#ifndef COOPY_NAME_SNIFFER
#define COOPY_NAME_SNIFFER

#include <coopy/DataSheet.h>

#include <vector>
#include <string>

namespace coopy {
  namespace store {
    class NameSniffer;
  }
}

class coopy::store::NameSniffer {
private:
  const DataSheet& sheet;
  std::vector<std::string> names;
  bool embed;
  bool fake;
  int div;
public:
  NameSniffer(const DataSheet& sheet) : sheet(sheet) {
    div = -1;
    fake = true;
    sniff();
  }

  void sniff(); 

  int getHeaderHeight() const {
    if (div<0) return 0;
    return div+1;
  }

  virtual std::string suggestColumnName(int col);

  virtual std::vector<std::string> suggestNames() const {
    return names;
  }

  bool isEmbedded() {
    return embed;
  }

  bool isFake() {
    return fake;
  }
};

#endif


