#include <stdio.h>

#include <coopy/PolyBook.h>
#include <coopy/NameSniffer.h>
#include <coopy/ShortTextBook.h>
#include <coopy/Dbg.h>

using namespace coopy::store;

int main(int argc, char *argv[]) {
  bool extractHeader = false;
  while (argc>1) {
    if (argv[1][0]!='-') break;
    if (argv[1][1]!='-') break;
    if (std::string(argv[1])=="--verbose") {
      coopy_set_verbose(true);
      argc--;
      argv++;
    }
    if (std::string(argv[1])=="--header") {
      extractHeader = true;
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
  if (extractHeader) {
    PolySheet sheet = src.readSheetByIndex(0);
    NameSniffer sniff(sheet);
    ShortTextBook *book = new ShortTextBook();
    if (book==NULL) {
      fprintf(stderr,"Failed to allocate output\n");
      return 1;
    }
    for (int i=0; i<sheet.width(); i++) {
      book->sheet.addField(sniff.suggestColumnName(i).c_str(),false);
    }
    book->sheet.addRecord();
    src.take(book);
  }
  if (!src.write(argv[2])) {
    fprintf(stderr,"Failed to write %s\n", argv[2]);
    return 1;
  }

  return 0;
}
