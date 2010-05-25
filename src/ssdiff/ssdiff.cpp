
#include <stdio.h>
#include <getopt.h>

#include <coopy/CsvFile.h>
#include <coopy/MergeOutputPatch.h>
#include <coopy/MergeOutputSqlDiff.h>
#include <coopy/MergeOutputVerboseDiff.h>
#include <coopy/BookCompare.h>
#include <coopy/PolyBook.h>

int main(int argc, char *argv[]) {
  std::string output = "";
  std::string mode = "human";
  bool verbose = false;
  while (true) {
    int option_index = 0;
    static struct option long_options[] = {
      {"format-csv", 0, 0, 'c'},
      {"format-sql", 0, 0, 's'},
      {"format-human", 0, 0, 'h'},
      {"verbose", 0, 0, 'v'},
      {"output", 1, 0, 'o'},
      {0, 0, 0, 0}
    };

    int c = getopt_long(argc, argv, "",
			long_options, &option_index);
    if (c==-1) break;
    switch (c) {
    case 'c':
      mode = "csv";
      break;
    case 's':
      mode = "sql";
      break;
    case 'h':
      mode = "human";
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

  if (optind<argc-3) {
    fprintf(stderr, "Options not understood\n");
    return 1;
  }
  argc -= optind;
  argv += optind;

  if (argc<2) {
    printf("Show difference between two spreadsheets. Call as:\n");
    printf("  ssdiff [--output <filename>] local.csv modified.csv\n");
    printf("  ssdiff --format-human local.csv modified.csv # human readable output\n");
    printf("  ssdiff --format-csv local.csv modified.csv   # format that sspatch can read\n");
    printf("  ssdiff --format-sql local.csv modified.csv   # not working yet\n");
    printf("Output defaults to standard output.\n");
    return 1;
  }
  /*
  CsvSheet local, remote;
  if (CsvFile::read(argv[0],local)!=0) {
    return 1;
  }
  if (CsvFile::read(argv[1],remote)!=0) {
    return 1;
  }
  */
  BookCompare cmp;
  cmp.setVerbose(verbose);

  PolyBook local, remote;
  if (!local.read(argv[0])) {
    fprintf(stderr,"Failed to read %s\n", argv[0]);
    return 1;
  }
  if (!remote.read(argv[1])) {
    fprintf(stderr,"Failed to read %s\n", argv[1]);
    return 1;
  }
  if (mode=="sql") {
    MergeOutputSqlDiff sqldiff;
    cmp.compare(local,local,remote,sqldiff);
  } else if (mode=="human") {
    MergeOutputVerboseDiff humandiff;
    cmp.compare(local,local,remote,humandiff);
  } else {
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
  }
  return 0;
}

