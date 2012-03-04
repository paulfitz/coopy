#include <coopy/JsWrap.h>
#include <stdio.h>

#include <coopy/SocialCalcSheet.h>
#include <coopy/SocialCalcTextBook.h>

using namespace std;
using namespace coopy::js;
using namespace coopy::store;
using namespace coopy::store::socialcalc;

extern std::string socialcalc_3();
extern std::string socialcalcconstants();
extern std::string socialcalctableeditor();
extern std::string socialcalcspreadsheetcontrol();
extern std::string socialcalcwrap();

int main(int argc, char *argv[]) {
  JsWrap js;
  js.apply(socialcalc_3());
  js.apply(socialcalcconstants());
  js.apply(socialcalctableeditor());
  js.apply(socialcalcspreadsheetcontrol());

  js.apply(socialcalcwrap());

  js.apply("create_spreadsheet()");
  string result = js.apply("test_sheet()");
  printf("[%s]\n", result.c_str());


  SocialCalcTextBook book;
  PolySheet sheet = book.readSheetByIndex(0);
  if (!sheet.isValid()) {
    fprintf(stderr,"Cannot get sheet\n");
  }
  sheet.cellString(0,0,"foo");
  sheet.cellString(1,0,"bar");
  //sheet.moveColumn(ColumnRef(0),ColumnRef(1));
  sheet.moveColumn(ColumnRef(1),ColumnRef(0));
  sheet.insertColumn(ColumnRef(0));
  sheet.cellString(1,0,"zig",true); 
  sheet.insertColumn(ColumnRef(3));
  printf(">>>\n%s\n", sheet.toString().c_str());
  sheet.insertRow(RowRef(-1));
  printf(">>>\n%s\n", sheet.toString().c_str());
  sheet.cellString(1,0,"zig",true); 
  printf(">>>\n%s\n", sheet.toString().c_str());
  sheet.insertRow(RowRef(-1));
  printf(">>>\n%s\n", sheet.toString().c_str());
  sheet.insertRow(RowRef(0));
  printf(">>>\n%s\n", sheet.toString().c_str());
  Property p;
  p.put("file","hi.sc");
  SocialCalcTextBook::write(&book,p);

  
  printf("done\n");

  return 0;
}
