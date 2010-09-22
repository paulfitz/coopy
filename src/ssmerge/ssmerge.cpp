
#include <stdio.h>
#include <getopt.h>

#include <coopy/CsvFile.h>
#include <coopy/BookCompare.h>
#include <coopy/MergeOutputAccum.h>
#include <coopy/PolyBook.h>

using namespace coopy::store;

int main(int argc, char *argv[]) {
  bool head_trimmed = false;
  bool tail_trimmed = false;
  std::string output = "-";
  bool verbose = false;
  while (true) {
    int option_index = 0;
    static struct option long_options[] = {
      {"head-trimmed", 0, 0, 'h'},
      {"tail-trimmed", 0, 0, 't'},
      {"verbose", 0, 0, 'v'},
      {"output", 1, 0, 'o'},
      {0, 0, 0, 0}
    };

    int c = getopt_long(argc, argv, "",
			long_options, &option_index);
    if (c==-1) break;
    switch (c) {
    case 'h':
      head_trimmed = true;
      break;
    case 't':
      tail_trimmed = true;
      break;
    case 'v':
      verbose = true;
      break;
    case 'o':
      output = optarg;
      break;
    default:
      fprintf(stderr, "Unrecognized option\n");
      return 1;
    }
  }

  if (optind<argc-4) {
    fprintf(stderr, "Options not understood\n");
    return 1;
  }
  argc -= optind;
  argv += optind;

  if (argc<3) {
    printf("Merge spreadsheets, with sensible treatment of changes:\n");
    printf("  ssmerge [--output output.csv] parent.csv local.csv remote.csv\n");
    printf("Note: parent.csv should be a 'common ancestor' of the other two.\n");
    printf("The merged or conflicting result will be placed in the specified output file.\n");
    printf("If one or more inputs have been trimmed, let ssmerge know so it won't\ntreat trimming as implying deletion:\n");
    printf("  ssmerge [--head-trimmed] [--tail-trimmed] parent.csv local.csv remote.csv\n");
    return 1;
  }
  PolyBook local, remote, parent;
  MergeOutputAccum accum;
  if (!parent.read(argv[0])) {
    fprintf(stderr,"Failed to read %s\n", argv[0]);
    return 1;
  }
  if (!local.read(argv[1])) {
    fprintf(stderr,"Failed to read %s\n", argv[1]);
    return 1;
  }
  if (!remote.read(argv[2])) {
    fprintf(stderr,"Failed to read %s\n", argv[2]);
    return 1;
  }
  CompareFlags flags;
  flags.head_trimmed = head_trimmed;
  flags.tail_trimmed = tail_trimmed;
  BookCompare cmp;
  cmp.setVerbose(verbose);
  cmp.compare(parent,local,remote,accum,flags);
  if (CsvFile::write(accum.get(),output.c_str())!=0) {
    return 1;
  }
  return 0;
}

