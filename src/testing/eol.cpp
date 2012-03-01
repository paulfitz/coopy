#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc!=3) return 1;
  FILE *fin = fopen(argv[1],"rb");
  if (!fin) return 2;
  FILE *fout = fopen(argv[2],"wb");
  if (!fout) return 3;

  char buf[1000];
  int r = -1;
  do {
    r = fread(buf,1,sizeof(buf),fin);
    if (r>0) {
      int len = 0;
      for (int i=0; i<r; i++) {
	if (buf[i]=='\r') continue;
	if (len<i) {
	  buf[len] = buf[i];
	}
	len++;
      }
      fwrite(buf,1,len,fout);
    }
  } while (r>0);

  fclose(fout);
  fclose(fin);
  return 0;
}
