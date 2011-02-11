#include <coopy/SheetCell.h>

#include <stdio.h>

using namespace coopy::store;

SheetCell::SheetCell(int x) {
  char buf[256];
  sprintf(buf,"%d",x);
  text = buf;
  escaped = false;
}


