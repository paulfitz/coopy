#ifndef COOPY_EFFICIENTMAP
#define COOPY_EFFICIENTMAP

#ifdef __GNUC__
#include <ext/hash_map>

#define efficient_map __gnu_cxx::hash_map

namespace __gnu_cxx {
  template <>
  struct hash<std::string> {
    size_t operator() (const std::string& x) const {
      return hash<const char*>()(x.c_str());
    }
  };
}
#else
#warning "Unfamiliar compiler, compiling without a hash map chosen - fix this"
#include <map>
#define efficient_map std::map
#endif

#endif

