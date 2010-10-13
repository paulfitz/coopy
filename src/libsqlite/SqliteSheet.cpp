#include <coopy/Dbg.h>
#include <coopy/SqliteSheet.h>
#include <sqlite3.h>

using namespace coopy::store;
using namespace std;

#define DB(x) ((sqlite3 *)(x))

SqliteSheet::SqliteSheet(void *db1, const char *name) {
  implementation = db1;
  this->name = name;
  w = h = 0;

  sqlite3 *db = DB(implementation);
  if (db==NULL) return;

  sqlite3_stmt *statement = NULL;
  char *query = NULL;

  
  //////////////////////////////////////////////////////////////////
  // Check dimensions

  query = sqlite3_mprintf("SELECT COUNT(*), * FROM %Q",
			  this->name.c_str());
 
  int iresult = sqlite3_prepare_v2(db, query, -1, 
				   &statement, NULL);
  if (iresult!=SQLITE_OK) {
    const char *msg = sqlite3_errmsg(db);
    if (msg!=NULL) {
      fprintf(stderr,"Error: %s\n", msg);
    }
    sqlite3_finalize(statement);
    sqlite3_free(query);
  } 
  if (sqlite3_step(statement) == SQLITE_ROW) {
    h = sqlite3_column_int(statement,0);
    w = sqlite3_column_count(statement)-1;
  }
  //printf("Dimensions %dx%d\n", w, h);
  sqlite3_finalize(statement);
  sqlite3_free(query);


  //////////////////////////////////////////////////////////////////
  // Check ROWIDs

  query = sqlite3_mprintf("SELECT ROWID FROM %Q ORDER BY ROWID",
			  this->name.c_str());

  iresult = sqlite3_prepare_v2(db, query, -1, 
			       &statement, NULL);
  if (iresult!=SQLITE_OK) {
    const char *msg = sqlite3_errmsg(db);
    if (msg!=NULL) {
      fprintf(stderr,"Error: %s\n", msg);
    }
    sqlite3_finalize(statement);
    sqlite3_free(query);
  } 

  while (sqlite3_step(statement) == SQLITE_ROW) {
    row2sql.push_back(sqlite3_column_int(statement,0));
  }
  sqlite3_finalize(statement);
  sqlite3_free(query);


  //////////////////////////////////////////////////////////////////
  // Check column names

  query = sqlite3_mprintf("PRAGMA table_info(%Q)",
			  this->name.c_str());

  iresult = sqlite3_prepare_v2(db, query, -1, 
			       &statement, NULL);
  if (iresult!=SQLITE_OK) {
    const char *msg = sqlite3_errmsg(db);
    if (msg!=NULL) {
      fprintf(stderr,"Error: %s\n", msg);
    }
    sqlite3_finalize(statement);
    sqlite3_free(query);
  } 

  while (sqlite3_step(statement) == SQLITE_ROW) {
    col2sql.push_back((const char *)sqlite3_column_text(statement,1));
  }
  sqlite3_finalize(statement);
  sqlite3_free(query);

}

SqliteSheet::~SqliteSheet() {
}

std::string SqliteSheet::cellString(int x, int y) const {
  // starting with a COMPLETELY brain-dead implementation

  sqlite3 *db = DB(implementation);
  if (db==NULL) return "";

  sqlite3_stmt *statement = NULL;
  char *query = NULL;

  query = sqlite3_mprintf("SELECT * FROM %Q WHERE ROWID = %d", 
			  name.c_str(),
			  row2sql[y]);
 
  int iresult = sqlite3_prepare_v2(db, query, -1, 
				   &statement, NULL);
  if (iresult!=SQLITE_OK) {
    sqlite3_finalize(statement);
    sqlite3_free(query);
    return "";
  }
  string result = "";
  if (sqlite3_step(statement) == SQLITE_ROW) {
    char *txt = (char *)sqlite3_column_text(statement,x);
    //printf("Got field %s\n", txt);
    result = txt;
  }
  sqlite3_finalize(statement);
  sqlite3_free(query);
  
  return result;
}

bool SqliteSheet::cellString(int x, int y, const std::string& str) {
  // starting with a COMPLETELY brain-dead implementation

  sqlite3 *db = DB(implementation);
  if (db==NULL) return false;

  sqlite3_stmt *statement = NULL;
  char *query = NULL;

  query = sqlite3_mprintf("UPDATE %Q SET %Q = %Q WHERE ROWID = %d", 
			  name.c_str(),
			  col2sql[x].c_str(),
			  str.c_str(),
			  row2sql[y]);

  int iresult = sqlite3_exec(db, query, NULL, NULL, NULL);
  if (iresult!=SQLITE_OK) {
    const char *msg = sqlite3_errmsg(db);
    if (msg!=NULL) {
      fprintf(stderr,"Error: %s\n", msg);
    }
    sqlite3_free(query);
    return false;
  }
  sqlite3_free(query);
  
  return true;
}


