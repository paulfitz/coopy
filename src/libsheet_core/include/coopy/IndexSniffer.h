#ifndef COOPY_INDEX_SNIFFER
#define COOPY_INDEX_SNIFFER

#include <coopy/DataSheet.h>

#include <vector>
#include <string>

namespace coopy {
  namespace store {
    class IndexSniffer;
  }
}

class coopy::store::IndexSniffer {
private:
  const DataSheet& sheet;
  std::vector<int> flags;
  bool guessed;
public:
  IndexSniffer(const DataSheet& sheet) : sheet(sheet) {
    sniff();
  }

  void sniff(); 

  // this is a poorly-named vector of primary key flags
  std::vector<int> suggestIndexes() const {
    return flags;
  }

  bool isGuessed() {
    return guessed;
  }
};

#endif


