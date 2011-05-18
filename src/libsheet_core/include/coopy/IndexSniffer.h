#ifndef COOPY_INDEX_SNIFFER
#define COOPY_INDEX_SNIFFER

#include <coopy/DataSheet.h>
#include <coopy/CompareFlags.h>
#include <coopy/NameSniffer.h>

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
  const coopy::cmp::CompareFlags& cflags;
  NameSniffer& sniffer;
public:
 IndexSniffer(const DataSheet& sheet,
	      const coopy::cmp::CompareFlags& cflags,
	      NameSniffer& sniffer) : sheet(sheet),
    cflags(cflags), sniffer(sniffer) {
    guessed = false;
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


