#ifndef COOPY_SHORTTEXTBOOKFACTORY
#define COOPY_SHORTTEXTBOOKFACTORY

#include <coopy/ShortTextBook.h>
#include <coopy/TextBookFactory.h>
#include <coopy/CsvFile.h>
#include <coopy/Dbg.h>

namespace coopy {
  namespace store {
    class ShortTextBookFactory;
  }
}

class coopy::store::ShortTextBookFactory : public TextBookFactory {
  virtual std::string getName() {
    return "csv";
  }

  virtual TextBook *open(AttachConfig& config, AttachReport& report) {
    if (config.shouldWrite) {
      if (config.prevBook!=NULL) {
	//if (config.prevOptions.get("type").asString()==getName()) {
	//ShortTextBook *book = (ShortTextBook *) config.prevBook;
	dbg_printf("writing csv file %s\n", config.options.get("file").asString().c_str());
	int r = CsvFile::write(config.prevBook->readSheetByIndex(0),
			       config.options);
	if (r==0) {
	  report.success = true;
	  return config.prevBook;
	}
      }
      return NULL;
    }

    ShortTextBook *book = new ShortTextBook;
    if (book==NULL) return NULL;

    if (config.shouldRead) {
      if (!config.options.check("should_attach")) {
	dbg_printf("reading csv file %s\n", config.options.get("file").asString().c_str());
	int r = CsvFile::read(config.fname.c_str(),book->sheet,config.options);
	if (r!=0) {
	  delete book;
	  book = NULL;
	}
      }
    }

    if (book!=NULL) {
      report.success = true;
    }

    return book;
  }
};

#endif


