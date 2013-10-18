#include <coopy/Coopyhx.h>
#include <coopy/CoopyhxLink.h>

using namespace coopy::cmp;

class CoopyhxDataSheet : public CoopyhxTable {
public:
  coopy::store::DataSheet& sheet;
  std::vector<std::string> names;
  int w, h, dy;

  CoopyhxDataSheet(coopy::store::DataSheet& sheet) : sheet(sheet) {
    w = sheet.width();
    h = sheet.height();
    dy = 0;
    if (sheet.hasExternalColumnNames()) {
      h++;
      dy++;
      coopy::store::SheetSchema *schema = sheet.getSchema();
      if (schema==NULL) {
	printf("Coopyhx needs a schema, tell someone to fix Coopyhx.cpp\n");
	exit(1);
      }
      for (int i=0; i<schema->getColumnCount(); i++) {
	coopy::store::ColumnInfo ci = schema->getColumnInfo(i);
	names.push_back(ci.getName());
      }
    }
  }

  virtual int width() { return w; }
  virtual int height() { return h; }
  virtual int getOffset() { return dy; }
  virtual std::string getCell(int x, int y, bool& isNull) {
    if (y<dy) {
      if (x<(int)names.size()) {
	isNull = false;
	return names[x];
      }
      isNull = true;
      return "";
    }
    return sheet.cellString(x,y-dy,isNull);
  }
};

int CoopyhxCompare::compare(coopy::store::DataSheet& pivot, 
			    coopy::store::DataSheet& local, 
			    coopy::store::DataSheet& remote, 
			    coopy::cmp::OrderResult& p2l_row_order, 
			    coopy::cmp::OrderResult& p2r_row_order, 
			    coopy::cmp::OrderResult& p2l_col_order, 
			    coopy::cmp::OrderResult& p2r_col_order, 
			    const CompareFlags& flags) {
  CoopyhxLink link;
  CoopyhxDataSheet hxpivot(pivot);
  CoopyhxDataSheet hxlocal(local);
  CoopyhxDataSheet hxremote(remote);
  p2l_row.resize(1,pivot.height(),-1);
  p2l_col.resize(1,pivot.width(),-1);
  p2r_row.resize(1,pivot.height(),-1);
  p2r_col.resize(1,pivot.width(),-1);
  l2p_row.resize(1,local.height(),-1);
  l2p_col.resize(1,local.width(),-1);
  r2p_row.resize(1,remote.height(),-1);
  r2p_col.resize(1,remote.width(),-1);
  link.compare(hxpivot,hxlocal,hxremote,*this);
  p2l_row_order.setup(p2l_row,l2p_row);
  p2r_row_order.setup(p2r_row,r2p_row);
  p2l_col_order.setup(p2l_col,l2p_col);
  p2r_col_order.setup(p2r_col,r2p_col);
  return 0;
}

void CoopyhxCompare::order(int a, int b, bool row, bool p2l) {
  if (a<0||b<0) return;
  //printf("  %d -> %d    %s %s\n", a, b, row?"row":"col", p2l?"p2l":"p2r");
  if (row) {
    if (p2l) {
      p2l_row.cell(0,a) = b;
      l2p_row.cell(0,b) = a;
    } else {
      p2r_row.cell(0,a) = b;
      r2p_row.cell(0,b) = a;
    }
  } else {
    if (p2l) {
      p2l_col.cell(0,a) = b;
      l2p_col.cell(0,b) = a;
    } else {
      p2r_col.cell(0,a) = b;
      r2p_col.cell(0,b) = a;
    }
  }
}

