#include <stdio.h>

#include <coopy/PolyBook.h>
#include <coopy/Dbg.h>

using namespace coopy::store;

int main(int argc, char *argv[]) {
  if (argc>1) {
    if (std::string(argv[1])=="--verbose") {
      coopy_set_verbose(true);
      argc--;
      argv++;
    }
  }

  if (argc!=3) {
    printf("Call with input file and desired output file.\n");
    printf("E.G. to convert from comma-separated to tab-separated format, call as:\n");
    printf("  ssformat input.csv output.tsv\n");
    printf("E.G. to convert from comma-separated to sqlite-readable sql, call as:\n");
    printf("  ssformat input.csv output.sql\n");
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
