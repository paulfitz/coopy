#ifndef COOPY_GNUMERICTEXTBOOKFACTORY
#define COOPY_GNUMERICTEXTBOOKFACTORY

#include <coopy/GnumericTextBook.h>
#include <coopy/TextBookFactory.h>

namespace coopy {
  namespace store {
    class GnumericTextBookFactory;
  }
}

class coopy::store::GnumericTextBookFactory : public TextBookFactory {
public:
  virtual std::string getName() {
    return "gnumeric";
  }

  virtual TextBook *open(AttachConfig& config, AttachReport& report);
};

#endif
