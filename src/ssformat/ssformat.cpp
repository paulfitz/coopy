#include <stdio.h>

#include <coopy/PolyBook.h>

int main(int argc, char *argv[]) {
  if (argc!=3) {
    printf("Call with input file and desired output file.\n");
    printf("E.G. to convert from comma-separated to tab-separated format, call as:\n");
    printf("  ssformat input.csv output.tsv\n");
    return 1;
  }

  PolyBook src;
  if (!src.read(argv[1])) {
    fprintf(stderr,"Failed to read %s\n", argv[1]);
    return 1;
  }
  if (!src.write(argv[2])) {
    fprintf(stderr,"Failed to write %s\n", argv[2]);
    return 1;
  }

  return 0;
}
