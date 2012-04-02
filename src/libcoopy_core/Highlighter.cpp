
#include <coopy/Highlighter.h>
#include <coopy/DiffRender.h>

#define FULL_COLOR (65535)
#define HALF_COLOR (65535/2)

using namespace coopy::store;
using namespace coopy::light;
using namespace std;

class HighlighterTable : public Table {
private:
  DataSheet& sheet;
public:

  HighlighterTable(DataSheet& sheet) : sheet(sheet) {}

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


class HighlighterDiffRender : public DiffRender {
private:
  int x, y;
  string row_mode;
  DataSheet& sheet;
public:
  HighlighterDiffRender(DataSheet& sheet) : sheet(sheet) {}

  virtual bool begin_table() { 
    x = y = 0;
    return true; 
  }

  virtual bool begin_row(const std::string& mode) {
    row_mode = mode;
    bool bold = false;
    int rr = 0, gg = 0, bb = 0;
    if (row_mode=="+++") {
      rr = HALF_COLOR;
      gg = FULL_COLOR;
      bb = HALF_COLOR;
      bold = true;
    } else if (row_mode=="---") {
      rr = FULL_COLOR;
      gg = HALF_COLOR;
      bb = HALF_COLOR;
    } else if (row_mode=="!") {
      rr = HALF_COLOR;
      gg = HALF_COLOR;
      bb = HALF_COLOR;
    } else if (row_mode=="@@") {
      rr = HALF_COLOR;
      gg = FULL_COLOR;
      bb = FULL_COLOR;
      bold = true;
    }
    if (rr!=0||gg!=0||bb!=0) {
      Poly<Appearance> appear = sheet.getRowAppearance(y);
      if (appear.isValid()) {
	appear->begin();
	appear->setBackgroundRgb16(rr,gg,bb,
				   AppearanceRange::full());
	if (bold) {
	  appear->setWeightBold(true,AppearanceRange::full());
	}
	appear->end();
      }
    }
    return true;
  }

  virtual bool insert_cell(const Cell& cell,
			   const std::string& mode,
			   const std::string& separator) {
    bool bold = false;
    int rr = 0, gg = 0, bb = 0;
    if (mode=="+++") {
      rr = HALF_COLOR;
      gg = FULL_COLOR;
      bb = HALF_COLOR;
      bold = true;
    } else if (mode=="---") {
      rr = FULL_COLOR;
      gg = HALF_COLOR;
      bb = HALF_COLOR;
    } else if (mode=="->") {
      rr = HALF_COLOR;
      gg = HALF_COLOR;
      bb = FULL_COLOR;
      bold = true;
    }
    if (rr!=0||gg!=0||bb!=0) {
      Poly<Appearance> appear = sheet.getCellAppearance(x,y);
      if (appear.isValid()) {
	appear->begin();
	appear->setBackgroundRgb16(rr,gg,bb,
				   AppearanceRange::full());
	if (bold) {
	  appear->setWeightBold(true,AppearanceRange::full());
	}
	appear->end();
      }
    }
    x++;
    return true;
  }

  virtual bool end_row() {
    x = 0;
    y++;
    return true;
  }

  virtual bool end_table() {
    return true;
  }

  virtual std::string to_string() const {
    return "";
  }
};


bool Highlighter::apply(DataSheet& sheet) {
  HighlighterTable table(sheet);
  HighlighterDiffRender render(sheet);
  return render.render(table);
}

bool Highlighter::apply(TextBook& book) {
  bool ok = true;
  int len = book.getSheetCount();
  for (int i=0; i<len; i++) {
    PolySheet sheet = book.readSheetByIndex(i);
    ok = ok && apply(sheet);
  }
  return ok;
}

