#include <stdio.h>

#include "mdbtools.h"

#include <coopy/AccessTextBook.h>

#include <vector>
#include <string>

using namespace coopy::store;
using namespace std;

/*
static bool is_col_indexed(MdbTableDef *table, MdbColumn *col, int colnum) {
  if (col->bind_ptr) {
    for (int i=0;i<table->num_idxs;i++) {
      MdbIndex *idx = (MdbIndex *)g_ptr_array_index(table->indices,i);
      for (int j=0;j<idx->num_keys;j++) {
	if (idx->key_col_num[j]==colnum) return true;
      }
    }
  }
  return false;
}
*/

int main(int argc, char *argv[]) {
  if (argc!=2) {
    fprintf(stderr,"Provide exactly one argument, a mdb file please\n");
    return 1;
  }

  mdb_init();

  MdbHandle *mdb;
  mdb = mdb_open(argv[1],MDB_NOFLAGS);
  if (!mdb) {
    fprintf(stderr,"Failed to open %s\n",argv[1]);
    return 1;
  }
  if (!mdb_read_catalog(mdb,MDB_ANY)) {
    fprintf(stderr,"Failed to read as Access file: %s\n",argv[1]);
    return 1;
  }
  std::vector<std::string> tables;
  for (int i=0; i<mdb->num_catalog; i++) {
    MdbCatalogEntry *entry = (MdbCatalogEntry *)g_ptr_array_index(mdb->catalog, i);
    if (entry->object_type!=MDB_TABLE) continue;
    if (mdb_is_system_table(entry)) continue;
    printf("Found table %s ...\n", entry->object_name);
    tables.push_back(entry->object_name);
  }
  
  for (int i=0; i<(int)tables.size(); i++) {
  
    MdbTableDef *table = mdb_read_table_by_name(mdb, (char *)tables[i].c_str(), 
						MDB_TABLE);
    printf("[%s]\n", tables[i].c_str());
    if (!table) continue;
    mdb_read_columns(table);
    //mdb_read_indices(table);
    mdb_rewind_table(table);
    
    char **bound_values = (char **)g_malloc(table->num_cols * sizeof(char *));
    int *bound_lens = (int *)g_malloc(table->num_cols * sizeof(int));
    for (int j=0;j<table->num_cols;j++) {
      bound_values[j] = (char *) g_malloc0(MDB_BIND_SIZE);
      mdb_bind_column(table, j+1, bound_values[j], &bound_lens[j]);
    }
    //header
    printf("   ");
    for (int j=0; j<table->num_cols; j++) {
      MdbColumn *col=(MdbColumn *)g_ptr_array_index(table->columns,j);
      //bool idx = is_col_indexed(table,col,j);
      bool idx = false;
      char *kind = mdb_get_coltype_string(mdb->default_backend, col->col_type);
      fprintf(stdout,"%s%s:%s ",idx?"*":"",col->name,
	      kind);
    }
    printf("\n");

    while(mdb_fetch_row(table)) {
      printf("   ");
      for (int j=0;j<table->num_cols;j++) {
	MdbColumn *col=(MdbColumn *)g_ptr_array_index(table->columns,j);
	bool ole = false;
	if ((col->col_type == MDB_OLE)
	    && ((j==0) || (col->cur_value_len))) {
	  //mdb_ole_read(mdb, col, bound_values[j], MDB_BIND_SIZE);
	  ole = true;
	}
	if (ole) {
	  printf("<MEDIA> ");
	} else if (!bound_lens[j]) {
	  printf("NULL ");
	} else {
	  printf("%s ", bound_values[j]);
	}
      }
      printf("\n");
    }
    for (int j=0;j<table->num_cols;j++) {
      g_free(bound_values[j]);
    }
    g_free(bound_values);
    g_free(bound_lens);
    mdb_free_tabledef(table);
  }

  mdb_close(mdb);
  mdb_exit();

  /*
  AccessTextBook book;
  book.read(argv[1]);
  vector<string> names = book.getNames();
  if (names.size()>0) {
    PolySheet sheet = book.readSheet(names[0]);
    if (sheet.isValid()) {
      printf("Size %dx%d\n", sheet.width(), sheet.height());
      printf("At (1,0): %s\n", sheet.cellString(1,0).c_str());
    }
  }
  */
  
  return 0;
}

