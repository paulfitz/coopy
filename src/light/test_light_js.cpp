#include <coopy/Coopy.h>
#include <coopy/FileIO.h>
#include <coopy/JsWrap.h>
#include <coopy/JsonBook.h>

#include <stdio.h>

extern std::string coopy_js();
extern std::string coopy_test_js();

using namespace coopy::app;
using namespace coopy::store;
using namespace coopy::store::json;
using namespace coopy::js;
using namespace std;

int main(int argc, char *argv[]) {
  if (argc!=4) return 1;
  const char *in_name = argv[1];
  const char *format = argv[2];
  const char *out_name = argv[3];
  Coopy coopy;
  PolyBook book = coopy.loadBook(in_name);
  if (!book.isValid()) return 2;
  if (book.getSheetCount()!=1) return 3;
  PolySheet sheet = book.readSheetByIndex(0);

  JsWrap wrap;
  wrap.apply(coopy_js());
  wrap.apply(coopy_test_js());
  wrap.send("set_format",format);
  Property opts;
  opts.put("hash",0);
  wrap.send("set_input",JsonBook::render(&book,opts));

  FileIO fout;
  if (!fout.openForWrite(out_name,Property())) {
    return 4;
  }
  fprintf(fout.get(),"%s",wrap.apply("test_reader()").c_str());

  return 0;
}
