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

  PatchParser(Patcher *patcher,
	      const std::vector<std::string>& cmd) : 
    patcher(patcher), oneliners(cmd)
    {}

  PatchParser(Patcher *patcher,
	      const std::string& fname) :
    patcher(patcher), fname(fname)
    {}
    
  bool apply();

private:
  bool applyCsv();

  bool applyTdiff();

  bool applyColor();
};

#endif


