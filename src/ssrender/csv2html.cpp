#include <stdio.h>
#include <coopy/CsvSheet.h>
#include <coopy/CsvFile.h>
#include <coopy/CsvRender.h>

using namespace std;
using namespace coopy::store;

int main(int argc, char *argv[]) {
  bool full = false;
  bool decorate = false;
  const char *fname = "-";
  while (argc>1) {
    string arg = argv[1];
    if (arg=="--full" || arg=="-f") {
      full = true;
      decorate = true;
    } else if (arg=="--decorate" || arg=="-d") {
      decorate = true;
    } else {
      fname = argv[1];
    }
    argv++;
    argc--;
  }
  CsvSheet sheet;
  if (CsvFile::read(fname,sheet)!=0) {
    printf("Failed to read input\n");
    return 1;
  }

  CsvRender render;
  render.setDecorate(decorate);
  render.setFull(full);
  string result = render.renderHtml(sheet);
  printf("%s", result.c_str());

  return 0;
}
