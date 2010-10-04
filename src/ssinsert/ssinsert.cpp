
extern "C" {
#include "gnumeric_link.h"
}

int main(int argc, char *argv[]) {
  // load an excel file
  // load a csv file
  // paste csv data into excel file
  // save excel file
  gnumeric_init();
  hello();
  gnumeric_convert("hello.csv","hello2.xls");
  gnumeric_fini();
  return 0;
}
