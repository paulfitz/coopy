#include <coopy/JsWrap.h>
#include <stdio.h>

using namespace std;
using namespace coopy::js;

extern std::string socialcalc_3();
extern std::string socialcalcconstants();
extern std::string socialcalctableeditor();
extern std::string socialcalcspreadsheetcontrol();
extern std::string socialcalcwrap();

int main(int argc, char *argv[]) {
  JsWrap js;
  //js.apply("function alert(x) { }\n");
  js.apply(socialcalc_3());
  js.apply(socialcalcconstants());
  js.apply(socialcalctableeditor());
  js.apply(socialcalcspreadsheetcontrol());

  // for occasional testing
  js.apply(socialcalcwrap());
  js.send("setSrc","helylo");

  string result = js.apply("get_full()");
  printf("[%s]\n", result.c_str());
  return 0;
}
