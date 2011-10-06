#include <coopy/MergeOutputSqlDiff.h>

#include <stdio.h>
#include <stdlib.h>

#define WANT_MAP2STRING
#define WANT_VECTOR2STRING
#include <coopy/Stringer.h>

using namespace std;
using namespace coopy::cmp;


bool MergeOutputSqlDiff::changeColumn(const OrderChange& change) {
  string name = quoteSql(sheet_name,'\"',false);
  switch (change.mode) {
  case ORDER_CHANGE_DELETE:
    {
      int idx = change.identityToIndex(change.subject);
      if (change.namesBefore.size()<=idx) {
	fprintf(stderr, "Could not find column to remove\n");
	exit(1);
      } else {
	string c = quoteSql(change.namesBefore[idx],'\"',false);
	fprintf(out,"ALTER TABLE %s DROP COLUMN %s;\n",
		name.c_str(),
		c.c_str());
      }
    }	    
    break;
  case ORDER_CHANGE_INSERT:
    {
      int idx = change.identityToIndexAfter(change.subject);
      if (change.namesAfter.size()<=idx) {
	fprintf(stderr, "Could not find column to insert\n");
	exit(1);
      } else {
	string c = quoteSql(change.namesAfter[idx],'\"',false);
	fprintf(out,"ALTER TABLE %s ADD COLUMN %s;\n",
		name.c_str(),
		c.c_str());
      }
    }
    break;
  }
  return true;
}


SqlText MergeOutputSqlDiff::getText(const RowChange& change, const char *sheet_name) {
  SqlText text;
  string& name = text.name;
  string& vals = text.vals;
  string& conds = text.conds;
  string& val_columns = text.val_columns;
  string& val_values = text.val_values;

  name = quoteSql(sheet_name,'\"',false);
  for (RowChange::txt2cell::const_iterator it=change.val.begin(); 
       it!=change.val.end(); 
       it++) {
    if (it!=change.val.begin()) {
      vals += ", ";
      val_columns += ", ";
      val_values += ", ";
    }
    // Should dig up good default quoting rules
    string c = quoteSql(it->first,'\"',false);
    vals += c;
    val_columns += c;
    vals += "=";
    if (it->second.escaped) {
      vals += "NULL";
      val_values += "NULL";
    } else {
      string q = quoteSql(it->second.text,'\'',true);
      vals += q;
      val_values += q;
    }
  }

  for (RowChange::txt2cell::const_iterator it=change.cond.begin(); 
       it!=change.cond.end(); 
       it++) {
    if (it!=change.cond.begin()) {
      conds += " AND ";
    }
    string c = quoteSql(it->first,'\"',false);
    conds += c;
    conds += "=";
    if (it->second.escaped) {
      conds += "NULL";
    } else {
      string q = quoteSql(it->second.text,'\'',true);
      conds += q;
    }
  }
  return text;
}

bool MergeOutputSqlDiff::changeRow(const RowChange& change) {
  SqlText text = getText(change,sheet_name.c_str());
  string& name = text.name;
  string& vals = text.vals;
  string& conds = text.conds;
  string& val_columns = text.val_columns;
  string& val_values = text.val_values;

  switch (change.mode) {
  case ROW_CHANGE_INSERT:
    fprintf(out,"INSERT INTO %s (%s) VALUES (%s);\n", 
	    name.c_str(),
	    val_columns.c_str(),
	    val_values.c_str());
    break;
  case ROW_CHANGE_DELETE:
    fprintf(out,"DELETE FROM %s WHERE %s;\n",
	    name.c_str(),
	    conds.c_str());
    break;
  case ROW_CHANGE_UPDATE:
    fprintf(out,"UPDATE %s SET %s WHERE %s;\n", 
	    name.c_str(),
	    vals.c_str(),
	    conds.c_str());
    break;
  default:
    // skip
    break;
  }
  return true;
}


