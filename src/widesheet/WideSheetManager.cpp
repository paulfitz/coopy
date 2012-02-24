#include "WideSheetManager.h"

#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"

#include <string>

#include <coopy/PolyBook.h>
#include <coopy/MergeOutputTdiff.h>
#include <coopy/BookCompare.h>
#include <coopy/SheetPatcher.h>

#ifndef WIN32
#include <unistd.h>
#else
#include <io.h>
#define access(f,a) _access(f,a)
#endif
#ifndef F_OK
#define F_OK 0
#endif

using namespace std;
using namespace coopy::store;
using namespace coopy::cmp;

static bool sql_enact(sqlite3 *db, const char *cmd) {
    int result = sqlite3_exec(db, cmd, NULL, NULL, NULL);
    if (result!=SQLITE_OK) {
        const char *msg = sqlite3_errmsg(db);
        if (msg!=NULL) {
            fprintf(stderr,"Database error: %s\n", msg);
        }
        sqlite3_close(db);
        fprintf(stderr,"Failed to set up database tables\n");
        exit(1);
    }
    return true;
}

WideSheetManager::~WideSheetManager() {
  disconnect();
}

bool WideSheetManager::connect(bool create, const char *fname) {
  disconnect();

  const char *filename = (fname!=NULL)?fname:"widesheet.sqlite";
  int result = sqlite3_open_v2(filename,
			       &db,
			       SQLITE_OPEN_READWRITE|
			       (create?SQLITE_OPEN_CREATE:0)|
			       SQLITE_OPEN_NOMUTEX,
			       NULL);
  if (result!=SQLITE_OK) {
    fprintf(stderr,"Failed to open database %s\n", filename);
    if (db!=NULL) {
      sqlite3_close(db);
    }
    db = NULL;
    return false;
  }

  string create_main_table = "CREATE TABLE IF NOT EXISTS widesheet_links (\n\
	filename TEXT,\n\
	key TEXT);";

  result = sqlite3_exec(db, create_main_table.c_str(), NULL, NULL, NULL);
  if (result!=SQLITE_OK) {
    const char *msg = sqlite3_errmsg(db);
    if (msg!=NULL) {
      fprintf(stderr,"Error in %s: %s\n", filename, msg);
    }
    sqlite3_close(db);
    db = NULL;
    fprintf(stderr,"Failed to set up database tables\n");
    return false;
  }

  string create_prop_table = "CREATE TABLE IF NOT EXISTS widesheet_props (\n\
	key TEXT PRIMARY KEY,\n\
	val TEXT);";

  result = sqlite3_exec(db, create_prop_table.c_str(), NULL, NULL, NULL);
  if (result!=SQLITE_OK) {
    const char *msg = sqlite3_errmsg(db);
    if (msg!=NULL) {
      fprintf(stderr,"Error in %s: %s\n", filename, msg);
    }
    sqlite3_close(db);
    db = NULL;
    fprintf(stderr,"Failed to set up database tables\n");
    return false;
  }
  return true;
}


bool WideSheetManager::disconnect() {
  if (db!=NULL) {
    sqlite3_close(db);
    db = NULL;
  }
  return true;
}

bool WideSheetManager::setProperty(const char *key, const char *val) {
  char *query = NULL;
  query = sqlite3_mprintf("INSERT OR REPLACE INTO widesheet_props (key,val) VALUES(%Q,%Q)",
			  key, val);
  sql_enact(db,query);
  sqlite3_free(query);
  return true;
}


std::string WideSheetManager::getProperty(const char *key) {
  char *query = NULL;
  string out;
  query = sqlite3_mprintf("SELECT val FROM widesheet_props WHERE key = %Q",
			  key);
  
  sqlite3_stmt *statement = NULL;
  int result = sqlite3_prepare_v2(db,query,-1,&statement,NULL);
  if (result!=SQLITE_OK) {
    printf("Error in query\n");
  }
  if (result == SQLITE_OK && sqlite3_step(statement) == SQLITE_ROW) {
    out = (char *)sqlite3_column_text(statement,0);
  }
  sqlite3_finalize(statement);
  sqlite3_free(query);
  return out;
}


std::string WideSheetManager::getPropertyWithDefault(const char *key) {
  string result = getProperty(key);
  if (result!="") return result;
  string str(key);
  if (str=="remote") {
    return "remote.csvs";
  }
  if (str=="pivot") {
    return "pivot.csvs";
  }
  return "";
}


bool WideSheetManager::exportSheet() {
  string local = getProperty("local");
  if (local=="") {
    fprintf(stderr,"No local file.\n");
    return false;
  }
  string remote = getPropertyWithDefault("remote");

  PolyBook src;
  if (!src.read(local.c_str())) {
    fprintf(stderr,"Failed to read %s\n", local.c_str());
    return false;
  } 
  if (!src.write(remote.c_str())) {
    fprintf(stderr,"Failed to write %s\n", remote.c_str());
    return false;
  } 
  printf("%s -> %s\n", local.c_str(), remote.c_str());
  return true;
}

bool WideSheetManager::acceptSheet() {
  string pivot = getPropertyWithDefault("pivot");
  string remote = getPropertyWithDefault("remote");

  // THIS IS JUST A COPY OPERATION!
  // Massively inefficient right now, replace with a file copy.

  PolyBook src;
  if (!src.read(remote.c_str())) {
    fprintf(stderr,"Failed to read %s\n", remote.c_str());
    return false;
  } 
  if (!src.write(pivot.c_str())) {
    fprintf(stderr,"Failed to write %s\n", pivot.c_str());
    return false;
  } 
  printf("%s -> %s\n", remote.c_str(), pivot.c_str());
  return true;
}


