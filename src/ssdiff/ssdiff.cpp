
#include <stdio.h>
#include <getopt.h>

#include <coopy/CsvFile.h>
#include <coopy/MergeOutputPatch.h>
#include <coopy/MergeOutputSqlDiff.h>
#include <coopy/MergeOutputHumanDiff.h>
#include <coopy/MergeOutputVerboseDiff.h>
#include <coopy/MergeOutputCsvDiff.h>
#include <coopy/BookCompare.h>
#include <coopy/PolyBook.h>

using namespace coopy::store;
using namespace coopy::cmp;
using namespace std;

static void start_output(string output, CompareFlags& flags) {
  if (output=="" || output=="-") {
    flags.out = stdout;
    return;
  }
  FILE *fout = fopen(output.c_str(),"wb");
  if (fout==NULL) {
    fprintf(stderr,"Could not open %s for writing\n", output.c_str());
    exit(1);
  }
  flags.out = fout;
}

static void stop_output(string output, CompareFlags& flags) {
  if (flags.out!=stdout) {
    fclose(flags.out);
    flags.out = stdout;
  }
}

int main(int argc, char *argv[]) {
  std::string output = "";
  std::string mode = "human";
  std::string parent_file = "";
  std::string version = "";
  bool verbose = false;
  bool equality = false;
  while (true) {
    int option_index = 0;
    static struct option long_options[] = {
      {"format-csv", 0, 0, 'c'},
      {"format-csv-old", 0, 0, '0'},
      {"format-sql", 0, 0, 's'},
      {"format-human", 0, 0, 'h'},
      {"format-raw", 0, 0, 'r'},
      {"equals", 0, 0, 'e'},
      {"verbose", 0, 0, 'v'},
      {"output", 1, 0, 'o'},
      {"version", 1, 0, 'V'},
      {"parent", 1, 0, 'p'},
      {0, 0, 0, 0}
    };

    int c = getopt_long(argc, argv, "",
			long_options, &option_index);
    if (c==-1) break;
    switch (c) {
    case 'c':
      mode = "csv";
      break;
    case '0':
      mode = "csv0";
      break;
    case 's':
      mode = "sql";
      break;
    case 'h':
      mode = "human";
      break;
    case 'r':
      mode = "raw";
      break;
    case 'v':
      verbose = true;
      break;
    case 'e':
      equality = true;
      break;
    case 'o':
      output = optarg;
      break;
    case 'p':
      parent_file = optarg;
      break;
    case 'V':
      version = optarg;
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
    printf("  ssdiff [--output <filename>] [--parent parent.csv] reference.csv modified.csv\n");
    printf("  ssdiff --format-human local.csv modified.csv # human readable output\n");
    printf("  ssdiff --format-csv local.csv modified.csv   # format that sspatch can read\n");
    printf("  ssdiff --format-raw local.csv modified.csv   # full information\n");
    printf("  ssdiff --version 0.2 --format-csv local.csv modified.csv  # change version\n");
    printf("  ssdiff --version 0.4 --format-csv local.csv modified.csv  # change version\n");
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

  PolyBook _pivot;
  PolyBook *pivot;
  PolyBook _local;
  PolyBook *local = &_local;
  PolyBook _remote;
  PolyBook *remote = &_remote;
  
  if (!_local.read(argv[0])) {
    fprintf(stderr,"Failed to read %s\n", argv[0]);
    return 1;
  }
  if (!_remote.read(argv[1])) {
    fprintf(stderr,"Failed to read %s\n", argv[1]);
    return 1;
  }
  if (parent_file!="") {
    if (!_pivot.read(parent_file.c_str())) {
      fprintf(stderr,"Failed to read %s\n", parent_file.c_str());
      return 1;
    }
    pivot = &_pivot;
  } else {
    pivot = &_local;
  }

  if (equality) {
    if (*local == *remote) {
      return 0;
    }
    return 1;
  }

  CompareFlags flags;
  if (mode=="sql") {
    MergeOutputSqlDiff sqldiff;
    start_output(output,flags);
    cmp.compare(*pivot,*local,*remote,sqldiff,flags);
    stop_output(output,flags);
  } else if (mode=="human") {
    MergeOutputHumanDiff humandiff;
    start_output(output,flags);
    cmp.compare(*pivot,*local,*remote,humandiff,flags);
    stop_output(output,flags);
  } else if (mode=="raw") {
    MergeOutputVerboseDiff verbosediff;
    start_output(output,flags);
    cmp.compare(*pivot,*local,*remote,verbosediff,flags);
    stop_output(output,flags);
  } else if (mode=="csv") {
    if (version=="0.2") {
      MergeOutputCsvDiffV0p2 diff;
      start_output(output,flags);
      cmp.compare(*pivot,*local,*remote,diff,flags);
      stop_output(output,flags);
    } else if (version=="0.4"||version=="") {
      MergeOutputCsvDiff diff;
      start_output(output,flags);
      cmp.compare(*pivot,*local,*remote,diff,flags);
      stop_output(output,flags);
    } else {
      fprintf(stderr,"Unrecognized diff version\n");
      exit(1);
    }
  } else if (mode=="csv0") {
    MergeOutputPatch patch;
    cmp.compare(*pivot,*local,*remote,patch,flags);
    const CsvSheet& result = patch.get();
    if (output!="") {
      if (CsvFile::write(result,output.c_str())!=0) {
	return 1;
      }
    } else {
      SheetStyle style;
      std::string out = result.encode(style);
      printf("%s",out.c_str());
      /*
      if (output==""||output=="-") {
	printf("%s",out.c_str());
      } else {
	FILE *fout = fopen(output.c_str(),"wb");
	if (fout==NULL) {
	  fprintf(stderr,"Could not open %s for writing\n", output.c_str());
	  return 1;
	}
	fprintf(fout,"%s",out.c_str());
	fclose(fout);
	fout = NULL;
      }
      */
    }
  } else {
    fprintf(stderr,"Patch mode not recognized\n");
    return 1;
  }
  return 0;
}

