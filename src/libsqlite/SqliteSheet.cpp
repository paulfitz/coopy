#include <coopy/Dbg.h>
#include <coopy/SqliteSheet.h>
#include <sqlite3.h>

using namespace coopy::store;
using namespace std;

namespace coopy {
  namespace store {
    class SqliteSchema;
  }
}

class coopy::store::SqliteSchema {
public:
  string preamble;
  vector<string> parts;
  sqlite3 *db;
  string table_name;

  SqliteSchema() {
    db = NULL;
  }

  string fetch(sqlite3 *db, const char *table);

  void parse(const char *str);

  string toString() const;

  bool apply(const vector<string>& names, const string& novel = "");
};


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
      fprintf(stderr,"Query was: %s\n", query);
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
      fprintf(stderr,"Query was: %s\n", query);
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
      fprintf(stderr,"Query was: %s\n", query);
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
    if (txt!=NULL) {
      result = txt;
    } else { 
      result = "";
    }
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
      fprintf(stderr,"Query was: %s\n", query);
    }
    sqlite3_free(query);
    return false;
  }
  sqlite3_free(query);
  
  return true;
}


ColumnRef SqliteSheet::moveColumn(const ColumnRef& src, 
				    const ColumnRef& base) {
  int src_index = src.getIndex();
  int base_index = base.getIndex();

  sqlite3 *db = DB(implementation);
  if (db==NULL) return ColumnRef();

  SqliteSchema schema;
  string sql_pre = schema.fetch(db,name.c_str());
  schema.parse(sql_pre.c_str());
  string cache_sql = schema.parts[src_index];
  string cache_name = col2sql[src_index];
  schema.parts.erase(schema.parts.begin()+src_index);
  col2sql.erase(col2sql.begin()+src_index);
  if (base_index>src_index) {
    base_index--;
  }
  if (base_index==-1) {
    schema.parts.push_back(cache_sql);
    col2sql.push_back(cache_name);
  } else {
    schema.parts.insert(schema.parts.begin()+base_index,cache_sql);
    col2sql.insert(col2sql.begin()+base_index,cache_name);
  }
  
  return schema.apply(col2sql)?ColumnRef(base_index):ColumnRef();
}


bool SqliteSheet::deleteColumn(const ColumnRef& column) {
  int deadbeat_index = column.getIndex();

  sqlite3 *db = DB(implementation);
  if (db==NULL) return false;

  SqliteSchema schema;
  string sql_pre = schema.fetch(db,name.c_str());
  schema.parse(sql_pre.c_str());
  schema.parts.erase(schema.parts.begin()+deadbeat_index);
  col2sql.erase(col2sql.begin()+deadbeat_index);
  w--;
  
  return schema.apply(col2sql);
}

ColumnRef SqliteSheet::insertColumn(const ColumnRef& base) {
  // we will need to pass in type hints in future, for better merges.
  // also, hint for column name.

  sqlite3 *db = DB(implementation);
  if (db==NULL) return ColumnRef();

  int index = base.getIndex();

  string suggest = "ins";
  bool found = false;
  for (int i=0; i<(int)col2sql.size(); i++) {
    string n = col2sql[i];
    if (n.substr(0,3)=="ins") {
      if (n.length()>=suggest.length()) {
	suggest = n;
	found = true;
      }
    }
  }
  if (found) {
    suggest += "_";
  }
  string col_name = suggest;

  if (index==-1) {
    char *query = sqlite3_mprintf("ALTER TABLE %Q ADD COLUMN %Q",
				  name.c_str(), col_name.c_str());

    int iresult = sqlite3_exec(db, query, NULL, NULL, NULL);
    if (iresult!=SQLITE_OK) {
      const char *msg = sqlite3_errmsg(db);
      if (msg!=NULL) {
	fprintf(stderr,"Error: %s\n", msg);
	fprintf(stderr,"Query was: %s\n", query);
      }
      sqlite3_free(query);
      return ColumnRef();
    }
    sqlite3_free(query);
    col2sql.push_back(col_name);
    w++;
    return ColumnRef(w-1);
  }

  string col_sql = suggest;

  SqliteSchema schema;
  string sql_pre = schema.fetch(db,name.c_str());
  schema.parse(sql_pre.c_str());
  if (index<0) {
    schema.parts.push_back(col_sql);
    col2sql.push_back(col_name);
    index = w;
  } else {
    schema.parts.insert(schema.parts.begin()+index,col_sql);
    col2sql.insert(col2sql.begin()+index,col_name);
  }
  if (schema.apply(col2sql,col_name)) {
    w++;
    return ColumnRef(index);
  }
  return ColumnRef();
}

RowRef SqliteSheet::insertRow(const RowRef& base) {
  // Relies on having default values, to insert "blank row".
  // This is suboptimal.

  sqlite3 *db = DB(implementation);
  if (db==NULL) return false;
  int index = base.getIndex();
  
  if (index!=-1) {
    fprintf(stderr,"*** WARNING: Row insertion order ignored for Sqlite\n");
  }

  char *query = sqlite3_mprintf("INSERT INTO %Q DEFAULT VALUES",
				name.c_str());

  int iresult = sqlite3_exec(db, query, NULL, NULL, NULL);
  if (iresult!=SQLITE_OK) {
    const char *msg = sqlite3_errmsg(db);
    if (msg!=NULL) {
      fprintf(stderr,"Error: %s\n", msg);
      fprintf(stderr,"Query was: %s\n", query);
    }
    sqlite3_free(query);
    return false;
  }
  sqlite3_free(query);

  int rid = (int)sqlite3_last_insert_rowid(db);

  // inconsistent ordering
  row2sql.push_back(rid);
  h++;
  return RowRef(h-1);
}

