#include <coopy/Dbg.h>
#include <coopy/AccessSheet.h>

#include "mdbtools.h"

using namespace coopy::store;
using namespace std;

namespace coopy {
  namespace store {
    class AccessSchema;
  }
}

#define DB(x) ((MdbHandle *)(x))
#define TABLE(x) ((MdbTableDef *)(x))

AccessSheet::AccessSheet(void *db1, const char *name, const Property& config) {
  implementation = db1;
  table_implementation = NULL;
  this->name = name;
  this->config = config;
  w = h = 0;

  schema = new AccessSheetSchema;
  COOPY_MEMORY(schema);
  schema->sheet = this;
}

bool AccessSheet::connect() {
  MdbHandle *mdb = DB(implementation);
  if (mdb==NULL) return false;

  MdbTableDef *table = mdb_read_table_by_name(mdb, (char *)name.c_str(), 
					      MDB_TABLE);
  if (!table) return false;
  table_implementation = table;
  mdb_read_columns(table);
  mdb_read_indices(table);
  mdb_rewind_table(table);

  //////////////////////////////////////////////////////////////////
  // Check dimensions

  h = (int)table->num_rows;
  w = (int)table->num_cols;

  //////////////////////////////////////////////////////////////////
  // Check column names

  for (int j=0; j<table->num_cols; j++) {
    MdbColumn *col=(MdbColumn *)g_ptr_array_index(table->columns,j);
    col2sql.push_back(col->name);
    col2pk.push_back(false);
    char *kind = mdb_get_coltype_string(mdb->default_backend, col->col_type);
    col2type.push_back(kind);
  }

  //////////////////////////////////////////////////////////////////
  // Check keys

  if (config.flag("indexes",true)) {
    for (int i=0;i<table->num_idxs;i++) {
      MdbIndex *idx = (MdbIndex *)g_ptr_array_index(table->indices,i);
      if (idx->index_type==1) {
	for (int j=0;j<idx->num_keys;j++) {
	  int off = idx->key_col_num[j]-1;
	  col2pk[off] = true;
	}
      }
    }
  }

  //////////////////////////////////////////////////////////////////
  // Cache data - haven't figured out random access yet

  cache.resize(w,h,"");
  cacheFlag.resize(w,h,0);

  char **bound_values = (char **)g_malloc(table->num_cols * sizeof(char *));
  int *bound_lens = (int *)g_malloc(table->num_cols * sizeof(int));
  for (int j=0;j<table->num_cols;j++) {
    bound_values[j] = (char *) g_malloc0(MDB_BIND_SIZE);
    mdb_bind_column(table, j+1, bound_values[j], &bound_lens[j]);
  }
  int at = 0;
  while(mdb_fetch_row(table)) {
     for (int j=0;j<table->num_cols;j++) {
      MdbColumn *col=(MdbColumn *)g_ptr_array_index(table->columns,j);
      bool ole = false;
      if ((col->col_type == MDB_OLE)
	  && ((j==0) || (col->cur_value_len))) {
	ole = true;
      }
      string val;
      bool escaped;
      if (ole) {
	val = "<MEDIA>";
	escaped = true;
      } else if (!bound_lens[j]) {
	val = "";
	escaped = true;
      } else {
	val = bound_values[j];
	escaped = false;
      }
      //printf("Got %d %d -> %s\n", j, at, val.c_str());
      cache.cell(j,at) = val;
      if (escaped) {
	cacheFlag.cell(j,at) = escaped?1:0;
      }
     }
     at++;
  }
  for (int j=0;j<table->num_cols;j++) {
    g_free(bound_values[j]);
  }
  g_free(bound_values);
  g_free(bound_lens);

  return true;
}

AccessSheet::~AccessSheet() {
  if (table_implementation!=NULL) {
    mdb_free_tabledef(TABLE(table_implementation));
    table_implementation = NULL;
  }
  if (schema!=NULL) delete schema;
  schema = NULL;
}

SheetSchema *AccessSheet::getSchema() const {
  return schema;
}


std::string AccessSheet::cellString(int x, int y) const {
  bool tmp;
  return cellString(x,y,tmp);
}

std::string AccessSheet::cellString(int x, int y, bool& escaped) const {
  escaped = false;
  const unsigned char *f = cacheFlag.pcell_const(x,y);
  if (f!=NULL) {
    escaped = true;
    return "";
  }
  const string *c = cache.pcell_const(x,y);
  if (c!=NULL) {
    return *c;
  }

  escaped = true;
  return "";
}

