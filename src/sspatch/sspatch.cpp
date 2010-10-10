
#include <stdio.h>
#include <getopt.h>

#include <coopy/CsvFile.h>
#include <coopy/CsvPatch.h>
#include <coopy/FormatSniffer.h>
#include <coopy/PatchParser.h>
#include <coopy/CsvPatcher.h>
#include <coopy/Dbg.h>

using namespace coopy::store;
using namespace coopy::cmp;
using namespace coopy::format;

using namespace std;

int main(int argc, char *argv[]) {
  bool verbose = false;
  string output = "-";
  while (true) {
    int option_index = 0;
    static struct option long_options[] = {
      {"verbose", 0, 0, 'v'},
      {"output", 1, 0, 'o'},
      {0, 0, 0, 0}
    };

    int c = getopt_long(argc, argv, "",
			long_options, &option_index);
    if (c==-1) break;
    switch (c) {
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

  if (optind<argc-2) {
    fprintf(stderr, "Options not understood\n");
    return 1;
  }
  argc -= optind;
  argv += optind;

  if (argc<2) {
    printf("Apply patch to a spreadsheet.\n");
    printf("  sspatch [--verbose] [--output output.csv] sheet.csv patch.txt\n");
    printf("Output defaults to standard output.\n");
    printf("Write output to original file by doing:\n");
    printf("  sspatch --output sheet.csv sheet.csv patch.txt\n");
    return 1;
  }

  if (verbose) {
    coopy_set_verbose(verbose);
  }

  CsvSheet local, patch;
  if (CsvFile::read(argv[0],local)!=0) {
    return 1;
  }
  
  FormatSniffer sniffer;
  sniffer.open(argv[1]);
  Format format = sniffer.getFormat();
  if (format.id == FORMAT_UNKNOWN) {
    // fall back on old, deprecated patch behavior
    
    if (CsvFile::read(argv[1],patch)!=0) {
      return 1;
    }
    CsvPatch patcher;
    patcher.apply(local,patch);
    const CsvSheet& result = patcher.get();
    if (argc>=4) {
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

  // new behavior
  if (format.id!=FORMAT_PATCH_CSV) {
    fprintf(stderr,"Only CSV format patches are supported right now\n");
    fprintf(stderr,"Use ssdiff with --format-csv option.\n");
    return 1;
  }

  CsvPatcher patcher(&local);
  PatchParser parser(&patcher,&sniffer);
  if (!parser.apply()) {
    fprintf(stderr,"Patch application failed\n");
    return 1;
  }
  
  if (CsvFile::write(local,output.c_str())!=0) {
    return 1;
  }
  return 0;
}

