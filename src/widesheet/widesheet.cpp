#include <stdio.h>
#include <getopt.h>

#include <coopy/Dbg.h>

//using namespace std;

int main(int argc, char *argv[]) {
  bool verbose = false;

  while (true) {
    int option_index = 0;
    static struct option long_options[] = {
      {"verbose", 0, 0, 'v'},
      {0, 0, 0, 0}
    };

    int c = getopt_long(argc, argv, "",
			long_options, &option_index);
    if (c==-1) break;
    switch (c) {
    case 'v':
      verbose = true;
      coopy_set_verbose(true);
      break;
    default:
      fprintf(stderr, "Unrecognized option\n");
      return 1;
    }
  }

  argc -= optind;
  argv += optind;

  if (argc<1) {
    printf("Welcome to widesheet.\n");
    return 1;
  }

  return 0;
}
