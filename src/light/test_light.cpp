#include <coopy/LogDiffRender.h>
#include <coopy/HtmlDiffRender.h>
#include <coopy/Coopy.h>
#include <coopy/FileIO.h>

#include <stdio.h>

using namespace coopy::app;
using namespace coopy::store;
using namespace coopy::light;
using namespace std;

class LightTable : public Table {
public:
  PolySheet sheet;

  virtual int width() const { return sheet.width(); }
  virtual int height() const { return sheet.height(); }

  virtual Cell get_cell(int x, int y) const {
    Cell cell = { "", false };
    SheetCell cell2 = sheet.cellSummary(x,y);
    cell.txt = cell2.text;
    cell.is_null = cell2.escaped;
    return cell;
  }
};

int main(int argc, char *argv[]) {
  if (argc<2||argc>4) return 1;
  const char *in_name = argv[1];
  char format = 'l';  // "log" or "html"
  const char *out_name = "-";
  if (argc>=3) {
    format = argv[2][0];
  }
  if (argc==4) {
    out_name = argv[3];
  }
  Coopy coopy;
  PolyBook book = coopy.loadBook(in_name);
  if (!book.isValid()) return 2;
  if (book.getSheetCount()!=1) return 3;
  PolySheet sheet = book.readSheetByIndex(0);
  LightTable table;
  table.sheet = sheet;
  DiffRender *render = NULL;
  if (format=='l') render = new LogDiffRender();
  if (format=='h') render = new HtmlDiffRender();
  if (!render) return 4;
  render->render(table);
  FileIO fout;
  if (!fout.openForWrite(out_name,Property())) {
    return 5;
  }
  fprintf(fout.get(),"%s",render->to_string().c_str());
  delete render;
  render = NULL;
  return 0;
}
