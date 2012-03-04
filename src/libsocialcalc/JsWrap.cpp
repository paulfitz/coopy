#include "jsapi.h"

#include <coopy/JsWrap.h>

using namespace coopy::js;
using namespace std;

static JSClass global_class = {
  "global", JSCLASS_GLOBAL_FLAGS,
  JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
  JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
  JSCLASS_NO_OPTIONAL_MEMBERS
};

void reportError(JSContext *cx, const char *message, JSErrorReport *report) {
  fprintf(stderr, "%s:%u:%s\n",
	  report->filename ? report->filename : "<no filename=\"filename\">",
	  (unsigned int) report->lineno,
	  message);
}


namespace coopy {
  namespace js {
    class JsWrapGlobal;
  }
}

class coopy::js::JsWrapGlobal {
public:
  JSRuntime *rt;
  JSContext *cx;
  JSObject  *global;


  bool init();
  bool fini();
  
public:
  JsWrapGlobal() {
    rt = 0/*NULL*/;
    cx = 0/*NULL*/;
    global = 0/*NULL*/;
    init();
  }

  virtual ~JsWrapGlobal() {
    fini();
  }

  std::string apply(const std::string& string);

  bool send(const std::string& function_name,
	    const std::string& str);

  bool send(const std::string& function_name,
	    jsval *val);
};



bool JsWrapGlobal::init() {
  if (rt) return false;

  rt = JS_NewRuntime(8 * 1024 * 1024);
  if (!rt) return false;

  cx = JS_NewContext(rt, 8192);
  if (!cx) {
    JS_DestroyRuntime(rt);
    rt = NULL;
    return false;
  }

  JS_SetOptions(cx, JSOPTION_VAROBJFIX | JSOPTION_JIT | JSOPTION_METHODJIT);
  JS_SetVersion(cx, JSVERSION_LATEST);
  JS_SetErrorReporter(cx, reportError);

  global = JS_NewCompartmentAndGlobalObject(cx, &global_class, NULL);
  if (!global) {
    JS_DestroyContext(cx);
    cx = NULL;
    JS_DestroyRuntime(rt);
    rt = NULL;
    return false;
  }

  if (!JS_InitStandardClasses(cx, global)) {
    fini();
    return false;
  }

  return true;
}

bool JsWrapGlobal::fini() {
  if (!rt) return false;
  JS_DestroyContext(cx);
  cx = NULL;
  JS_DestroyRuntime(rt);
  rt = NULL;
  JS_ShutDown();
  global = NULL;
  return true;
}


string JsWrapGlobal::apply(const string& script) {
  jsval rval;
  JSString *str;
  JSBool ok;
  const char *filename = "noname";
  uintN lineno = 0;

  ok = JS_EvaluateScript(cx, global, script.c_str(), script.length(),
			 filename, lineno, &rval);

  if (!ok) return "";
  
  str = JS_ValueToString(cx, rval);
  return JS_EncodeString(cx, str);
}

bool JsWrapGlobal::send(const std::string& function_name, 
			const std::string& str) {
  JSBool ok;
  jsval rval;
  jsval lval;

  lval = STRING_TO_JSVAL(JS_NewStringCopyZ(cx,str.c_str()));
  ok = JS_CallFunctionName(cx, global, function_name.c_str(), 
			   1,&lval,&rval);
  return ok;
}

bool JsWrapGlobal::send(const std::string& function_name,
			jsval *val) {
  JSBool ok;
  jsval rval;

  ok = JS_CallFunctionName(cx, global, function_name.c_str(), 
			   1,val,&rval);
  return ok;
}


static int js_instance_count = 0;
static JsWrapGlobal *js_global = NULL;

JsWrap::JsWrap() {
  if (!js_global) js_global = new JsWrapGlobal();
  js_instance_count++;
}

JsWrap::~JsWrap() {
  js_instance_count--;
  if (js_instance_count==0&&js_global) {
    delete js_global;
    js_global = NULL;
  }
}

std::string JsWrap::apply(const std::string& str) {
  if (!js_global) return "";
  return js_global->apply(str);
}

bool JsWrap::send(const std::string& function_name, const std::string& str) {
  if (!js_global) return false;
  return js_global->send(function_name,str);
}

bool JsWrap::send(const std::string& function_name, jsval *val) {
  if (!js_global) return false;
  return js_global->send(function_name,val);
}

JSContext *JsWrap::context() const { 
  if (!js_global) return NULL;
  return js_global->cx;
}

JSObject *JsWrap::global() const {
  if (!js_global) return NULL;
  return js_global->global;
}

