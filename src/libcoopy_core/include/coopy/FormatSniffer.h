#ifndef COOPY_FORMATSNIFFER_INC
#define COOPY_FORMATSNIFFER_INC

#include <stdio.h>
#include <string>

#include <coopy/Reader.h>
#include <coopy/Format.h>
#include <coopy/FileIO.h>

namespace coopy {
  namespace format {
    class FormatSniffer;
  }
}


class coopy::format::FormatSniffer : public Reader {
private:
  coopy::store::FileIO fio;
  std::string cache;
public:
  FormatSniffer() {
  }

  virtual ~FormatSniffer() {
    close();
  }

  bool setString(const char *str);

  bool open(const char *fname, bool caching = true);
  bool close();

  bool wrap(coopy::store::FileIO& fin, bool caching);

  Format getFormat();

  virtual std::string read() { 
    std::string tmp = cache; 
    cache = "";
    return tmp;
  }
};

#endif

