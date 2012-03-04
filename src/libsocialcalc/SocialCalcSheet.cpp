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
  if (!jssheet) {
    ((SocialCalcSheet *)this)->setup();
  }
  if (!jssheet) return false;
  if (!js.isCurrentId(id)) {
    ((JsWrap *)&js)->send("set_active_sheet",jssheet);
    js.setCurrentId(id);
  }
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
  id = js.getId();
  dbg_printf("Made a sheet! %d\n", id);
  js.send("set_active_sheet",jssheet);
  dbg_printf("Set a sheet!\n");
  js.setCurrentId(id);
  return true;
}


std::string SocialCalcSheet::cellString(int x, int y) const {
  bool escaped;
  return cellString(x,y,escaped);
}

std::string SocialCalcSheet::cellString(int x, int y, bool& escaped) const {
  escaped = true;
  if (!select()) return "";
  jsval args[3] = { INT_TO_JSVAL(x), INT_TO_JSVAL(y) };
  jsval rval;
  JSBool ok = JS_CallFunctionName(js.context(),js.global(),"sheet_get_cell",2,&args[0],&rval);
  if (JSVAL_IS_NULL(rval)) {
    return "";
  }
  JSString *str = JS_ValueToString(js.context(),rval);
  escaped = false;
  return JS_EncodeString(js.context(), str);
}

bool SocialCalcSheet::cellString(int x, int y, const std::string& str, bool escaped) {
  if (!select()) return false;
  jsval args[3] = { INT_TO_JSVAL(x), INT_TO_JSVAL(y), JSVAL_NULL };
  if (!escaped) {
    args[2] = STRING_TO_JSVAL(JS_NewStringCopyN(js.context(),str.c_str(),
						str.length()));
  }
  jsval rval;
  JSBool ok = JS_CallFunctionName(js.context(),js.global(),"sheet_set_cell",3,&args[0],&rval);
  if (x>=w) w = x+1;
  if (y>=h) h = y+1;
  return true;
}

ColumnRef SocialCalcSheet::moveColumn(const ColumnRef& src, const ColumnRef& base) {
  if (!select()) return ColumnRef();
  jsval args[4] = { 
    INT_TO_JSVAL(src.getIndex()), 
    INT_TO_JSVAL(base.getIndex()),
    INT_TO_JSVAL(w),
    INT_TO_JSVAL(h)
  };
  jsval rval;
  JSBool ok = JS_CallFunctionName(js.context(),js.global(),"sheet_move_column",
				  4,&args[0],&rval);
  int ret = JSVAL_TO_INT(rval);
  return ColumnRef(ret);
}

bool SocialCalcSheet::deleteColumn(const ColumnRef& column) {
  if (!select()) return false;
  jsval args[3] = { 
    INT_TO_JSVAL(column.getIndex()) ,
    INT_TO_JSVAL(w),
    INT_TO_JSVAL(h)
  };
  jsval rval;
  JSBool ok = JS_CallFunctionName(js.context(),js.global(),
				  "sheet_delete_column",3,&args[0],&rval);
  int ret = JSVAL_TO_INT(rval);
  if (ret>=0) w--;
  return ret>=0;
}

ColumnRef SocialCalcSheet::insertColumn(const ColumnRef& base) {
  if (!select()) return ColumnRef();
  jsval args[3] = { 
    INT_TO_JSVAL(base.getIndex()) ,
    INT_TO_JSVAL(w),
    INT_TO_JSVAL(h)
  };
  jsval rval;
  JSBool ok = JS_CallFunctionName(js.context(),js.global(),
				  "sheet_insert_column",3,&args[0],&rval);
  int ret = JSVAL_TO_INT(rval);
  if (ret>=0) w++;
  return ColumnRef(ret);
}

ColumnRef SocialCalcSheet::insertColumn(const ColumnRef& base, const ColumnInfo& kind) {
  return insertColumn(base);
}

bool SocialCalcSheet::modifyColumn(const ColumnRef& base, const ColumnInfo& kind) {
  return false;
}

RowRef SocialCalcSheet::insertRow(const RowRef& base) {
  if (!select()) return RowRef();
  jsval args[3] = { 
    INT_TO_JSVAL(base.getIndex()) ,
    INT_TO_JSVAL(w),
    INT_TO_JSVAL(h)
  };
  jsval rval;
  JSBool ok = JS_CallFunctionName(js.context(),js.global(),
				  "sheet_insert_row",3,&args[0],&rval);
  int ret = JSVAL_TO_INT(rval);
  if (ret>=0) h++;
  return RowRef(ret);
}

bool SocialCalcSheet::deleteRow(const RowRef& src) {
  return deleteRows(src,src);
}

bool SocialCalcSheet::deleteRows(const RowRef& first, const RowRef& last) {
  if (!select()) return false;
  jsval args[4] = { 
    INT_TO_JSVAL(first.getIndex()) ,
    INT_TO_JSVAL(last.getIndex()) ,
    INT_TO_JSVAL(w),
    INT_TO_JSVAL(h)
  };
  jsval rval;
  JSBool ok = JS_CallFunctionName(js.context(),js.global(),
				  "sheet_delete_rows",4,&args[0],&rval);
  int ret = JSVAL_TO_INT(rval);
  if (ret>=0) h -= ret;
  return ret>=0;
}

RowRef SocialCalcSheet::moveRow(const RowRef& src, const RowRef& base) {
  return RowRef();
}

bool SocialCalcSheet::fromCsvString(const std::string& str) {
  if (!setup()) return false;
  dbg_printf("SocialCalc: Loading from csv\n");
  js.send("load_from_csv",str);
  updateSize();
  return true;
}

bool SocialCalcSheet::fromSocialCalcString(const std::string& str) {
  if (!setup()) return false;
  dbg_printf("SocialCalc: Loading from native:\n===\n%s\n===\n", str.c_str());
  js.send("set_src",str);
  dbg_printf("Set src!\n");
  updateSize();
  return true;
}

std::string SocialCalcSheet::toSocialCalcString(const Property& config) {
  if (!select()) return "";
  string result = js.apply("get_full()");

  return result;
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