bool SqliteSheet::deleteRow(const RowRef& src) {
  sqlite3 *db = DB(implementation);
  if (db==NULL) return false;

  int index = src.getIndex();
  if (index==-1) return false;
  int rid = row2sql[index];
  char *query = sqlite3_mprintf("DELETE FROM %Q WHERE ROWID=%d",
				name.c_str(), rid);

  int iresult = sqlite3_exec(db, query, NULL, NULL, NULL);
  if (iresult!=SQLITE_OK) {
    const char *msg = sqlite3_errmsg(db);
    if (msg!=NULL) {
      fprintf(stderr,"Error: %s\n", msg);
      fprintf(stderr,"Query was: %s\n", query);
    }
    sqlite3_free(query);
    return false;
  }
  sqlite3_free(query);
  row2sql.erase(row2sql.begin()+index);
  h--;
  return true;
}


void SqliteSchema::parse(const char *str) {
  string sql = str;
  preamble = "";
  parts.clear();

  bool double_quote = false;
  bool single_quote = false;
  string token = "";
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
    if (brk) {
      if (token[0]==' ') {
	token = token.substr(1,token.length());
      }
      if (preamble=="") {
	preamble = token;
      } else {
	parts.push_back(token);
      }
      token = "";
    } else {
      token += ch;
    }
  }

  dbg_printf("preamble %s\n", preamble.c_str());
  for (int i=0; i<(int)parts.size(); i++) {
    dbg_printf(" part %s\n", parts[i].c_str());
  }
}


string SqliteSchema::toString() const {
  string result = preamble;
  result += "(";
  for (int i=0; i<(int)parts.size(); i++) {
    if (i>0) {
      result += ", ";
    }
    result += parts[i];
  }
  result += ")";
  dbg_printf("modified SQL is %s\n", result.c_str());
  return result;
}


string SqliteSchema::fetch(sqlite3 *db, const char *table) {
  this->db = db;
  this->table_name = table;
  string sql = "";
  sqlite3_stmt *statement = NULL;
  char *query = NULL;

  query = sqlite3_mprintf("SELECT sql FROM SQLITE_MASTER WHERE name = %Q",
			  table);
 
  int iresult = sqlite3_prepare_v2(db, query, -1, 
				   &statement, NULL);
  if (iresult!=SQLITE_OK) {
    const char *msg = sqlite3_errmsg(db);
    if (msg!=NULL) {
      fprintf(stderr,"Error: %s\n", msg);
      fprintf(stderr,"Query was: %s\n", query);
    }
    sqlite3_finalize(statement);
    sqlite3_free(query);
    return "";
  } 
  if (sqlite3_step(statement) == SQLITE_ROW) {
    sql = (const char *)sqlite3_column_text(statement,0);
  }
  sqlite3_finalize(statement);
  sqlite3_free(query);
  return sql;
}


bool SqliteSchema::apply(const vector<string>& names, const string& novel) {
  string sql_mod = toString();

  //////////////////////////////////////////////////////////////////
  // Apply schema

  string new_column_list = "";
  string ins_column_list = "";
  for (int i=0; i<(int)names.size(); i++) {
    string base = names[i].c_str();
    string add = base;
    if (add.find('\"')!=string::npos) {
      printf("Quoting of sql column names not done yet\n");
      exit(1);
    }
    add = string("\"") + add + "\"";
    if (i>0) {
      new_column_list += ',';
    }
    new_column_list += add;
    if (base!=novel) {
      if (ins_column_list!="") {
	ins_column_list += ',';
      }
      ins_column_list += add;
    }
  }

  char *query = sqlite3_mprintf("BEGIN TRANSACTION; \
CREATE TEMPORARY TABLE __coopy_backup(%s);	    \
INSERT INTO __coopy_backup (%s) SELECT %s FROM %Q;   \
DROP TABLE %Q;					    \
%s;						    \
INSERT INTO %Q (%s) SELECT * FROM __coopy_backup;   \
DROP TABLE __coopy_backup;			    \
COMMIT;						    \
", 
				ins_column_list.c_str(),
				ins_column_list.c_str(),
				ins_column_list.c_str(),
				table_name.c_str(),
				table_name.c_str(),
				sql_mod.c_str(),
				table_name.c_str(),
				ins_column_list.c_str());

  dbg_printf("Operation %s\n", query);

  int iresult = sqlite3_exec(db, query, NULL, NULL, NULL);
  if (iresult!=SQLITE_OK) {
    const char *msg = sqlite3_errmsg(db);
    if (msg!=NULL) {
      fprintf(stderr,"Error: %s\n", msg);
      fprintf(stderr,"Query was: %s\n", query);
    }
    sqlite3_free(query);
    return false;
  }

  sqlite3_free(query);
  return true;
}
