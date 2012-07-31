#include <stdio.h>
#include <coopy/Diff.h>

#include <emscripten.h>
#include <string>

using namespace coopy::app;
using namespace std;

std::string emcc_result;

extern "C" const char *get_message() {
  return (const char *)emcc_result.c_str();
}

extern "C" void process_message(const char *msg) {
  if (msg[0]=='\0') return;
  emcc_result = std::string("I GOT ") + msg;
  emscripten_run_script("js_get_message = cwrap('get_message', 'string', []);");
  emscripten_run_script("postMessage(js_get_message());");
}

int main(int argc, char *argv[]) {
  process_message(get_message());
  process_message("");
  if (argc==1) {
    emscripten_run_script("if (typeof postMessage !== 'undefined') { postMessage('hello from worker 3!') }");
    emscripten_run_script("js_process_message = cwrap('process_message', 'void', ['string']);");
    emscripten_run_script("onmessage = function(e){ js_process_message(e.data); };");    
    return 0;
  }
  
  Options opt("ssdiff");
  int r = opt.apply(argc,argv);
  if (r!=0) return r;

  Diff diff;
  return diff.apply(opt);
}

