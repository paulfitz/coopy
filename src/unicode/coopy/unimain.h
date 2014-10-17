#ifdef NEED_UNIMAIN

#include <windows.h>

// This document is released into the public domain. Absolutely no warranty is provided.
// See http://www.coderforlife.com/projects/utilities.
//
// This is for the MinGW compiler which does not support wmain.
// It is a wrapper for _tmain when _UNICODE is defined (wmain).
//
// !! Do not compile this file, but instead include it right before your _tmain function like:
// #include "mingw-unicode.c"
// int _tmain(int argc, _TCHAR *argv[]) {
//
// If you wish to have enpv in your main, then define the following before including this file:
// #define MAIN_USE_ENVP
//
// This wrapper adds ~300 bytes to the program and negligible overhead

#undef _tmain
#ifdef _UNICODE
#define _tmain wmain
#else
#define _tmain main
#endif

//#if defined(__GNUC__) && defined(_UNICODE)

#ifndef __MSVCRT__
#error Unicode main function requires linking to MSVCRT
#endif

#include <wchar.h>
#include <stdlib.h>

extern int _CRT_glob;
extern 
#ifdef __cplusplus
"C" 
#endif
void __wgetmainargs(int*,wchar_t***,wchar_t***,int,int*);

int __unimain(int argc, char *argv[]);

#ifdef MAIN_USE_ENVP
int wmain(int argc, wchar_t *argv[], wchar_t *envp[]);
#else
int wmain(int argc, wchar_t *argv[]);
#endif

int main() {
  wchar_t **enpv, **argv;
  int argc, si = 0;
  __wgetmainargs(&argc, &argv, &enpv, _CRT_glob, &si); // this also creates the global variable __wargv
#ifdef MAIN_USE_ENVP
  return wmain(argc, argv, enpv);
#else
  return wmain(argc, argv);
#endif
}

int wmain(int argc, wchar_t *argv[]) {
printf("Converting...\n");
 char **argvConverted = new char*[argc + 1];
 if (!argvConverted) return 1;
 
 for (int i=0; i<argc; i++) {
   int len = wcslen(argv[i]);
   int tlen = WideCharToMultiByte(CP_UTF8, 0, argv[i], len, NULL, 0, NULL, NULL);
   char *str = new char[tlen+1];
   WideCharToMultiByte(CP_UTF8, 0, argv[i], len, str, tlen, NULL, NULL);
   argvConverted[i] = str;
   if (!argvConverted[i]) return 1;
 }
 argvConverted[argc] = NULL;
 int result = __unimain(argc, argvConverted);
 for (int i=0; i<argc; i++) {
   delete argvConverted[i];
 }
 delete[] argvConverted;
 return result;
}

#define main __unimain

#endif
