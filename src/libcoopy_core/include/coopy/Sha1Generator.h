#ifndef COOPY_Sha1Generator
#define COOPY_Sha1Generator

#include <string>

extern void sha1sum_step_text(const char *zText, int nBytes);
extern char *sha1sum_finish();

namespace coopy {
  namespace store {
    class Sha1Generator;
  }
}

class coopy::store::Sha1Generator {
public:
  void add(const std::string& str) {
    add(str.c_str(),str.length());
  }

  void add(const char *txt, int len) {
    sha1sum_step_text(txt,len);
  }

  std::string finish() {
    return sha1sum_finish();
  }
};

#endif

