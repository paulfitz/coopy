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
public:
  NameSniffer(const DataSheet& sheet) : sheet(sheet) {
    sniff();
  }

  void sniff(); 

  virtual std::string suggestColumnName(int col);

  bool isEmbedded() {
    return embed;
  }
};

#endif


