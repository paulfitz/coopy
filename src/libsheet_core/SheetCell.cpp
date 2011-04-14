#include <coopy/SheetCell.h>

#include <stdio.h>
#include <stdlib.h>

using namespace coopy::store;

SheetCell::SheetCell(int x) {
  char buf[256];
  sprintf(buf,"%d",x);
  text = buf;
  escaped = false;
}

int SheetCell::asInt() const {
  if (escaped) return 0;
  return atoi(text.c_str());
}



