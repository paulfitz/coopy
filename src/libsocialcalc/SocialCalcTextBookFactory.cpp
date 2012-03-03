#include <coopy/SocialCalcTextBook.h>
#include <coopy/SocialCalcTextBookFactory.h>

using namespace coopy::store;
using namespace coopy::store::socialcalc;

TextBook *SocialCalcTextBookFactory::open(AttachConfig& config, 
					  AttachReport& report) {
  if (config.shouldWrite) {
    if (config.prevBook!=NULL) {
      dbg_printf("writing socialcalc file %s\n", config.options.get("file").asString().c_str());
      bool ok = SocialCalcTextBook::write(config.prevBook,config.options);
      if (ok) {
	report.success = true;
	return config.prevBook;
      }
    }
    return NULL;
  }
  
  SocialCalcTextBook *book = new SocialCalcTextBook;
  if (book==NULL) return NULL;
  
  if (config.shouldRead) {
    if (!config.options.check("should_attach")) {
      dbg_printf("reading socialcalc file %s\n", config.options.get("file").asString().c_str());
      bool ok = book->open(config.options);
      if (!ok) {
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
