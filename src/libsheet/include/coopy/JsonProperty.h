#ifndef COOPY_JSONPROPERTY
#define COOPY_JSONPROPERTY

#include <coopy/Property.h>

#include <json/json.h>

namespace coopy {
  namespace store {
    class JsonProperty;
  }
}

class coopy::store::JsonProperty {
public:
  static bool add(Property& prop, const char *fname);
  static bool add(Property& prop, const std::string& fname) {
    return add(prop,fname.c_str());
  }
  static bool add(Property& prop, Json::Value& root);
};

#endif


