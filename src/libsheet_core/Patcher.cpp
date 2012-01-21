#include <coopy/Patcher.h>
#include <coopy/MergeOutputSqlDiff.h>
#include <coopy/MergeOutputHumanDiff.h>
#include <coopy/MergeOutputVerboseDiff.h>
#include <coopy/MergeOutputCsvDiff.h>
#include <coopy/MergeOutputTdiff.h>
#include <coopy/MergeOutputPatch.h>
#include <coopy/MergeOutputIndex.h>
#include <coopy/MergeOutputRowOps.h>
#include <coopy/SheetPatcher.h>

#include <algorithm>

using namespace std;
using namespace coopy::cmp;
using namespace coopy::store;

int OrderChange::identityToIndex(int id) const {
  //if (id==-1) return -1;
  vector<int>::const_iterator it = find(indicesBefore.begin(),
					indicesBefore.end(),id);
  if (it == indicesBefore.end()) { return -1; }
  return it-indicesBefore.begin();
}

int OrderChange::identityToIndexAfter(int id) const {
  //if (id==-1) return -1;
  vector<int>::const_iterator it = find(indicesAfter.begin(),
					indicesAfter.end(),id);
  if (it == indicesAfter.end()) { return -1; }
  return it-indicesAfter.begin();
}

Patcher *Patcher::createByName(const char *name, const char *version) {
  string mode(name);
  string _version;
  if (version!=NULL) _version = version;
  Patcher *result = NULL;
  if (mode=="sql") {
    result = new MergeOutputSqlDiff;
  } else if (mode=="human") {
    result = new MergeOutputHumanDiff;
  } else if (mode=="raw") {
    result = new MergeOutputVerboseDiff;
  } else if (mode=="tdiff") {
    result = new MergeOutputTdiff;
  } else if (mode=="apply") {
    result = SheetPatcher::createForApply();
  } else if (mode=="sheet"||mode=="color"||mode=="hilite"||mode=="highlight"||mode=="hiliter"||mode=="highlighter") {
    result = SheetPatcher::createForDescription();
  } else if (mode=="review") {
    result = SheetPatcher::createForReview();
  } else if (mode=="csv") {
    if (_version=="0.2") {
      result = new MergeOutputCsvDiffV0p2;
    } else if (_version=="0.4"||_version=="0.5"||_version=="") {
      result = new MergeOutputCsvDiff;
    }
  } else if (mode=="csv0") {
    result = new MergeOutputPatch;
  } else if (mode=="index") {
    result = new MergeOutputIndex;
  } else if (mode=="merge") {
    result = SheetPatcher::createForMerge();
  } else if (mode=="ops") {
    result = new MergeOutputRowOps;
  } else {
    fprintf(stderr, "Format %s?\n", mode.c_str());
  }
  return result;
}



bool Patcher::startOutput(const std::string& output, CompareFlags& flags) {
  if (needOutputBook()) return true;
  if (output=="" || output=="-") {
    flags.out = stdout;
    return true;
  }
  FILE *fout = fopen(output.c_str(),"wb");
  if (fout==NULL) {
    fprintf(stderr,"Could not open %s for writing\n", output.c_str());
    exit(1);
    return false;
  }
  flags.out = fout;
  return true;
}

bool Patcher::stopOutput(const std::string& output, CompareFlags& flags) {
  if (needOutputBook()) return true;
  if (flags.out!=stdout) {
    fclose(flags.out);
    flags.out = stdout;
  }
  return true;
}


bool Patcher::copyFile(const char *src, const char *dest) {
  FILE *fin = NULL;
  FILE *fout = NULL;
  fin = fopen(src,"rb");
  if (fin==NULL) return false;
  fout = fopen(dest,"wb");
  if (fout==NULL) {
    fclose(fin);
    return false;
  }
  char buf[32768];
  int bytes_read = 0;
  while ((bytes_read=fread(buf,1,sizeof(buf),fin))>0) {
    fwrite(buf,1,bytes_read,fout);
  }
  fclose(fout);
  fclose(fin);
  return true;
}

