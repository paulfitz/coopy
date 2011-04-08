#include <coopy/ColumnInfo.h>

#include <stdio.h>

using namespace coopy::store;

bool ColumnType::setType(const std::string& name, 
			 const std::string& lang) {
  reset();
  src_name = name;
  src_lang = lang;
  if (name=="INT"||name=="INTEGER"||name=="int"||name=="tinyint") {
    family = COLUMN_FAMILY_INTEGER;
  } else if (name=="TEXT") {
    family = COLUMN_FAMILY_TEXT;
  } else if (name == "Long Integer") {
    family = COLUMN_FAMILY_INTEGER;
  } else if (name == "float") {
    family = COLUMN_FAMILY_REAL;
  } else if (name == "DateTime (Short)"||name=="datetime"||name=="date") {
    family = COLUMN_FAMILY_DATETIME;
  } else if (name=="Text"||name=="text"||name=="varchar") {
    family = COLUMN_FAMILY_TEXT;
  }
  if (family==COLUMN_FAMILY_NONE) {
    fprintf(stderr,"Unrecognized type %s; update src/libsheet_core/ColumnInfo.cpp\n", name.c_str());
  }

  return family!=COLUMN_FAMILY_NONE;
}


std::string ColumnType::asSqlite(bool addPrimaryKey) const {
  std::string result = "";
  switch (family) {
  case COLUMN_FAMILY_INTEGER:
    result = "INTEGER";
    break;
  case COLUMN_FAMILY_TEXT:
    result = "TEXT";
    break;
  case COLUMN_FAMILY_DATETIME:
    result = "DATETIME"; // for documentation purposes only, really
    break;
  }
  bool pk = false;
  if (primaryKeySet&&addPrimaryKey) {
    if (primaryKey) {
      if (result!="") {
	result += " ";
      }
      result += "PRIMARY KEY";
      pk = true;
    }
  }
  if (!allowNull) {
    if (!pk) {
      if (result!="") {
	result += " ";
      }
      result += "NOT NULL";
    }
  }
  return result;
}