ColumnRef SqliteSheet::moveColumn(const ColumnRef& src, 
				    const ColumnRef& base) {
  return ColumnRef();
}


bool SqliteSheet::deleteColumn(const ColumnRef& column) {
  int deadbeat_index = column.getIndex();
  string deadbeat = col2sql[deadbeat_index];
  dbg_printf("sqlite: eliminating column %s\n", deadbeat.c_str());

  sqlite3 *db = DB(implementation);
  if (db==NULL) return false;

  sqlite3_stmt *statement = NULL;
  char *query = NULL;

  //////////////////////////////////////////////////////////////////
  // Check schema

  query = sqlite3_mprintf("SELECT sql FROM SQLITE_MASTER WHERE name = %Q",
			  this->name.c_str());
 
  int iresult = sqlite3_prepare_v2(db, query, -1, 
				   &statement, NULL);
  if (iresult!=SQLITE_OK) {
    const char *msg = sqlite3_errmsg(db);
    if (msg!=NULL) {
      fprintf(stderr,"Error: %s\n", msg);
    }
    sqlite3_finalize(statement);
    sqlite3_free(query);
  } 
  string sql;
  if (sqlite3_step(statement) == SQLITE_ROW) {
    sql = (const char *)sqlite3_column_text(statement,0);
  }
  sqlite3_finalize(statement);
  sqlite3_free(query);

  //////////////////////////////////////////////////////////////////
  // Modify schema

  dbg_printf("Original table SQL is %s\n", sql.c_str());

  bool double_quote = false;
  bool single_quote = false;
  string token = "";
  string key = "";
  int key_state = 1;
  string output = "";
  string mod = "";
  int offset = 0;
  int nesting = 0;
  for (int i=0; i<(int)sql.length(); i++) {
    char ch = sql[i];
    if (double_quote||single_quote) {
      if (double_quote) {
	if (ch=='\"') double_quote = false;
      }
      if (single_quote) {
	if (ch=='\'') single_quote = false;
      }
      token += ch;
      continue;
    }
    bool brk = false;
    if (ch=='(') {
      nesting++;
      if (nesting==1) {
	brk = true;
      }
    } else if (ch==')') {
      nesting--;
      if (nesting==0) {
	brk = true;
      }
    }
    if (ch==',') {
      brk = true;
      if (nesting==1) {
      }
    }
    if (key_state==0) {
      if ((ch==' '||brk)&&token!="") {
	key = token;
	if (key[0]==' ') {
	  key = key.substr(1,key.length());
	}
	if (key[0]=='\''||key[0]=='\"') {
	  key = key.substr(1,key.length()-2);
	}
	key_state = 1;
      }
    }
    if (brk) {
      output += "[";
      output += key;
      output += ":";
      output += token;
      output += "]";
      output += ch;
      if (key!=deadbeat) {
	mod += token;
	mod += ch;
      } else {
	if (ch!=',') {
	  mod += ch;
	}
      }
      token = "";
      key = "";
      key_state = 0;
    } else {
      token += ch;
    }
  }

  dbg_printf("parsed SQL is %s\n", output.c_str());
  dbg_printf("modified SQL is %s\n", mod.c_str());

  //////////////////////////////////////////////////////////////////
  // Apply schema

  string new_column_list = "";
  for (int i=0; i<w; i++) {
    if (i!=deadbeat_index) {
      if (i>0) {
	new_column_list += ',';
      }
      string add = col2sql[i].c_str();
      if (add.find('\"')!=string::npos) {
	printf("Quoting of sql column names not done yet\n");
	exit(1);
      }
      new_column_list += "\"";
      new_column_list += add;
      new_column_list += "\"";
    }
  }

  query = sqlite3_mprintf("BEGIN TRANSACTION; \
CREATE TEMPORARY TABLE __coopy_backup(%s); \
INSERT INTO __coopy_backup(%s) SELECT %s FROM %Q; \
DROP TABLE %Q; \
%s; \
INSERT INTO %Q SELECT * FROM __coopy_backup; \
DROP TABLE __coopy_backup; \
COMMIT; \
", 
			  new_column_list.c_str(),
			  new_column_list.c_str(),
			  new_column_list.c_str(),
			  name.c_str(),
			  name.c_str(),
			  mod.c_str(),
			  name.c_str());

  dbg_printf("Operation %s\n", query);

  iresult = sqlite3_exec(db, query, NULL, NULL, NULL);
  if (iresult!=SQLITE_OK) {
    const char *msg = sqlite3_errmsg(db);
    if (msg!=NULL) {
      fprintf(stderr,"Error: %s\n", msg);
    }
    sqlite3_free(query);
    return false;
  }

  sqlite3_free(query);

  return false;
}

ColumnRef SqliteSheet::insertColumn(const ColumnRef& base) {
  return ColumnRef(-1);
}

RowRef SqliteSheet::insertRow(const RowRef& base) {
  return RowRef(-1);  
}

bool SqliteSheet::deleteRow(const RowRef& src) {
  return false;
}
