#ifndef COOPY_FILEIO
#define COOPY_FILEIO

#include <stdio.h>

#include <coopy/Property.h>

namespace coopy {
  namespace store {
    class FileIO;
  }
}

class coopy::store::FileIO {
private:
  FILE *fp;
  bool need_close;
  size_t pending_length;
  bool has_length;
public:
  FileIO() {
    fp = NULL;
    need_close = false;
    pending_length = 0;
    has_length = false;
  }

  virtual ~FileIO() {
    close();
  }

  bool open(const char *src, const Property& config);

  bool close();

  size_t fread(void *ptr, size_t size, size_t nmemb);

  bool isPiped() {
    return fp==stdin;
  }

  bool isValid() {
    return fp!=NULL;
  }

  bool openAndWrite(const std::string& txt, const Property& config);
};

#endif


