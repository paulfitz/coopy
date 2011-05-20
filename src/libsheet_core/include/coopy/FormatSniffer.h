#ifndef COOPY_FORMATSNIFFER_INC
#define COOPY_FORMATSNIFFER_INC

#include <string>

#include <coopy/Reader.h>
#include <coopy/Format.h>

namespace coopy {
  namespace format {
    class FormatSniffer;
  }
}


class coopy::format::FormatSniffer : public Reader {
private:
  void *impl;
  bool need_close;
  std::string cache;
public:
  FormatSniffer() {
    impl = 0 /*NULL*/;
    need_close = false;
  }

  virtual ~FormatSniffer() {
    close();
  }

  bool setString(const char *str);

  bool open(const char *fname, bool caching = true);
  bool close();

  Format getFormat();

  virtual std::string read() { 
    std::string tmp = cache; 
    cache = "";
    return tmp;
  }
};

#endif

