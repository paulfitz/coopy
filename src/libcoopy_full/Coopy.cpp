#include <coopy/Coopy.h>
#include <coopy/CsvSheet.h>
#include <coopy/PatchParser.h>
#include <coopy/BookCompare.h>

using namespace coopy::store;
using namespace coopy::cmp;
using namespace coopy::app;


bool Coopy::compare(coopy::store::TextBook& pivot, 
		    coopy::store::TextBook& local, 
		    coopy::store::TextBook& remote) {
  if (!checkFormat()) return false;
  coopy::cmp::BookCompare cmp;
  if (output_book) {
    patcher->attachOutputBook(*output_book);
  }
  if (!patcher->startOutput(output,flags)) {
    return false;
  }
  if (patcher->outputStartsFromInput()) {
    patcher->attachBook(local);
    //if (!output_book) {
    //patcher->attachOutputBook(local);
    //}
  }
  int r = cmp.compare(pivot,local,remote,*patcher,flags);
  patcher->stopOutput(output,flags);
  return (r==0);
}

bool Coopy::merge(coopy::store::TextBook& pivot, 
		  coopy::store::TextBook& local, 
		  coopy::store::TextBook& remote) {
  setFormat("merge");
  if (!checkFormat()) return false;
  patcher->attachBook(local);
  coopy::cmp::BookCompare cmp;
  if (!patcher->startOutput(output,flags)) {
    return false;
  }
  int r = cmp.compare(pivot,local,remote,*patcher,flags);
  patcher->stopOutput(output,flags);
  return (r==0)&&!patcher->isConflicted();
}


bool Coopy::patch(coopy::store::TextBook& local, 
		  coopy::store::TextBook& mod) {
  setFormat("apply");
  if (!checkFormat()) return false;
  patcher->attachBook(local);
  coopy::cmp::BookCompare cmp;
  if (!patcher->startOutput(output,flags)) {
    return false;
  }
  PatchParser parser(patcher,flags);
  bool ok = parser.applyHiliteBook(mod);
  patcher->stopOutput(output,flags);
  return ok;
}

PolySheet Coopy::createSheet() {
  CsvSheet *local = new CsvSheet();
  if (!local) return PolySheet();
  return PolySheet(local,true);
}


coopy::store::PolyBook Coopy::createBook(coopy::store::PolySheet sheet) {
  WrapBook *book = new WrapBook(sheet);
  PolyBook pb;
  pb.take(book);
  return pb;
}

coopy::store::PolyBook Coopy::loadBook(const char *filename) {
  PolyBook pb;
  pb.read(filename);
  return pb;
}


