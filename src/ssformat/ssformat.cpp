#include <stdio.h>
#include <getopt.h>

#include <coopy/PolyBook.h>
#include <coopy/NameSniffer.h>
#include <coopy/IndexSniffer.h>
#include <coopy/ShortTextBook.h>
#include <coopy/CsvTextBook.h>
#include <coopy/Dbg.h>
#include <coopy/CompareFlags.h>

using namespace coopy::store;
using namespace coopy::cmp;
using namespace std;

int main(int argc, char *argv[]) {
  bool extractHeader = false;
  bool extractIndex = false;
  bool verbose = false;
  bool help = false;
  string sheetSelection = "";

  while (true) {
    int option_index = 0;
    static struct option long_options[] = {
      {"verbose", 0, 0, 'v'},
      {"header", 0, 0, 'h'},
      {"index", 0, 0, 'i'},
      {"help", 0, 0, 'H'},
      {"sheet", 1, 0, 's'},
      {"table", 1, 0, 't'},
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
    case 'h':
      extractHeader = true;
      break;
    case 'H':
      help = true;
      break;
    case 'i':
      extractIndex = true;
      break;
    case 's':
    case 't':
      sheetSelection = optarg;
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

  if (argc<1||help) {
    printf("Call with input file and desired output file. Examples:\n");
    printf("  ssformat input.csv output.sqlite\n");
    printf("  ssformat input.sqlite output.csvs\n");
    printf("  ssformat dbi:mysql:database_name:host=localhost:username=root output.sqlite\n");
    printf("  ssformat input.sqlite\n");
    printf("Subsets of data can be extracted:\n");
    printf("  ssformat --table people input.sqlite people.csv # extract single table\n");
    printf("  ssformat --header people.csv # show just the header line, if one is detected\n");
    printf("  ssformat --index people.csv  # show just the key columns, if detected\n");
    printf("If the output file is omitted, it is set to standard output\n");
    return 1;
  }

  PolyBook src;
  if (!src.read(argv[0])) {
    fprintf(stderr,"Failed to read %s\n", argv[0]);
    return 1;
  }
  if (sheetSelection!="") {
    CsvTextBook *book = new CsvTextBook(true);
    if (book==NULL) {
      fprintf(stderr,"Failed to allocate output\n");
      return 1;
    }
    Property p;
    p.put("sheet",sheetSelection.c_str());
    book->copy(src,p);
    src.take(book);

    //vector<string> names = book->getNames();
    //printf("have %d\n", names.size());
  }
  if (extractHeader) {
    PolySheet sheet = src.readSheetByIndex(0);
    CompareFlags flags;
    NameSniffer sniff(sheet,flags);
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
  if (extractIndex) {
    PolySheet sheet = src.readSheetByIndex(0);
    CompareFlags flags;
    NameSniffer nsniff(sheet,flags);
    IndexSniffer sniff(sheet,flags,nsniff);
    vector<int> indexes = sniff.suggestIndexes();
    dbg_printf("Index count %d\n", (int)indexes.size());
    ShortTextBook *book = new ShortTextBook();
    if (book==NULL) {
      fprintf(stderr,"Failed to allocate output\n");
      return 1;
    }
    book->sheet.copy(sheet);
    int at = 0;
    for (int i=0; i<sheet.width(); i++) {
      if (indexes[i]==0) {
	book->sheet.deleteColumn(at);
      } else {
	at++;
      }
    }
    src.take(book);
  }
  string out_file = "-";
  if (argc==2) {
    out_file = argv[1];
  }
  if (!src.write(out_file.c_str())) {
    fprintf(stderr,"Failed to write %s\n", out_file.c_str());
    return 1;
  }

  return 0;
}
