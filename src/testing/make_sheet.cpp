#include <coopy/CsvFile.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc<3) return 1;
  IntSheet s;
  int h = atoi(argv[1]);
  int w = atoi(argv[2]);
  s.resize(w,h,0);
  int ct = 0;
  for (int y=0; y<h; y++) {
    for (int x=0; x<w; x++) {
      //char buf[256];
      //snprintf(buf,sizeof(buf),"%d",ct);
      ct++;
      s.cell(x,y) = ct;
    }
  }
  printf("%s",s.encode().c_str());
  return 0;
}

