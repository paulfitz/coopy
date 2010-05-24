
#include <stdio.h>
#include <getopt.h>

#include <coopy/SheetCompare.h>
#include <coopy/CsvFile.h>
#include <coopy/MergeOutputPatch.h>

int main(int argc, char *argv[]) {
  std::string output = "";
  while (true) {
    int this_option_optind = optind ? optind : 1;
    int option_index = 0;
    static struct option long_options[] = {
      {"format-csv", 0, 0, 'c'},
      {"format-sql", 0, 0, 's'},
      {"output", 1, 0, 'o'},
      {0, 0, 0, 0}
    };

    int c = getopt_long(argc, argv, "",
			long_options, &option_index);
    if (c==-1) break;
    switch (c) {
    case 'c':
      printf("Switching to csv mode\n");
      break;
    case 's':
      printf("Switching to sql mode\n");
      break;
    case 'o':
      output = optarg;
      break;
    default:
      fprintf(stderr, "Unrecognized option\n");
      return 1;
    }
  }

  if (optind<argc-3) {
    fprintf(stderr, "Options not understood\n");
    return 1;
  }
  argc -= optind;
  argv += optind;

  if (argc<2) {
    printf("Show difference between two CSV spreadsheets. Call as:\n");
    printf("  ssdiff [--output output.csv] local.csv modified.csv\n");
    printf("Output defaults to standard output.\n");
    return 1;
  }
  CsvSheet local, remote;
  if (CsvFile::read(argv[0],local)!=0) {
    return 1;
  }
  if (CsvFile::read(argv[1],remote)!=0) {
    return 1;
  }
  SheetCompare cmp;
  MergeOutputPatch patch;
  cmp.compare(local,local,remote,patch);
  const CsvSheet& result = patch.get();
  if (output!="") {
    if (CsvFile::write(result,output.c_str())!=0) {
      return 1;
    }
  } else {
    SheetStyle style;
    std::string out = result.encode(style);
    printf("%s",out.c_str());
  }
  return 0;
}

