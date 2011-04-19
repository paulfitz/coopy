#ifndef COOPY_PATCHPARSER_INC
#define COOPY_PATCHPARSER_INC

#include <coopy/Patcher.h>
#include <coopy/Reader.h>
#include <coopy/Property.h>

namespace coopy {
  namespace cmp {
    class PatchParser; 
  }
}

class coopy::cmp::PatchParser {
public:
  Patcher *patcher;
  coopy::format::Reader *reader;
  coopy::store::Property config;

  PatchParser(Patcher *patcher,
	      coopy::format::Reader *reader) : patcher(patcher), reader(reader)
    {}
    
  bool apply();

private:
  bool applyCsv();

  bool applyTdiff();
};

#endif


