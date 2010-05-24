#include <coopy/ColumnInfo.h>

#include <stdio.h>

std::string ColumnInfo::getName() const {
  char buf[256];
  sprintf(buf,"%d",index);
  return buf;
}

