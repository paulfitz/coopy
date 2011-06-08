#ifndef COOPY_NAME_SNIFFER
#define COOPY_NAME_SNIFFER

#include <coopy/DataSheet.h>
#include <coopy/CompareFlags.h>

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
  std::vector<int> subset_index;
  std::vector<ColumnType> ct;
  const coopy::cmp::CompareFlags& flags;
  bool embed;
  bool fake;
  bool sniffed;
  int div;
public:
 NameSniffer(const DataSheet& sheet, 
	     const coopy::cmp::CompareFlags& flags,
	     bool autosniff=true) : sheet(sheet), flags(flags) {
    div = -1;
    fake = true;
    sniffed = false;
    if (autosniff) {
      sniff();
    }
  }

  void sniff(); 

  int getHeaderHeight() const {
    if (div<0) return 0;
    return div+1;
  }

  int dataWidth() const {
    return sheet.width();
  }

  int dataHeight() const {
    if (div<0) return sheet.height();
    return sheet.height()-(div+1);
  }

  virtual std::string suggestColumnName(int col);

  virtual const std::vector<std::string>& suggestNames() const {
    return names;
  }

  virtual const std::vector<int>& getSubset() const {
    return subset_index;
  }

  virtual const std::vector<ColumnType>& suggestTypes() const {
    return ct;
  }

  bool isEmbedded() {
    return embed;
  }

  bool isFake() {
    return fake;
  }
  
  bool hasSubset() const {
    return subset_index.size()>0;
  }

  bool subset(std::vector<std::string>& names);
};

#endif


