#ifndef COOPY_TEXTBOOKFACTORY
#define COOPY_TEXTBOOKFACTORY

#include <coopy/TextBook.h>
#include <coopy/TextBookFactory.h>

namespace coopy {
  namespace store {
    class TextBookFactory;
  }
}

class coopy::store::TextBookFactory {
public:
  virtual std::string getName() = 0;

  virtual bool check(const char *fname, 
		     const char *ext,
		     const char *data) = 0;

  virtual TextBook *load(const char *fname) = 0;
};

#endif
