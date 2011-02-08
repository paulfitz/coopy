#include "WideSheet.h"

#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"

#include <string>

#include <coopy/PolyBook.h>
#include <coopy/MergeOutputTdiff.h>
#include <coopy/BookCompare.h>

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

WideSheet::~WideSheet() {
  if (db!=NULL) {
    sqlite3_close(db);
    db = NULL;
  }
}

void WideSheet::connect(bool create) {
  if (init) {
    return;
  }
  init = true;

  const char *filename = "widesheet.sqlite";
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
    return;
  }

  string create_main_table = "CREATE TABLE IF NOT EXISTS links (\n\
	filename TEXT,\n\
	key TEXT);";

  result = sqlite3_exec(db, create_main_table.c_str(), NULL, NULL, NULL);
  if (result!=SQLITE_OK) {
    const char *msg = sqlite3_errmsg(db);
    if (msg!=NULL) {
      fprintf(stderr,"Error in %s: %s\n", filename, msg);
    }
    sqlite3_close(db);
    fprintf(stderr,"Failed to set up database tables\n");
    exit(1);
  }

  string create_prop_table = "CREATE TABLE IF NOT EXISTS props (\n\
	key TEXT PRIMARY KEY,\n\
	val TEXT);";

  result = sqlite3_exec(db, create_prop_table.c_str(), NULL, NULL, NULL);
  if (result!=SQLITE_OK) {
    const char *msg = sqlite3_errmsg(db);
    if (msg!=NULL) {
      fprintf(stderr,"Error in %s: %s\n", filename, msg);
    }
    sqlite3_close(db);
    fprintf(stderr,"Failed to set up database tables\n");
    exit(1);
  }
}


bool WideSheet::setProperty(const char *key, const char *val) {
  char *query = NULL;
  query = sqlite3_mprintf("INSERT OR REPLACE INTO props (key,val) VALUES(%Q,%Q)",
			  key, val);
  sql_enact(db,query);
  sqlite3_free(query);
}


std::string WideSheet::getProperty(const char *key) {
  char *query = NULL;
  string out;
  query = sqlite3_mprintf("SELECT val FROM props WHERE key = %Q",
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


std::string WideSheet::getPropertyWithDefault(const char *key) {
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


bool WideSheet::exportSheet() {
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
  printf("%s -> offer -> %s\n", local.c_str(), remote.c_str());
  return true;
}

bool WideSheet::acceptSheet() {
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
  printf("%s -> accepted -> %s\n", remote.c_str(), pivot.c_str());
  return true;
}


bool WideSheet::diffSheet() {
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

