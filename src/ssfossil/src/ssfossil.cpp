#include "ssfossil.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include <string>
using namespace std;

static int __ssfossil_debug = 0;
static FossilHandler *_ssfossil_handler = NULL;

extern "C" int fossil_main(int argc, char **argv);
extern "C" int _ssfossil_exit(int result);
extern "C" int _ssfossil_printf(const char *format, ...);
extern "C" int _ssfossil_fprintf(FILE *target, const char *format, ...);
extern "C" int db_close();

int ssfossil_debug() {
  return __ssfossil_debug;
}

int ssfossil_call(int argc, char *argv[]) {
  if (getenv("SSFOSSIL_DEBUG")!=NULL) {
    if (!__ssfossil_debug) {
      __ssfossil_debug = 1;
      printf("*** Modified fossil, do not entrust it with your data ***\n");
    }
  }
  int result = fossil_main(argc,argv);
  db_close();
  return result;
}

void ssfossil_set_handler(FossilHandler *handler) {
  _ssfossil_handler = handler;
}

int _ssfossil_exit(int result) {
  db_close();
  if (_ssfossil_handler) {
    return _ssfossil_handler->exit(result);
    // must not actually return!
  }
  exit(result);
  return 1;
}


int _ssfossil_printf(const char *format, ...) {
  va_list listPointer;

  int result;
  if (_ssfossil_handler) {
    string str;
    int sz = 4192;

    int n = 0;
    bool more = false;
    do {
      str.reserve(sz);
      va_start(listPointer,format);
      n = vsnprintf((char*)str.c_str(),sz,format,listPointer);
      va_end(listPointer);
      more = false;
      if (n<=-1) {
	sz *= 2;
	more = true;
      } else if (n>=sz) {
	sz = n+1;
	more = true;
      }
    } while (more);
    result = _ssfossil_handler->printf(str.c_str());
  } else {
    va_start(listPointer,format);
    result = vprintf(format,listPointer);
    va_end(listPointer);
  }

  fflush(stdout);
  return result;
}



int _ssfossil_fprintf(FILE *target, const char *format, ...) {
  // exactly the same as _ssfossil_printf, we ignore target

  va_list listPointer;

  int result;
  if (_ssfossil_handler) {
    string str;
    int sz = 4192;

    int n = 0;
    bool more = false;
    do {
      str.reserve(sz);
      va_start(listPointer,format);
      n = vsnprintf((char*)str.c_str(),sz,format,listPointer);
      va_end(listPointer);
      more = false;
      if (n<=-1) {
	sz *= 2;
	more = true;
      } else if (n>=sz) {
	sz = n+1;
	more = true;
      }
    } while (more);
    result = _ssfossil_handler->printf(str.c_str());
  } else {
    va_start(listPointer,format);
    result = vprintf(format,listPointer);
    va_end(listPointer);
  }

  fflush(stdout);
  return result;
}


