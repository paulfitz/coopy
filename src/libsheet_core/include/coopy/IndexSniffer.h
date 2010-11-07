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
  std::vector<int> offsets;
  int len;
  bool guessed;
public:
  IndexSniffer(const DataSheet& sheet) : sheet(sheet) {
    sniff();
  }

  void sniff(); 

  int length() {
    return len;
  }

  int suggestIndex(int at) const {
    return offsets[at];
  }

  std::vector<int> suggestIndexes() const {
    return offsets;
  }

  bool isGuessed() {
    return guessed;
  }
};

#endif


