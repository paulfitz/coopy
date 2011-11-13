#include <coopy/MergeOutputIndex.h>
#include <coopy/Dbg.h>

#include <coopy/EfficientMap.h>
#include <coopy/Stringer.h>

using namespace coopy::store;
using namespace coopy::cmp;
using namespace std;

#define HELPER(x) (*((efficient_map<std::string,int> *)(x)))

MergeOutputIndex::MergeOutputIndex() {
  implementation = new efficient_map<std::string,int>;
  COOPY_ASSERT(implementation);
}

MergeOutputIndex::~MergeOutputIndex() {
  delete &HELPER(implementation);
  implementation = NULL;
}

bool MergeOutputIndex::mergeStart() {
  return true;
}

bool MergeOutputIndex::setSheet(const char *name) {
  efficient_map<string,int>& seen = HELPER(implementation);
  seen.clear();
  links_column.clear();

  SimpleSheetSchema ss;
  ss.setSheetName(name);
  ss.addColumn("pivot",ColumnType("INTEGER"));
  ss.addColumn("local",ColumnType("INTEGER"));
  ss.addColumn("remote",ColumnType("INTEGER"));
  if (getOutputBook()!=NULL) {
    links = getOutputBook()->provideSheet(ss);
  }
  
  if (!links.isValid()) {
    fprintf(stderr,"* Could not generate links sheet\n");
    exit(1);
    return false;
  }

  SimpleSheetSchema links_column_schema;
  links_column_schema.setSheetName((string(name) + "_columns").c_str());
  links_column_schema.addColumn("pivot",ColumnType("INTEGER"));
  links_column_schema.addColumn("local",ColumnType("INTEGER"));
  links_column_schema.addColumn("remote",ColumnType("INTEGER"));
  links_column_schema.addColumn("local_name",ColumnType("TEXT"));
  links_column_schema.addColumn("remote_name",ColumnType("TEXT"));

  return true;
}


bool MergeOutputIndex::mergeDone() {
  return true;
}


static SheetCell link_cell(int x) {
  if (x>=0) return SheetCell(x);
  return SheetCell();
}

bool MergeOutputIndex::declareLink(const LinkDeclare& decl) {
  
  dbg_printf("LINK %d %d %d %d\n",
	 decl.mode,
	 decl.rc_id_pivot,
	 decl.rc_id_local,
	 decl.rc_id_remote);
  std::string mode = decl.column?"column":"row";

  if (decl.column) {
    if (!links_column.isValid()) {
      if (getOutputBook()!=NULL) {
	links_column = getOutputBook()->provideSheet(links_column_schema);
      }
      
      if (!links_column.isValid()) {
	fprintf(stderr,"* Could not generate links schema sheet\n");
	exit(1);
	return false;
      }
    }
  }

  std::string frame = "none";
  switch (decl.mode) {
  case LINK_DECLARE_MERGE:
    frame = "merge";
    break;
  case LINK_DECLARE_LOCAL:
    frame = "local";
    break;
  case LINK_DECLARE_REMOTE:
    frame = "remote";
    break;
  }

  string s = mode + "_" +
    stringer_encoder(decl.rc_id_pivot) + "_" +
    stringer_encoder(decl.rc_id_local) + "_" +
    stringer_encoder(decl.rc_id_remote) + "_" +
    stringer_encoder(decl.rc_deleted);

  efficient_map<string,int>& seen = HELPER(implementation);
  if (seen.find(s)==seen.end()) {
    seen[s] = 1;
    if (decl.column) {
      Poly<SheetRow> pRow = links_column.insertRow();
      SheetRow& row = *pRow;
      int at = 0;
      row.setCell(at,link_cell(decl.rc_id_pivot)); at++;
      row.setCell(at,link_cell(decl.rc_id_local)); at++;
      row.setCell(at,link_cell(decl.rc_id_remote)); at++;
      //row.setCell(at,SheetCell(decl.rc_deleted?1:0)); at++;
      row.setCell(at,SheetCell(decl.rc_str_local,false)); at++;
      row.setCell(at,SheetCell(decl.rc_str_remote,false)); at++;
      row.flush();
    } else {
      Poly<SheetRow> pRow = links.insertRow();
      SheetRow& row = *pRow;
      int at = 0;
      row.setCell(at,link_cell(decl.rc_id_pivot)); at++;
      row.setCell(at,link_cell(decl.rc_id_local)); at++;
      row.setCell(at,link_cell(decl.rc_id_remote)); at++;
      //row.setCell(at,SheetCell(decl.rc_deleted?1:0)); at++;
      row.flush();
    }
  }
  return true;
}