bool Patcher::setFlags(const CompareFlags& flags) {
  this->flags = flags;
  out = flags.out;
}

void Patcher::attachBook(coopy::store::TextBook& book) {
  patch_book = &book;
  pending  = true;
}

coopy::store::PolySheet Patcher::getSheet() {
  if (pending) {
    pending = false;
    coopy::store::TextBook *pbook = getBook();
    if (pbook) {
      if (flags.tables.size()>0) {
	attachSheet(pbook->readSheet(flags.ordered_tables[0]));
      } else {
	attachSheet(pbook->readSheetByIndex(0));
      }
    }
  }
  return patch_sheet;
}


bool Patcher::applyPool(const PoolChange& change) {
  if (flags.pool==NULL) {
    fprintf(stderr,"No storage available for pool information.\n");
    return false;
  }
  std::string poolName = change.poolName;
  std::string tableName = change.tableName;
  for (std::vector<coopy::cmp::TableField>::const_iterator it=change.pool.begin(); it!=change.pool.end(); it++) {
    flags.pool->create(poolName,(it->tableName=="")?tableName:(it->tableName),it->fieldName,it->invented);
  }
  return true;
}



void RowChange::show() {
  MergeOutputVerboseDiff diff;
  diff.changeRow(*this);
}




bool Patcher::addPoolsFromFlags(const DataSheet& sheet, bool msg) {
  if (flags.coined.size()>0) {

    SheetSchema *ss = sheet.getSchema();
    if (!ss) return false;

    string name = ss->getSheetName();

    for (int j=0; j<(int)flags.coined.size(); j++) {
      string field = flags.coined[j];
      for (int i=0; i<ss->getColumnCount(); i++) {
	string field2 = ss->getColumnInfo(i).getName();
	if (field==field2) {
	  PoolChange pc;
	  pc.poolName = name;
	  pc.tableName = name;
	  pc.pool.push_back(TableField("",field,true));
	  changePool(pc);
	}
      }
    }
  } else {
    if (flags.pool!=NULL) {
      SheetSchema *ss = sheet.getSchema();
      if (!ss) return false;
      string name = ss->getSheetName();
      for (int i=0; i<ss->getColumnCount(); i++) {
	string field2 = ss->getColumnInfo(i).getName();
	PoolColumnLink link = flags.pool->lookup(name,field2);
	if (link.isValid()) {
	  PoolChange pc;
	  pc.poolName = link.getPoolName();
	  pc.tableName = link.getTableName();
	  pc.pool.push_back(TableField("",link.getColumnName(),link.isInventor()));
	  if (msg) {
	    changePool(pc);
	  } else {
	    applyPool(pc);
	  }
	}
      }
    }
  }
  return true;
}


bool Patcher::addPoolsFromSchema(const coopy::store::DataSheet& sheet,
				 const coopy::store::NameSniffer& sniffer,
				 const std::string& sheetName,
				 bool msg) {
  const CompareFlags& flags = getFlags();
  bool addedAuto = false;
  for (int i=0; i<sheet.width(); i++) {
    string name = sniffer.suggestColumnName(i);
    PoolColumnLink pc = flags.pool->lookup(sheetName,name);
    if (!pc.isValid()) {
      ColumnType t = sniffer.suggestColumnType(i);
      if (t.autoIncrement&&!addedAuto) {
	flags.pool->create(sheetName,sheetName,name,true);
	pc = flags.pool->lookup(sheetName,name);
	addedAuto = true;
      }
      if (t.foreignKey!="") {
	flags.pool->create(t.foreignTable,sheetName,name,false);
	pc = flags.pool->lookup(sheetName,name);
      }
    }
    if (pc.isValid()) {
      PoolChange c;
      c.poolName = pc.getPoolName();
      c.tableName = pc.getTableName();
      c.pool.push_back(TableField("",pc.getColumnName(),pc.isInventor()));
      if (msg) {
	changePool(c);
      } else {
	applyPool(c);
      }
    }
  }
  return addedAuto;
}



