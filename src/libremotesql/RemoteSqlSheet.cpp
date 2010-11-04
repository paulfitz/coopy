#include <coopy/RemoteSqlSheet.h>
#include <coopy/RemoteSqlTextBook.h>
#include <coopy/Dbg.h>

#include <sqlxx.h>
#include <strutilsxx.h>

#include <algorithm>

using namespace std;
using namespace coopy::store;
using namespace sqlxx;

#define SQL_CONNECTION(x) (*((CSQL*)((x)->getSqlInterface())))

RemoteSqlSheet::RemoteSqlSheet(RemoteSqlTextBook *owner, const char *name) {
  implementation = NULL;
  w = h = 0;
  book = owner;
  this->name = name;
  schema = new RemoteSqlSheetSchema;
  COOPY_MEMORY(schema);
  schema->sheet = this;

  //////////////////////////////////////////////////////////////////
  // Check columns
  
  CSQL& SQL = SQL_CONNECTION(book);

  {
    string query = string("SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME=")+quote(name);
    //printf("Query is %s\n", query.c_str());
    CSQLResult *result = SQL.openQuery(query);
    if (result==NULL) return;
    while (result->fetch()) {
      //printf("Got %s\n", result->get(0).c_str());
      col2sql.push_back(result->get(0));
      w++;
    }
    SQL.closeQuery(result);
  }

  /*
  {
    string query = string("SELECT * FROM ")+name;
    //printf("Query is %s\n", query.c_str());
    CSQLResult *result = SQL.openQuery(query);
    SQL.closeQuery(result);
  }
  */

  //////////////////////////////////////////////////////////////////
  // Check rows

  {
    string query = string("SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = ") + quote(book->getDatabaseName()) + " AND TABLE_NAME = " + quote(name) + " AND COLUMN_KEY = 'PRI'";
    //printf("Query is %s\n", query.c_str());
    CSQLResult *result = SQL.openQuery(query);
    while (result->fetch()) {
      //printf("Got %s\n", result->get(0).c_str());
      keys.push_back(result->get(0));
      //h++;
    }
  }

  if (keys.size()==0) {
    keys = col2sql;
  }

  string key_list = "";
  for (vector<string>::iterator it=keys.begin(); it!=keys.end(); it++) {
    int at = find(col2sql.begin(),col2sql.end(),*it)-col2sql.begin();
    key_cols.push_back(at);
    if (key_list!="") key_list += ", ";
    key_list += *it;
  }
  int k = (int)keys.size();

  cache.resize(w,0,"");

  {
    string query = string("SELECT * FROM ") + name + " ORDER BY " + key_list;
    //printf("Query is %s\n", query.c_str());
    CSQLResult *result = SQL.openQuery(query);
    while (result->fetch()) {
      vector<string> accum;
      cache.reheight(h+1);
      for (int i=0; i<w; i++) {
	//printf("Got %d,%d: ", i, h);
	//printf("Got %d,%d %s\n", i, h, result->get(i).c_str());
	cache.cell(i,h) = result->get(i);
      }
      for (int i=0; i<k; i++) {
	//printf("Got %s\n", result->get(i).c_str());
	accum.push_back(result->get(key_cols[i]));
      }
      row2sql.push_back(accum);
      h++;
    }
    SQL.closeQuery(result);
  }
}

RemoteSqlSheet::~RemoteSqlSheet() {
  if (schema!=NULL) delete schema;
}

SheetSchema *RemoteSqlSheet::getSchema() const {
  return schema;
}


std::string RemoteSqlSheet::cellString(int x, int y) const {
  const string *c = cache.pcell_const(x,y);
  if (c!=NULL) {
    return *c;
  }

  // starting with a COMPLETELY brain-dead implementation

  CSQL& SQL = SQL_CONNECTION(book);

  string query = string("SELECT * FROM ") + name + " WHERE ";
  const vector<string>& idx = row2sql[y];
  for (int i=0; i<(int)keys.size(); i++) {
    if (i!=0) {
      query += " AND ";
    }
    query += keys[i];
    query += " = ";
    query += quote(idx[i]);
  }
  //printf("<%d/%d> Query %s\n", x, y, query.c_str());
  string out;
  CSQLResult *result = SQL.openQuery(query);
  if (result==NULL) return "";
  if (result->fetch()) {
    out = result->get(x);
  }
  SQL.closeQuery(result);
  return out;
}

bool RemoteSqlSheet::cellString(int x, int y, const std::string& str) {
  // starting with a COMPLETELY brain-dead implementation

  cache.cell(x,y) = str;

  CSQL& SQL = SQL_CONNECTION(book);

  string query = string("UPDATE ") + name + " SET " +
    col2sql[x] + " = " +
    quote(str) +
    " WHERE ";
  //printf("Making query %s\n", query.c_str());
  const vector<string>& idx = row2sql[y];
  bool index_change = false;
  for (int i=0; i<(int)keys.size(); i++) {
    //printf("Making query %s\n", query.c_str());
    if (key_cols[i]==x) {
      index_change = true;
    }
    if (i!=0) {
      query += " AND ";
    }
    query += keys[i];
    query += " = ";
    query += quote(idx[i]);
  }
  if (index_change) {
    printf("Mysql code can't deal with change in primary key yet\n");
    exit(1);
  }
  //printf("<%d/%d> Query %s\n", x, y, query.c_str());
  SQL.execQuery(query);
  return true;
}


ColumnRef RemoteSqlSheet::moveColumn(const ColumnRef& src, 
				     const ColumnRef& base) {
  return ColumnRef();
}

bool RemoteSqlSheet::deleteColumn(const ColumnRef& column) {
  return false;
}

ColumnRef RemoteSqlSheet::insertColumn(const ColumnRef& base) {
  return ColumnRef();
}

RowRef RemoteSqlSheet::insertRow(const RowRef& base) {
  return RowRef();
}

bool RemoteSqlSheet::deleteRow(const RowRef& src) {
  return false;
}

