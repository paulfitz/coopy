#ifndef COOPY_OS_INC
#define COOPY_OS_INC

namespace coopy {
  namespace os {
    class OS;
  }
}

#include <string>

class coopy::os::OS {
public:
  static std::string getTemporaryFilename();
  static bool deleteFile(const std::string& name);
};

#endif

