#ifndef COOPY_PATCHPARSER_INC
#define COOPY_PATCHPARSER_INC

#include <coopy/Patcher.h>
#include <coopy/Reader.h>
#include <coopy/Property.h>
#include <coopy/FormatSniffer.h>

#include <string>

namespace coopy {
  namespace cmp {
    class PatchParser; 
  }
}

class coopy::cmp::PatchParser {
public:
  coopy::format::FormatSniffer sniffer;
  Patcher *patcher;
  coopy::store::Property config;
  std::string fname;
  std::vector<std::string> oneliners;
  bool use_oneliners;
  const coopy::cmp::CompareFlags& flags;
  coopy::store::TextBook *preread_book;

  PatchParser(Patcher *patcher,
	     coopy::cmp::CompareFlags& flags) : 
    patcher(patcher),
    use_oneliners(false),
    flags(flags),
    preread_book(0/*NULL*/)
  {}

  PatchParser(Patcher *patcher,
	      const std::vector<std::string>& cmd,
	      coopy::cmp::CompareFlags& flags) : 
    patcher(patcher), oneliners(cmd), 
    use_oneliners(true),
    flags(flags),
    preread_book(0/*NULL*/)
  {}

  PatchParser(Patcher *patcher,
	      const std::string& fname,
	      coopy::cmp::CompareFlags& flags) :
    patcher(patcher), fname(fname), 
    use_oneliners(false),
    flags(flags),
    preread_book(0/*NULL*/)
  {}

  PatchParser(Patcher *patcher,
	      coopy::store::TextBook *book,
	      coopy::cmp::CompareFlags& flags) :
    patcher(patcher), fname(""), 
    use_oneliners(false),
    flags(flags),
    preread_book(book)
  {}
    
  // deprecated, undesirable name in python
  bool apply();

  bool applyPatch() {
    return apply();
  }

  bool applyHiliteBook(coopy::store::TextBook& patch);

private:
  bool applyCsv();

  bool applyTdiff();

  bool applyColor();
};

#endif


