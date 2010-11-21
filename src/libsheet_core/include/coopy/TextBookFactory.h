#ifndef COOPY_TEXTBOOKFACTORY
#define COOPY_TEXTBOOKFACTORY

#include <coopy/TextBook.h>
#include <coopy/TextBookFactory.h>
#include <coopy/Property.h>
#include <coopy/Dbg.h>

namespace coopy {
  namespace store {
    class TextBookFactory;
    class AttachConfig;
    class AttachReport;
  }
}


class coopy::store::AttachConfig {
public:
  std::string fname;
  std::string ext;
  std::string data;
  Property options;
  bool canCreate;
  bool canOverwrite;
  bool shouldCreate;
  bool shouldRead;
  bool shouldWrite;
  TextBook *prevBook;
  Property prevOptions;

  AttachConfig() {
    canCreate = false;
    canOverwrite = false;
    shouldCreate = false;
    shouldRead = false;
    shouldWrite = false;
    prevBook = NULL;
  }
};

class coopy::store::AttachReport {
public:
  bool success;
  bool applicable;
  std::string msg;

  AttachReport() {
    success = false;
    applicable = false;
  }

  void errorCreateNotImplemented(const char *str) {
    success = false;
    applicable = true;
    msg = std::string("Creation not implemented yet (") + str + ")";
  }
};

class coopy::store::TextBookFactory {
public:
  TextBookFactory();

  virtual ~TextBookFactory() {
  }

  virtual std::string getName() = 0;

  virtual TextBook *open(AttachConfig& config, AttachReport& report) = 0;
};

#endif
