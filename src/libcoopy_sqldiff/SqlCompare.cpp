#include <coopy/SqlCompare.h>

using namespace std;
using namespace coopy::store;
using namespace coopy::cmp;

bool SqlCompare::validateSchema() {
  vector<string> all_cols1 = local.getColumnNames();
  vector<string> all_cols2 = remote.getColumnNames();

  if (all_cols1!=all_cols2) { 
    dbg_printf("Columns do not match, cannot make quick comparison\n");
    return false;
  }

  vector<string> key_cols1 = local.getPrimaryKey();
  vector<string> key_cols2 = remote.getPrimaryKey();

  if (key_cols1!=key_cols2) {
    dbg_printf("Keys do not match, cannot make quick comparison\n");
    return false;
  }

  if (key_cols1.size()==0) {
    dbg_printf("No key, cannot make quick comparison\n");
    return false;
  }

  return true;
}


bool SqlCompare::apply() {
  if (!db) return false;
  if (!validateSchema()) return false;

  dbg_printf("Making quick comparison\n");

  vector<string> key_cols = local.getPrimaryKey();
  vector<string> data_cols = local.getAllButPrimaryKey();
  vector<string> all_cols = local.getColumnNames();

  string sql_table1 = local.getQuotedTableName();
  string sql_table2 = remote.getQuotedTableName();
  string sql_key_cols;
  for (int i=0; i<(int)key_cols.size(); i++) {
    if (i>0) sql_key_cols += ",";
    sql_key_cols += local.getQuotedColumnName(key_cols[i]);
  }
  string sql_all_cols;
  for (int i=0; i<(int)all_cols.size(); i++) {
    if (i>0) sql_all_cols += ",";
    sql_all_cols += local.getQuotedColumnName(all_cols[i]);
  }
  string sql_key_match;
  for (int i=0; i<(int)key_cols.size(); i++) {
    if (i>0) sql_key_match += " AND ";
    string n = local.getQuotedColumnName(key_cols[i]);
    sql_key_match += sql_table1 + "." + n + " IS " + sql_table2 + "." + n;
  }
  string sql_data_mismatch;
  for (int i=0; i<(int)data_cols.size(); i++) {
    if (i>0) sql_data_mismatch += " OR ";
    string n = local.getQuotedColumnName(data_cols[i]);
    sql_data_mismatch += sql_table1 + "." + n + " IS NOT " + sql_table2 + "." + n;
  }
  string sql_dbl_cols;
  for (int i=0; i<(int)all_cols.size(); i++) {
    if (i>0) sql_dbl_cols += ",";
    string n = local.getQuotedColumnName(all_cols[i]);
    char buf[256];
    sprintf(buf,"__coopy_%d", i);
    sql_dbl_cols += sql_table1 + "." + n + " AS " + buf;
    sql_dbl_cols += ",";
    sql_dbl_cols += sql_table2 + "." + n + " AS " + buf + "b";
  }
 
  string sql_inserts = string("SELECT ") + sql_all_cols + " FROM " + sql_table2 + " WHERE NOT EXISTS (SELECT 1 FROM " + sql_table1 + " WHERE " + sql_key_match + ")";
  

  string sql_updates = string("SELECT ") + sql_dbl_cols + " FROM " + sql_table1 + " INNER JOIN " + sql_table2 + " ON " + sql_key_match + " WHERE " + sql_data_mismatch;

  string sql_deletes = string("SELECT ") + sql_all_cols + " FROM " + sql_table1 + " WHERE NOT EXISTS (SELECT 1 FROM " + sql_table2 + " WHERE " + sql_key_match + ")";

  dbg_printf(" SQL to find inserts: %s\n", sql_inserts.c_str());
  dbg_printf(" SQL to find updates: %s\n", sql_updates.c_str());
  dbg_printf(" SQL to find deletes: %s\n", sql_deletes.c_str());

  RowChange::txt2bool indexes;
  for (int i=0; i<(int)data_cols.size(); i++) {
    indexes[data_cols[i]] = false;
  }
  for (int i=0; i<(int)key_cols.size(); i++) {
    indexes[key_cols[i]] = true;
  }

  if (db->begin(sql_inserts)) {
    while (db->read()) {
      RowChange rc;
      rc.mode = ROW_CHANGE_INSERT;
      for (int i=0; i<(int)all_cols.size(); i++) {
	SheetCell c = db->get(i);
	string key = all_cols[i];
	dbg_printf("[%s] +%s\n", 
		   key.c_str(),
		   c.toString().c_str());
	rc.val[key] = c;
	rc.names.push_back(key);
      }
      rc.allNames = all_cols;
      rc.indexes = indexes;
      output.changeRow(rc);
    }
    db->end();
  }

  if (db->begin(sql_updates)) {
    // double all_cols
    while (db->read()) {
      RowChange rc;
      rc.mode = ROW_CHANGE_UPDATE;
      for (int i=0; i<(int)all_cols.size(); i++) {
	SheetCell c1 = db->get(2*i);
	SheetCell c2 = db->get(2*i+1);
	string key = all_cols[i];
	dbg_printf("[%s] %s => %s\n", 
		   key.c_str(),
		   c1.toString().c_str(), c2.toString().c_str());
	rc.cond[key] = c1;
	if (c1!=c2) {
	  rc.val[key] = c2;
	}
	rc.names.push_back(key);
      }
      rc.allNames = all_cols;
      rc.indexes = indexes;
      output.changeRow(rc);
    }
    db->end();
  }

  if (db->begin(sql_deletes)) {
    while (db->read()) {
      RowChange rc;
      rc.mode = ROW_CHANGE_DELETE;
      for (int i=0; i<(int)all_cols.size(); i++) {
	SheetCell c = db->get(i);
	string key = all_cols[i];
	dbg_printf("[%s] -%s\n", 
		   key.c_str(),
		   c.toString().c_str());
	rc.cond[key] = c;
	rc.names.push_back(key);
      }
      rc.allNames = all_cols;
      rc.indexes = indexes;
      output.changeRow(rc);
    }
    db->end();
  }

  return true;
}

