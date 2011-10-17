#include <stdio.h>
#include <getopt.h>

#include <coopy/PolyBook.h>
#include <coopy/NameSniffer.h>
#include <coopy/IndexSniffer.h>
#include <coopy/ShortTextBook.h>
#include <coopy/CsvTextBook.h>
#include <coopy/Dbg.h>
#include <coopy/CompareFlags.h>
#include <coopy/Options.h>

using namespace coopy::store;
using namespace coopy::cmp;
using namespace coopy::app;
using namespace std;

int main(int argc, char *argv[]) {
  Options opt("ssformat");
  int r = opt.apply(argc,argv);
  if (r!=0) return r;

  bool extractHeader = opt.checkBool("header");
  bool extractIndex = opt.checkBool("index");
  bool verbose = opt.checkBool("verbose");
  bool help = opt.checkBool("help");
  string inputFormat = opt.checkString("input-format");
  string outputFormat = opt.checkString("output-format");
  string sheetSelection = "";
  if (opt.getCompareFlags().tables.size()>1) {
    fprintf(stderr,"sorry, can only select one table right now\n");
    return 1;
  }
  if (opt.getCompareFlags().tables.size()==1) {
    sheetSelection = *(opt.getCompareFlags().tables.begin());
  }

  const vector<string>& core = opt.getCore();

  if (core.size()>2) {
    fprintf(stderr, "Options not understood\n");
    return 1;
  }

  if (core.size()<1||help) {
    /*
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
    */

    opt.beginHelp();
    opt.addUsage("ssformat [options] FILE");
    opt.addUsage("ssformat [options] FILE1 FILE2");
    opt.addDescription("Reformat tables/databases/spreadsheets.");
    opt.showOptions(OPTION_FOR_FORMAT);
    opt.endHelp();
    return 1;
  }

  PolyBook src;
  if (!src.read(core[0].c_str(),inputFormat.c_str())) {
    fprintf(stderr,"Failed to read %s\n", core[0].c_str());
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
  if (core.size()==2) {
    out_file = core[1];
  }
  if (!src.write(out_file.c_str(),outputFormat.c_str())) {
    fprintf(stderr,"Failed to write %s\n", out_file.c_str());
    return 1;
  }

  return 0;
}
