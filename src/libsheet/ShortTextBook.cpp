
#include <coopy/ShortTextBook.h>

#include <coopy/CsvFile.h>

using namespace coopy::store;
using namespace std;

bool ShortTextBook::open(const Property& config) {
  if (!config.check("file")) return false;
  return CsvFile::read(config.get("file").asString().c_str(),sheet,config)==0;
}