bool WideSheetManager::diffSheet() {
  string local = getProperty("local");
  if (local=="") {
    fprintf(stderr,"No local file.\n");
    return false;
  }
  string pivot = getPropertyWithDefault("pivot");
  string remote = getPropertyWithDefault("remote");

  int result = access(pivot.c_str(),F_OK);
  if (result!=0) {
    fprintf(stderr,"Pivot not present, please offer/accept first.\n");
    return false;
  }

  MergeOutputTdiff diff;
  CompareFlags flags;
  flags.out = stdout;
  PolyBook p,l,r;
  if (!p.read(pivot.c_str())) {
    fprintf(stderr,"Failed to read %s\n", pivot.c_str());
    return false;
  } 
  if (!l.read(local.c_str())) {
    fprintf(stderr,"Failed to read %s\n", local.c_str());
    return false;
  } 
  if (!r.read(remote.c_str())) {
    fprintf(stderr,"Failed to read %s\n", remote.c_str());
    return false;
  } 
  BookCompare cmp;
  cmp.compare(p,l,r,diff,flags);

  return true;
}



bool WideSheetManager::setFile(const char *key, const char *val) {
  char *query = NULL;

  query = sqlite3_mprintf("DELETE FROM widesheet_links WHERE key = %Q",
			  key);
  sql_enact(db,query);
  sqlite3_free(query);
  query = NULL;

  query = sqlite3_mprintf("INSERT OR REPLACE INTO widesheet_links (filename,key) VALUES(%Q,%Q)",
			  val, key);
  sql_enact(db,query);
  sqlite3_free(query);
  return true;
}

std::string WideSheetManager::getFile(const char *key) {
  char *query = NULL;
  string out;
  query = sqlite3_mprintf("SELECT filename FROM widesheet_links WHERE key = %Q",
			  key);
  
  sqlite3_stmt *statement = NULL;
  int result = sqlite3_prepare_v2(db,query,-1,&statement,NULL);
  if (result!=SQLITE_OK) {
    printf("Error in query\n");
  }
  if (result == SQLITE_OK && sqlite3_step(statement) == SQLITE_ROW) {
    out = (char *)sqlite3_column_text(statement,0);
  }
  sqlite3_finalize(statement);
  sqlite3_free(query);
  return out;
}

bool WideSheetManager::setDirectory(const char *dir, const char *sep) {
  this->dir = dir;
  this->sep = sep;
}


bool WideSheetManager::exportSheet(const char *key, bool reverse) {
  //printf("Export key %s direction %s\n", key, reverse?"reversed":"forward");
  string remote = getFile(key);
  if (remote=="") {
    return false;
  }
  string local = dir + sep + key + ".csvs";

  if (reverse) {
    string tmp = local;
    local = remote;
    remote = tmp;
  }

  PolyBook src;
  if (!src.read(local.c_str())) {
    fprintf(stderr,"Failed to read %s\n", local.c_str());
    return false;
  } 
  if (!src.write(remote.c_str())) {
    fprintf(stderr,"Failed to write %s\n", remote.c_str());
    return false;
  } 
  printf("%s -> %s\n", local.c_str(), remote.c_str());
  return true;
}

static void start_output2(string output, CompareFlags& flags) {
  if (output=="" || output=="-") {
    flags.out = stdout;
    return;
  }
  FILE *fout = fopen(output.c_str(),"wb");
  if (fout==NULL) {
    fprintf(stderr,"Could not open %s for writing\n", output.c_str());
    exit(1);
  }
  flags.out = fout;
}

static void stop_output2(string output, CompareFlags& flags) {
  if (flags.out!=stdout) {
    fclose(flags.out);
    flags.out = stdout;
  }
}

bool WideSheetManager::mergeToLocal(const char *localName,
				    const char *remoteName,
				    const char *pivotName,
				    const char *logName) {
  printf("merging...\n");
  PolyBook _pivot;
  PolyBook *pivot;
  PolyBook _local;
  PolyBook *local = &_local;
  PolyBook _remote;
  PolyBook *remote = &_remote;
  
  if (!_local.read(localName)) {
    fprintf(stderr,"Failed to read %s\n", localName);
    return false;
  }
  if (!_remote.read(remoteName)) {
    fprintf(stderr,"Failed to read %s\n", remoteName);
    return 1;
  }
  if (pivotName!=NULL) {
    if (!_pivot.read(pivotName)) {
      fprintf(stderr,"Failed to read %s\n", pivotName);
      return false;
    }
    pivot = &_pivot;
  } else {
    pivot = &_local;
  }

  SheetPatcher *diff = SheetPatcher::createForMerge();
  COOPY_ASSERT(diff);
  MergeOutputTdiff nested_diff;
  diff->attachBook(*local);
  diff->showSummary(&nested_diff,false);
  BookCompare cmp;
  CompareFlags flags;
  string output = logName;
  start_output2(output,flags);
  nested_diff.setFlags(flags);
  int r = cmp.compare(*pivot,*local,*remote,*diff,flags);
  int cc = diff->getChangeCount();
  stop_output2(output,flags);
  delete diff;  diff = NULL;

  if (r<0) {
    return false;
  }
  if (cc>0) {
    if (!local->inplace()) {
      if (!local->write(localName)) {
	fprintf(stderr,"Failed to write %s\n", localName);
	return false;
      }
    }
  }
  return true;
}
