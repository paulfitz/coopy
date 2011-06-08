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
  {
    SimpleSheetSchema ss;
    ss.setSheetName("links");
    ss.addColumn("category",ColumnType("TEXT"));
    //ss.addColumn("frame",ColumnType("TEXT"));
    ss.addColumn("pivot",ColumnType("INTEGER"));
    ss.addColumn("local",ColumnType("INTEGER"));
    ss.addColumn("remote",ColumnType("INTEGER"));
    ss.addColumn("deleted",ColumnType("INTEGER"));
    if (getOutputBook()!=NULL) {
      links = getOutputBook()->provideSheet(ss);
    }
    
    if (!links.isValid()) {
      fprintf(stderr,"* Could not generate links sheet\n");
      exit(1);
      return false;
    }
  }

  /*
  {
    SimpleSheetSchema ss;
    ss.setSheetName("identity");
    ss.addColumn("frame",ColumnType("TEXT"));
    ss.addColumn("column",ColumnType("TEXT"));
    ss.addColumn("identity",ColumnType("INTEGER"));
    identity = getBook()->provideSheet(ss);
    
    if (!identity.isValid()) {
      fprintf(stderr,"* Could not generate identity sheet\n");
      exit(1);
      return false;
    }
  }
  */

  return true;
}

bool MergeOutputIndex::mergeDone() {
  return true;
}

bool MergeOutputIndex::declareLink(const LinkDeclare& decl) {
  
  dbg_printf("LINK %d %d %d %d\n",
	     decl.mode,
	     decl.rc_id_pivot,
	     decl.rc_id_local,
	     decl.rc_id_remote);
  std::string mode = decl.column?"column":"row";
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
  /*
  row.setCell(0,SheetCell(frame,false));
  row.setCell(1,SheetCell(mode,false));
  row.setCell(2,SheetCell(decl.rc_id_pivot));
  row.setCell(3,SheetCell(decl.rc_id_local));
  row.setCell(4,SheetCell(decl.rc_id_remote));
  row.setCell(5,SheetCell(decl.rc_deleted?1:0));
  row.flush();
  */

  string s = mode + "_" +
    stringer_encoder(decl.rc_id_pivot) + "_" +
    stringer_encoder(decl.rc_id_local) + "_" +
    stringer_encoder(decl.rc_id_remote) + "_" +
    stringer_encoder(decl.rc_deleted);

  //printf("string is %s\n", s.c_str());
  efficient_map<string,int>& seen = HELPER(implementation);
  if (seen.find(s)==seen.end()) {
    Poly<SheetRow> pRow = links.insertRow();
    SheetRow& row = *pRow;
    seen[s] = 1;
    int at = 0;
    row.setCell(at,SheetCell(mode,false)); at++;
    //row.setCell(at,SheetCell(frame,false)); at++;
    row.setCell(at,SheetCell(decl.rc_id_pivot)); at++;
    row.setCell(at,SheetCell(decl.rc_id_local)); at++;
    row.setCell(at,SheetCell(decl.rc_id_remote)); at++;
    row.setCell(at,SheetCell(decl.rc_deleted?1:0)); at++;
    row.flush();
  }
  return true;
}

/*
bool MergeOutputIndex::startOutput(const std::string& output, 
				   CompareFlags& flags) {
  //if (!index.attach(output.c_str())) {
  //     return false;
  //   }
   return true;
}

bool MergeOutputIndex::stopOutput(const std::string& output, 
				  CompareFlags& flags) {

  //index.flush();
  return true;
}

*/
