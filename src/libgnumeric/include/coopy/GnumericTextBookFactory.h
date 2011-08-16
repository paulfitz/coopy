#ifndef COOPY_GNUMERICTEXTBOOKFACTORY
#define COOPY_GNUMERICTEXTBOOKFACTORY

#include <coopy/GnumericTextBook.h>
#include <coopy/TextBookFactory.h>

namespace coopy {
  namespace store {
    namespace gnumeric {
      class GnumericTextBookFactory;
    }
  }
}

class coopy::store::gnumeric::GnumericTextBookFactory : public TextBookFactory {
public:
  virtual std::string getName() {
    return "gnumeric";
  }

  virtual TextBook *open(AttachConfig& config, AttachReport& report);
};

#endif
