#include <json/json.h>
#include <iostream>

int main() {

  Json::Value root;   // will contains the root value after parsing.
  Json::Reader reader;

  root["greeting"] = "hello";
  root["number"] = 10;

  Json::StyledWriter writer;
  // Make a new JSON document for the configuration. Preserve original comments.
  std::string outputConfig = writer.write( root );

  // And you can write to a stream, using the StyledWriter automatically.
  std::cout << root;
  return 0;
}
