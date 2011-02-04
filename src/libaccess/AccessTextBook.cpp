#include <coopy/AccessTextBook.h>
#include <coopy/AccessSheet.h>

#include "mdbtools.h"
#include <stdio.h>

#define WANT_VECTOR2STRING
#include <coopy/Stringer.h>

#include <algorithm>

using namespace std;
using namespace coopy::store;

int AccessTextBook::uses = 0;

#define DB(x) ((MdbHandle *)(x))

AccessTextBook::AccessTextBook() {
  implementation = NULL;
  uses++;
  if (uses==1) {
    mdb_init();
  }
}

AccessTextBook::~AccessTextBook() {
  clear();
  uses--;
  if (uses==0) {
    mdb_exit();
  }
}

void AccessTextBook::clear() {
  if (implementation!=NULL) {
    mdb_close(DB(implementation));
    implementation = NULL;
  }
}

bool AccessTextBook::read(const char *fname) {
  clear();
  MdbHandle *mdb = mdb_open(fname,MDB_NOFLAGS);
  if (!mdb) {
    fprintf(stderr,"Failed to open database %s\n", fname);
    return false;
  }
  implementation = (void *)mdb;
  if (!mdb_read_catalog(mdb,MDB_ANY)) {
    fprintf(stderr,"Failed to read as Access file: %s\n",fname);
    clear();
    return false;
  }

  names = getNamesImpl();
  return true;
}

bool AccessTextBook::open(const Property& config) {
  clear();
  if (!config.check("file")) {
    fprintf(stderr,"file parameter needed\n");
    return false;
  }
  if (!read(config.get("file").asString().c_str())) {
    fprintf(stderr,"failed to read %s\n", config.get("file").asString().c_str());
    return false;
  }
  if (!config.check("table")) {
    dbg_printf("Loaded access workbook\n");
    return true;
  }
  names.clear();
  names.push_back(config.get("table").asString().c_str());
  dbg_printf("Loaded access workbook, table: %s\n",
	     vector2string(names).c_str());
  return true;
}


vector<string> AccessTextBook::getNamesImpl() {
  std::vector<std::string> tables;
  MdbHandle *mdb = DB(implementation);
  for (int i=0; i<mdb->num_catalog; i++) {
    MdbCatalogEntry *entry = (MdbCatalogEntry *)g_ptr_array_index(mdb->catalog, i);
    if (entry->object_type!=MDB_TABLE) continue;
    if (mdb_is_system_table(entry)) continue;
    //printf("Found table %s ...\n", entry->object_name);
    tables.push_back(entry->object_name);
  }
  return tables;
}

PolySheet AccessTextBook::readSheet(const string& name) {
  getNames();
  if (find(names.begin(),names.end(),name)==names.end()) {
    return PolySheet();
  }
  AccessSheet *sheet = new AccessSheet(implementation,name.c_str());
  if (sheet!=NULL) sheet->connect();
  return PolySheet(sheet,true);
}

bool AccessTextBook::addSheet(const SheetSchema& schema) {
  return false;
}

