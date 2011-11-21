#include <coopy/Patcher.h>
#include <coopy/MergeOutputSqlDiff.h>
#include <coopy/MergeOutputHumanDiff.h>
#include <coopy/MergeOutputVerboseDiff.h>
#include <coopy/MergeOutputCsvDiff.h>
#include <coopy/MergeOutputTdiff.h>
#include <coopy/MergeOutputPatch.h>
#include <coopy/MergeOutputIndex.h>
#include <coopy/MergeOutputAccum.h>
#include <coopy/MergeOutputRowOps.h>
#include <coopy/SheetPatcher.h>

#include <algorithm>

using namespace std;
using namespace coopy::cmp;

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
  } else if (mode=="accum") {
    result = new MergeOutputAccum;
  } else if (mode=="ops") {
    result = new MergeOutputRowOps;
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
    dbg_printf("NO SINK AVAILABLE FOR POOL CHANGE\n");
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


