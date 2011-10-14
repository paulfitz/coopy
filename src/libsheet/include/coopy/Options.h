#ifndef COOPY_OPTIONS_H
#define COOPY_OPTIONS_H

#include <string>
#include <vector>
#include <map>

#include <coopy/CompareFlags.h>
#include <coopy/PolyBook.h>

namespace coopy {
  namespace app {
    class Options;
  }
}

/**
 *
 * Set up options.  This object must exist at the widest possible scope.
 *
 */
class coopy::app::Options {
public:
  int apply(int argc, char *argv[]);

  const std::vector<std::string>& getCore() const { return core; }
  const coopy::cmp::CompareFlags& getCompareFlags() const { return flags; }

  bool checkBool(const char *name, bool fallback=false) const { 
    if (option_bool.find(name)==option_bool.end()) return fallback;
    return option_bool.find(name)->second; 
  }

  std::string checkString(const char *name, const char *fallback="") const { 
    if (option_string.find(name)==option_string.end()) return fallback;
    return option_string.find(name)->second; 
  }

  bool isVerbose() const { return checkBool("verbose"); }

  bool isHelp() const { return checkBool("help"); }

  Options(const char *name) : name(name) {
  }

  bool isDiffLike() const {
    return (name=="ssdiff"||name=="sspatch");
  }

  bool isPatchLike() const {
    return (name=="sspatch");
  }

  bool isMergeLike() const {
    return (name=="ssmerge");
  }
private:
  std::string name;
  std::vector<std::string> core;
  coopy::cmp::CompareFlags flags;
  std::map<std::string,bool> option_bool;
  std::map<std::string,std::string> option_string;
  coopy::store::PolyBook mapping;
};

#endif
