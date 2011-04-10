#ifndef COOPY_SHEETVIEW
#define COOPY_SHEETVIEW

#include <coopy/DataSheet.h>
#include <coopy/NameSniffer.h>

namespace coopy {
  namespace store {
    class SheetView;
  }
}

class coopy::store::SheetView {
public:
  DataSheet& sheet;
  NameSniffer& meta;
  std::string sha1;

  SheetView(DataSheet& sheet, NameSniffer& meta, const std::string &sha1) : 
    sheet(sheet), meta(meta), sha1(sha1) {
  }
};

#endif
