#include <coopy/LogDiffRender.h>
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
  if (argc<2||argc>3) return 1;
  const char *in_name = argv[1];
  const char *out_name = "-";
  if (argc==3) {
    out_name = argv[2];
  }
  Coopy coopy;
  PolyBook book = coopy.loadBook(in_name);
  if (!book.isValid()) return 2;
  if (book.getSheetCount()!=1) return 3;
  PolySheet sheet = book.readSheetByIndex(0);
  LightTable table;
  table.sheet = sheet;
  LogDiffRender render;
  render.render(table);
  FileIO fout;
  if (!fout.openForWrite(out_name,Property())) {
    return 4;
  }
  for (list<LogCell>::iterator it = render.cell_log.begin();
       it != render.cell_log.end(); it++) {
    fprintf(fout.get(),"%d,%d [%s/%s] %s : %s\n", it->col, it->row,
	    it->cell_mode.c_str(),
	    it->row_mode.c_str(),
	    it->separator.c_str(),
	    it->cell.txt.c_str());
  }
  return 0;
}
