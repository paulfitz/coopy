
#include <stdio.h>

#include <coopy/CsvFile.h>
#include <coopy/CsvPatch.h>
#include <coopy/FormatSniffer.h>
#include <coopy/PatchParser.h>
#include <coopy/CsvPatcher.h>

using namespace coopy::store;
using namespace coopy::cmp;
using namespace coopy::format;

int main(int argc, char *argv[]) {
  if (argc<3) {
    printf("Apply patch to a spreadsheet. Currently doesn't do much.\n");
    printf("  sspatch sheet.csv patch.txt [output.csv]\n");
    printf("Output defaults to standard output.\n");
    printf("Write output to original file by doing:.\n");
    printf("  sspatch sheet.csv patch.txt sheet.csv\n");
    return 1;
  }
  CsvSheet local, patch;
  if (CsvFile::read(argv[1],local)!=0) {
    return 1;
  }
  
  FormatSniffer sniffer;
  sniffer.open(argv[2]);
  Format format = sniffer.getFormat();
  if (format.id == FORMAT_UNKNOWN) {
    // fall back on old, deprecated patch behavior
    
    if (CsvFile::read(argv[2],patch)!=0) {
      return 1;
    }
    CsvPatch patcher;
    patcher.apply(local,patch);
    const CsvSheet& result = patcher.get();
    if (argc>=4) {
      if (CsvFile::write(result,argv[3])!=0) {
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
  
  if (argc>=4) {
    if (CsvFile::write(local,argv[3])!=0) {
      return 1;
    }
  } else {
    SheetStyle style;
    std::string out = local.encode(style);
    printf("%s",out.c_str());
  }  
  return 0;
}

