
#include <coopy/FormatSniffer.h>
#include <coopy/SocialCalcTextBook.h>
#include <coopy/CsvRead.h>
#include <coopy/JsWrap.h>
#include <coopy/FileIO.h>
#include <coopy/SocialCalcSheet.h>
#include "jsapi.h"

using namespace coopy::store;
using namespace coopy::format;
using namespace coopy::store::socialcalc;
using namespace coopy::js;
using namespace std;

class SCCSV : public CsvSheetReader {
public:
  CsvSheet *sheet;
  virtual CsvSheet *nextSheet(const char *name, bool named) {
    return sheet;
  }
};

bool SocialCalcTextBook::open(const Property& config) {
  if (!config.check("file")) return false;
  if (config.check("name")) {
    name = config.get("name").asString();
  }

  FormatSniffer f;
  if (!f.open(config.get("file").asString().c_str(),true)) return false;
  return sheet.fromSocialCalcString(f.read());
}


bool SocialCalcTextBook::write(TextBook *book, const Property& config) {
  if (!book) return false;
  SocialCalcTextBook *prev = dynamic_cast<SocialCalcTextBook *>(book);
  if (prev) {
    FileIO io;
    string txt = prev->sheet.toSocialCalcString(config);
    return io.openAndWrite(txt,config);
  }
  if (book->getNames().size()>1) {
    fprintf(stderr,"* socialcalc plugin cannot currently deal with multiple sheets\n");
    return false;
  }
  SocialCalcSheet sheet;
  if (!sheet.fromCsvString(book->readSheetByIndex(0).toString())) return false;
  FileIO io;
  return io.openAndWrite(sheet.toSocialCalcString(config),config);
}
