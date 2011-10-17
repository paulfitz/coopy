#ifndef COOPY_OPTIONS_H
#define COOPY_OPTIONS_H

#include <string>
#include <vector>
#include <map>

#include <coopy/CompareFlags.h>
#include <coopy/PolyBook.h>

namespace coopy {
  namespace app {
    class Option;
    class Options;
  }
}

enum {
  OPTION_FOR_DIFF = 1,
  OPTION_FOR_PATCH = 2,
  OPTION_FOR_MERGE = 4,
  OPTION_FOR_FORMAT = 8,
  OPTION_FOR_REDIFF = 16,
  OPTION_FOR_COMPARE = OPTION_FOR_DIFF|OPTION_FOR_MERGE,
  OPTION_FOR_ALL = 31,
  OPTION_PATCH_FORMAT = 32,
};

class coopy::app::Option {
public:
  std::string long_name;
  std::string arg;
  std::string desc;
  int coverage;
  bool is_default;

  Option() {
    coverage = 0;
    is_default = false;
  }
};


/**
 *
 * Set up options.  This object must exist at the widest possible scope.
 *
 */
class coopy::app::Options {
public:
  Options();

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

  Options(const char *name);

  bool isDiffLike() const {
    return (name=="ssdiff"||name=="ssrediff"||name=="sspatch");
  }

  bool isPatchLike() const {
    return (name=="sspatch")||(name=="ssrediff");
  }

  bool isRediffLike() const {
    return (name=="ssrediff");
  }

  bool isMergeLike() const {
    return (name=="ssmerge");
  }

  std::string getVersion() const;

  void add(int coverage, const char *name, const char *desc);

  void addAll(const char *name, const char *desc) {
    add(OPTION_FOR_ALL,name,desc);
  }

  void addTransform(const char *name, const char *desc) {
    add(OPTION_FOR_DIFF|OPTION_FOR_MERGE|OPTION_FOR_PATCH|OPTION_FOR_REDIFF,name,desc);
  }

  void addCompare(const char *name, const char *desc) {
    add(OPTION_FOR_COMPARE,name,desc);
  }

  void showOptions(int filter);


  void beginHelp();
  void addUsage(const char *usage);
  void addDescription(const char *desc);
  void endHelp();

private:
  std::string name;
  std::vector<std::string> core;
  coopy::cmp::CompareFlags flags;
  std::map<std::string,bool> option_bool;
  std::map<std::string,std::string> option_string;
  coopy::store::PolyBook mapping;
  std::vector<Option> opts;
};

#endif
