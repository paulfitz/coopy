
#include <stdio.h>
#include <getopt.h>

#include <coopy/PolyBook.h>
#include <coopy/CsvFile.h>
#include <coopy/CsvPatch.h>
#include <coopy/FormatSniffer.h>
#include <coopy/PatchParser.h>
#include <coopy/SheetPatcher.h>
#include <coopy/MergeOutputVerboseDiff.h>
#include <coopy/Dbg.h>

using namespace coopy::store;
using namespace coopy::cmp;
using namespace coopy::format;

using namespace std;

bool copy_file(const char *src, const char *dest) {
  FILE *fin = NULL;
  FILE *fout = NULL;
  fin = fopen(src,"rb");
  if (fin==NULL) return false;
  fout = fopen(dest,"wb");
  if (fout==NULL) {
    fclose(fin);
    return false;
  }
  char buf[32768];
  int bytes_read = 0;
  while ((bytes_read=fread(buf,1,sizeof(buf),fin))>0) {
    fwrite(buf,1,bytes_read,fout);
  }
  fclose(fout);
  fclose(fin);
  return true;
}

int main(int argc, char *argv[]) {
  bool verbose = false;
  bool fake = false;
  string output = "-";
  string tmp = "-";
  while (true) {
    int option_index = 0;
    static struct option long_options[] = {
      {"verbose", 0, 0, 'v'},
      {"output", 1, 0, 'o'},
      {"fake", 0, 0, 'k'},
      {"tmp", 1, 0, 't'},
      {"output-format", 1, 0, 'f'},
      {0, 0, 0, 0}
    };

    int c = getopt_long(argc, argv, "",
			long_options, &option_index);
    if (c==-1) break;
    switch (c) {
    case 'v':
      verbose = true;
      break;
    case 'k':
      fake = true;
      break;
    case 'o':
      output = optarg;
      break;
    case 't':
      tmp = optarg;
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
    printf("  sspatch [--output output.sqlite] [--tmp tmp.sqlite] db.sqlite patch.txt\n");
    //printf("  sspatch [--output-format FORMAT] sheet.csv patch.txt\n");
    printf("Output defaults to standard output.\n");
    printf("Write output to original file by doing:\n");
    printf("  sspatch --output sheet.csv sheet.csv patch.txt\n");
    printf("Get debug info about what a patch would do:\n");
    printf("  sspatch --fake sheet.csv patch.txt\n");
    return 1;
  }

  if (verbose) {
    coopy_set_verbose(verbose);
  }

  PolyBook local;
  CsvSheet patch;
  if (!local.read(argv[0])) {
    fprintf(stderr,"Failed to read %s\n", argv[0]);
    return 1;
  }

  if (local.inplace()&&output!=argv[0]) {
    if (output=="-"&&tmp=="-") {
      fprintf(stderr,"Inplace operation; please confirm output or specify a 'tmp' location\n");
      return 1;
    }
    if (tmp=="-") {
      tmp = output;
    }
    if (!copy_file(argv[0],tmp.c_str())) {
      fprintf(stderr,"Failed to write %s\n", output.c_str());
      return 1;
    }
    if (!local.read(tmp.c_str())) {
      fprintf(stderr,"Failed to switch to %s\n", output.c_str());
      return 1;
    }
  }
  
  FormatSniffer sniffer;
  sniffer.open(argv[1]);
  Format format = sniffer.getFormat();

  if (format.id!=FORMAT_PATCH_CSV) {
    fprintf(stderr,"Only CSV format patches are supported right now\n");
    fprintf(stderr,"Use ssdiff with --format-csv option.\n");
    return 1;
  }

  PolySheet sheet = local.readSheetByIndex(0);
  if (!sheet.isValid()) {
    fprintf(stderr, "No sheet found\n");
    return 1;
  }
  SheetPatcher patcher(&sheet);
  MergeOutputVerboseDiff fakePatcher;
  PatchParser parser(fake?((Patcher*)(&fakePatcher)):((Patcher*)(&patcher)),
		     &sniffer);
  if (!parser.apply()) {
    fprintf(stderr,"Patch application failed\n");
    return 1;
  }

  if (fake) {
    return 0;
  }
  
  //if (CsvFile::write(local,output.c_str())!=0) {
  if ((!local.inplace())||(tmp!=output)) {
    if (output!=argv[0] || !local.inplace()) {
      if (!local.write(output.c_str())) {
	fprintf(stderr,"Failed to write %s\n", output.c_str());
	return 1;
      }
    }
  }
  return 0;
}

