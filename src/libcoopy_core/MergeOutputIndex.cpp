#include <coopy/MergeOutputIndex.h>
#include <coopy/Dbg.h>

#include <coopy/EfficientMap.h>
#include <coopy/Stringer.h>
#include <coopy/IndexSniffer.h>

using namespace coopy::store;
using namespace coopy::cmp;
using namespace std;

#define HELPER(x) (*((efficient_map<std::string,int> *)(x)))

MergeOutputIndex::MergeOutputIndex() {
  implementation = new efficient_map<std::string,int>;
  COOPY_ASSERT(implementation);
  pending_row = true;
  sheet_set = false;
}

MergeOutputIndex::~MergeOutputIndex() {
  delete &HELPER(implementation);
  implementation = NULL;
}

bool MergeOutputIndex::mergeStart() {
  pending_row = true;
  sheet_set = false;
  return true;
}

bool MergeOutputIndex::setSheet(const char *name) {
  this->name = name;

  sheet_set = true;
  efficient_map<string,int>& seen = HELPER(implementation);
  seen.clear();
  links_column.clear();
  links.clear();
  pending_row = true;

  links_column_schema = SimpleSheetSchema();
  links_column_schema.setSheetName((string(name) + "_columns").c_str());
  if (flags.has_pivot) {
    links_column_schema.addColumn("p_offset",ColumnType("INTEGER"));
  }
  links_column_schema.addColumn("l_name",ColumnType("TEXT"));
  links_column_schema.addColumn("r_name",ColumnType("TEXT"));
  links_column_schema.addColumn("l_offset",ColumnType("INTEGER"));
  links_column_schema.addColumn("r_offset",ColumnType("INTEGER"));

  return true;
}


bool MergeOutputIndex::mergeDone() {
  return true;
}


bool MergeOutputIndex::mergeAllDone() {
  return true;
}


static SheetCell link_cell(int x) {
  if (x>=0) return SheetCell(x);
  return SheetCell();
}

static SheetCell link_cell(PolySheet& sheet, int x, int y) {
  if (y<0) {
    return SheetCell();
  }
  return sheet.cellSummary(x,y);
}

bool MergeOutputIndex::declareLink(const LinkDeclare& decl) {
  std::string mode = decl.column?"column":"row";
  dbg_printf("LINK %s %d %d %d // %s %s %s\n",
	     mode.c_str(),
	     decl.rc_id_pivot,
	     decl.rc_id_local,
	     decl.rc_id_remote,
	     decl.rc_str_pivot.c_str(),
	     decl.rc_str_local.c_str(),
	     decl.rc_str_remote.c_str());

  if (!sheet_set) {
    setSheet("sheet");
  }

  if (pending_row && !decl.column) {
    pending_row = false;
    pivot = decl.pivot;
    //printf("WORKING ON A %d x %d sheet: %s\n", pivot.width(), pivot.height(), pivot.toString().c_str());
    local = decl.local;
    remote = decl.remote;
    if (!pivot.isValid()) {
      fprintf(stderr,"Table not provided\n");
      exit(1);
    }
    
    pivot.mustHaveSchema();
    local.mustHaveSchema();
    remote.mustHaveSchema();
    NameSniffer npivot(pivot,flags);
    NameSniffer nlocal(local,flags);
    NameSniffer nremote(remote,flags);
    IndexSniffer xpivot(pivot,flags,npivot);
    IndexSniffer xlocal(local,flags,nlocal);
    IndexSniffer xremote(remote,flags,nremote);
    ipivot = xpivot.suggestIndexes();
    ilocal = xlocal.suggestIndexes();
    iremote = xremote.suggestIndexes();
    //printf("%d %d %d\n", ipivot.size(), ilocal.size(), iremote.size());

    SimpleSheetSchema ss;
    ss.setSheetName(name.c_str());
    if (flags.has_pivot) {
      for (int i=0; i<(int)ipivot.size(); i++) {
	if (ipivot[i]) {
	  ss.addColumn((string("p_")+npivot.suggestColumnName(i)).c_str(),
		       npivot.suggestColumnType(i));
	}
      }
    }
    {
      for (int i=0; i<(int)ilocal.size(); i++) {
	if (ilocal[i]) {
	  ss.addColumn((string("l_")+nlocal.suggestColumnName(i)).c_str(),
		       nlocal.suggestColumnType(i));
	}
      }
    }
    {
      for (int i=0; i<(int)iremote.size(); i++) {
	if (iremote[i]) {
	  ss.addColumn((string("r_")+nremote.suggestColumnName(i)).c_str(),
		       nremote.suggestColumnType(i));
	}
      }
    }
    if (getOutputBook()!=NULL) {
      links = getOutputBook()->provideSheet(ss);
    }
    
    if (!links.isValid()) {
      fprintf(stderr,"* Could not generate links sheet\n");
      exit(1);
      return false;
    }
    links.deleteData();
  }
  
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
      links_column.deleteData();
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
      if (flags.has_pivot) {
	row.setCell(at,link_cell(decl.rc_id_pivot)); at++;
      }
      //row.setCell(at,SheetCell(decl.rc_deleted?1:0)); at++;
      row.setCell(at,SheetCell(decl.rc_str_local,false)); at++;
      row.setCell(at,SheetCell(decl.rc_str_remote,false)); at++;
      row.setCell(at,link_cell(decl.rc_id_local)); at++;
      row.setCell(at,link_cell(decl.rc_id_remote)); at++;
      row.flush();
    } else {
      Poly<SheetRow> pRow = links.insertRow();
      SheetRow& row = *pRow;
      int at = 0;
      if (flags.has_pivot) {
	for (int i=0; i<(int)ipivot.size(); i++) {
	  if (ipivot[i]) {
	    row.setCell(at,link_cell(pivot,i,decl.rc_id_pivot)); at++;
	  }
	}
      }
      {
	for (int i=0; i<(int)ilocal.size(); i++) {
	  if (ilocal[i]) {
	    row.setCell(at,link_cell(local,i,decl.rc_id_local)); at++;
	  }
	}
      }
      {
	for (int i=0; i<(int)iremote.size(); i++) {
	  if (iremote[i]) {
	    row.setCell(at,link_cell(remote,i,decl.rc_id_remote)); at++;
	  }
	}
      }
	/*
	// not yet ready for showing multi-column keys etc
	row.setCell(at,link_cell(decl.rc_id_pivot)); at++;
	row.setCell(at,link_cell(decl.rc_id_local)); at++;
	row.setCell(at,link_cell(decl.rc_id_remote)); at++;
	*/
      //row.setCell(at,SheetCell(decl.rc_deleted?1:0)); at++;
      row.flush();
    }
  }
  return true;
}
