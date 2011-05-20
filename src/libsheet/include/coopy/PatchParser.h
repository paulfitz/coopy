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
  std::string oneliner;

  PatchParser(Patcher *patcher,
	      const std::string& fname,
	      const std::string& cmd) : 
    patcher(patcher), fname(fname), oneliner(cmd)
    {}
    
  bool apply();

private:
  bool applyCsv();

  bool applyTdiff();

  bool applyColor();
};

#endif


