#include <coopy/DiffRender.h>

using namespace coopy::light;
using namespace std;

bool DiffRender::render(const Table& table) {
  begin_table();
  int w = table.width();
  int h = table.height();
  int change_row = -1;
  for (int y=0; y<h; y++) {
    string row_mode = "";
    Cell cell = { "", true };
    bool open = false;
    if (w>0) {
      cell = table.get_cell(0,y);
      if (cell.txt=="@"||cell.txt=="@@") {
	row_mode = "@@";
      } else if (cell.txt=="!"||cell.txt=="+++"||cell.txt=="---") {
	row_mode = cell.txt;
	if (cell.txt=="!") { change_row = y; }
      } else {
	open = true;
      }
    }
    string cmd = cell.txt;
    begin_row(row_mode);
    for (int x=0; x<w; x++) {
      cell = table.get_cell(x,y);
      string cell_mode = "";
      string separator = "";
      if (open && change_row!=-1) {
	string change = table.get_cell(x,change_row).txt;
	if (change=="+++"||change=="---") {
	  cell_mode = change;
	}
      }
      if (cmd.find("->")!=string::npos) {
	if (cell.txt.find(cmd)!=string::npos) {
	  cell_mode = "->";
	  separator = cmd;
	}
      }
      insert_cell(cell,cell_mode,separator);
    }
    end_row();
  }
  end_table();
  return true;
}
