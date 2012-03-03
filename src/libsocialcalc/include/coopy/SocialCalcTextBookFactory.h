#ifndef COOPY_SOCIALCALCTEXTBOOKFACTORY
#define COOPY_SOCIALCALCTEXTBOOKFACTORY

#include <coopy/TextBookFactory.h>
#include <coopy/CsvWrite.h>
#include <coopy/Dbg.h>

namespace coopy {
  namespace store {
    namespace socialcalc {
      class SocialCalcTextBookFactory;
    }
  }
}

class coopy::store::socialcalc::SocialCalcTextBookFactory : public TextBookFactory {
  virtual std::string getName() {
    return "socialcalc";
  }

  virtual TextBook *open(AttachConfig& config, AttachReport& report);
};

#endif


