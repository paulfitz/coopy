#include <coopy/SocialCalcSheet.h>

using namespace coopy::store;
using namespace coopy::store::socialcalc;
using namespace coopy::js;
using namespace std;

extern std::string socialcalc_3();
extern std::string socialcalcconstants();
extern std::string socialcalctableeditor();
extern std::string socialcalcspreadsheetcontrol();
extern std::string socialcalcwrap();

bool SocialCalcSheet::jsSetup(JsWrap& j) {
  string check = j.apply("(typeof socialcalcwrap_loaded!='undefined')?'loaded':'not loaded'");
  if (check!="loaded") {
    dbg_printf("Setup\n");
    j.apply(socialcalc_3());
    j.apply(socialcalcconstants());
    j.apply(socialcalctableeditor());
    j.apply(socialcalcspreadsheetcontrol());
    j.apply(socialcalcwrap());
  }
  return true;
}

bool SocialCalcSheet::select() const {
  if (!jssheet) return false;
  ((JsWrap *)&js)->send("set_active_sheet",jssheet);
  return true;
}

bool SocialCalcSheet::reset() {
  if (jssheet) {
    JS_RemoveValueRoot(js.context(),jssheet);
    delete jssheet;
    jssheet = NULL;
  }
  return true;
}

bool SocialCalcSheet::setup() {
  jsSetup(js);
  reset();
  jssheet = new jsval;
  if (!jssheet) return false;
  jsval lval;
  JSBool ok = JS_CallFunctionName(js.context(), js.global(), 
				  "create_spreadsheet", 
				  0,&lval,jssheet);
  if (!jssheet) return false;
  JS_AddValueRoot(js.context(),jssheet);
  dbg_printf("Made a sheet!\n");
  js.send("set_active_sheet",jssheet);
  dbg_printf("Set a sheet!\n");
  return true;
}


std::string SocialCalcSheet::cellString(int x, int y) const {
  bool escaped;
  return cellString(x,y,escaped);
}

std::string SocialCalcSheet::cellString(int x, int y, bool& escaped) const {
  escaped = true;
  if (!select()) return "NULL";
  jsval args[3] = { INT_TO_JSVAL(x), INT_TO_JSVAL(y) };
  jsval rval;
  JSBool ok = JS_CallFunctionName(js.context(),js.global(),"sheet_get_cell",2,&args[0],&rval);
  if (JSVAL_IS_NULL(rval)) {
    return "NULL";
  }
  JSString *str = JS_ValueToString(js.context(),rval);
  escaped = false;
  return JS_EncodeString(js.context(), str);
}

bool SocialCalcSheet::cellString(int x, int y, const std::string& str, bool escaped) {
  if (!select()) return false;
  jsval args[3] = { INT_TO_JSVAL(x), INT_TO_JSVAL(y), STRING_TO_JSVAL(JS_NewStringCopyN(js.context(),str.c_str(),str.length())) };
  jsval rval;
  JSBool ok = JS_CallFunctionName(js.context(),js.global(),"sheet_set_cell",3,&args[0],&rval);
  if (x>=w) w = x+1;
  if (y>=h) h = y+1;
  return true;
}

ColumnRef SocialCalcSheet::moveColumn(const ColumnRef& src, const ColumnRef& base) {
  return ColumnRef();
}

bool SocialCalcSheet::deleteColumn(const ColumnRef& column) {
  return false;
}

ColumnRef SocialCalcSheet::insertColumn(const ColumnRef& base) {
  return ColumnRef();
}

ColumnRef SocialCalcSheet::insertColumn(const ColumnRef& base, const ColumnInfo& kind) {
  return ColumnRef();
}

bool SocialCalcSheet::modifyColumn(const ColumnRef& base, const ColumnInfo& kind) {
  return false;
}

RowRef SocialCalcSheet::insertRow(const RowRef& base) {
  return RowRef();
}

bool SocialCalcSheet::deleteRow(const RowRef& src) {
  return false;
}

bool SocialCalcSheet::deleteRows(const RowRef& first, const RowRef& last) {
}

RowRef SocialCalcSheet::moveRow(const RowRef& src, const RowRef& base) {
  return RowRef();
}

bool SocialCalcSheet::fromCsvString(const std::string& str) {
  if (!setup()) return false;
  js.send("load_from_csv",str);
  updateSize();
  return true;
}

bool SocialCalcSheet::fromSocialCalcString(const std::string& str) {
  if (!setup()) return false;
  js.send("set_src",str);
  dbg_printf("Set src!\n");
  updateSize();
  return true;
}

std::string SocialCalcSheet::toSocialCalcString(const Property& config) {
  if (!select()) return "";
  string result = js.apply("get_full()");
  return result;
  //FileIO io;
  //return io.openAndWrite(result,config);
}

bool SocialCalcSheet::updateSize() {
  jsval jw, jh;
  jsval lval;
  JSBool ok;
  ok = JS_CallFunctionName(js.context(), js.global(), 
			   "sheet_get_width", 
			   0,&lval,&jw);
  ok = ok && JS_CallFunctionName(js.context(), js.global(), 
				 "sheet_get_height", 
				 0,&lval,&jh);
  if (!ok) return false;
  w = JSVAL_TO_INT(jw);
  h = JSVAL_TO_INT(jh);
  //printf("SIZE %dx%d\n", w, h);
  return true;
}
