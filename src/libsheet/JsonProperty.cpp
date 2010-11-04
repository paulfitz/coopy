#include <coopy/JsonProperty.h>
#include <coopy/Dbg.h>

#include <fstream>

using namespace std;
using namespace coopy::store;

bool JsonProperty::add(Property& prop, const char *fname) {
  ifstream in(fname);
  Json::Value root;
  Json::Reader reader;
  if (!reader.parse(in,root,false)) {
    fprintf(stderr,"Failed to parse %s\n", fname);
    return false;
  }
  return add(prop,root);
}

bool JsonProperty::add(Property& prop, Json::Value& root) {
   for (Json::Value::iterator it=root.begin(); it!=root.end(); it++) {
     if ((*it).isString()) {
       dbg_printf("Got %s -> %s\n", it.memberName(),
		  (*it).asCString());
       prop.put(it.memberName(),(*it).asCString());
     } else if ((*it).isInt()) {
       dbg_printf("Got %s -> %d\n", it.memberName(),
		  (*it).asInt());
       prop.put(it.memberName(),(*it).asInt());
     } else {
       fprintf(stderr, "JSON: unsupported type\n");
       return false;
     }
   }
   return true;
}


