#ifndef COOPY_PATCHPARSER_INC
#define COOPY_PATCHPARSER_INC

#include <coopy/Patcher.h>
#include <coopy/Reader.h>

namespace coopy {
  namespace cmp {
    class PatchParser; 
  }
}

class coopy::cmp::PatchParser {
public:
  Patcher *patcher;
  coopy::format::Reader *reader;

  PatchParser(Patcher *patcher,
	      coopy::format::Reader *reader) : patcher(patcher), reader(reader)
    {}
    
  bool apply();
};

#endif


