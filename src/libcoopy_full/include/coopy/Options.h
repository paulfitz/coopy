#ifndef COOPY_OPTIONS_H
#define COOPY_OPTIONS_H

#include <string>
#include <vector>
#include <map>

#include <coopy/CompareFlags.h>
#include <coopy/PolyBook.h>

namespace coopy {
  /**
   * Helper classes for the standard Coopy command-line tools.
   */
  namespace app {
    class Option;
    class Example;
    typedef Example Recipe;
    class Options;
  }
}

enum {
  OPTION_FOR_DIFF = 1,
  OPTION_FOR_PATCH = 2,
  OPTION_FOR_MERGE = 4,
  OPTION_FOR_FORMAT = 8,
  OPTION_FOR_REDIFF = 16,
  OPTION_FOR_RESOLVE = 32,
  OPTION_FOR_COOPY = 64,
  OPTION_FOR_COMPARE = OPTION_FOR_DIFF|OPTION_FOR_MERGE,
  OPTION_FOR_ALL = 127,
  OPTION_PATCH_FORMAT = 128,
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

class coopy::app::Example {
public:
  std::string code;
  std::string desc;
  std::vector<std::string> reqs;

  Example& require(std::string req) {
    reqs.push_back(req);
    return *this;
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

  void setBool(const char *name, bool val) {
    option_bool[name] = val;
  }

  void addStringToList(const char *name, const char *val) {
    if (option_list.find(name)==option_list.end()) {
      option_list[name] = std::vector<std::string>();
    }
    option_list[name].push_back(val);
  }

  std::string checkString(const char *name, const char *fallback="") const { 
    if (option_string.find(name)==option_string.end()) return fallback;
    return option_string.find(name)->second; 
  }

  bool isStringList(const char *name) const {
    return option_list.find(name)!=option_list.end();
  }

  const std::vector<std::string>& getStringList(const char *name) const {
    return option_list.find(name)->second;
  }

  bool isVerbose() const { return checkBool("verbose"); }

  bool isHelp() const { return checkBool("help"); }

  Options(const char *name);

  bool isDiffLike() const {
    return (name=="ssdiff"||name=="ssrediff"||name=="sspatch"||name=="ssresolve");
  }

  bool isPatchLike() const {
    return (name=="sspatch")||(name=="ssrediff");
  }

  bool isRediffLike() const {
    return (name=="ssrediff");
  }

  bool isResolveLike() const {
    return (name=="ssresolve");
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

  void showOptions(int filter) {
    option_filter = filter;
  }


  void beginHelp();
  void addUsage(const char *usage);
  void addDescription(const char *desc);
  Example& addExample(const char *code, const char *desc) {
    Example eg;
    eg.code = code;
    eg.desc = desc;
    examples.push_back(eg);
    return examples.back();
  }
  Recipe& addRecipe(const char *name, const char *code) {
    Recipe eg;
    eg.desc = name;
    eg.code = code;
    recipes[name] = eg;
    return recipes[name];
  }

  void endHelp();

  const std::vector<Option>& getOptionList() const { return opts; }
  const std::string& getName() const { return name; }
  const std::vector<std::string>& getUsages() const { return usages; }
  const std::vector<Example>& getExamples() const { return examples; }
  const std::vector<std::string> getExampleReqs() const;
  const std::vector<std::string> getExampleRecipes(const std::vector<std::string>& reqs) const;
  const std::string& getDescription() const { return description; }
  int getOptionFilter() const { return option_filter; }

private:
  std::string name;
  std::vector<std::string> core;
  coopy::cmp::CompareFlags flags;
  std::map<std::string,bool> option_bool;
  std::map<std::string,std::string> option_string;
  std::map<std::string,std::vector<std::string> > option_list;
  coopy::store::PolyBook mapping;
  std::vector<Option> opts;
  std::vector<Example> examples;
  std::map<std::string,Recipe> recipes;
  std::vector<std::string> usages;
  std::string description;
  int option_filter;
};

#endif
