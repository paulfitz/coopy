#ifndef COOPY_COOPY_H
#define COOPY_COOPY_H

#include <coopy/Patcher.h>
#include <coopy/WrapBook.h>
#include <coopy/BookCompare.h>

namespace coopy {
  namespace app {
    class Coopy;
  }
}

class coopy::app::Coopy {
private:
  coopy::cmp::Patcher *patcher;
  std::string output;
public:
  coopy::cmp::CompareFlags flags;

  Coopy() {
    patcher = 0/*NULL*/;
    output = "-";
  }

  virtual ~Coopy() {
    reset();
  }

  void reset() {
    if (patcher!=0/*NULL*/) delete patcher;
  }

  bool setFormat(const char *name, const char *version = NULL) {
    reset();
    patcher = coopy::cmp::Patcher::createByName(name,version);
    return patcher!=0/*NULL*/;
  }

  bool checkFormat() {
    if (patcher) return true;
    return setFormat("tdiff");
  }

  bool setOutput(const char *filename) {
    output = filename;
    return true;
  }

  bool compare(coopy::store::TextBook& pivot, 
	       coopy::store::TextBook& local, 
	       coopy::store::TextBook& remote) {
    if (!checkFormat()) return false;
    coopy::cmp::BookCompare cmp;
    if (!patcher->startOutput(output,flags)) {
      return false;
    }
    int r = cmp.compare(pivot,local,remote,*patcher,flags);
    patcher->stopOutput(output,flags);
    return (r==0);
  }

  bool compare(coopy::store::TextBook& local, 
	       coopy::store::TextBook& remote) {
    return compare(local,local,remote);
  }

  bool compare(coopy::store::DataSheet& pivot, 
	       coopy::store::DataSheet& local, 
	       coopy::store::DataSheet& remote) {
    coopy::store::WrapBook book1(pivot,false);
    coopy::store::WrapBook book2(local,false);
    coopy::store::WrapBook book3(remote,false);
    return compare(book1,book2,book3);
  }

  bool compare(coopy::store::DataSheet& local, 
	       coopy::store::DataSheet& remote) {
    return compare(local,local,remote);
  }
};


#endif
