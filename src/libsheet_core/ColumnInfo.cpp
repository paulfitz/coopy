#include <coopy/ColumnInfo.h>

#include <stdio.h>

using namespace coopy::store;

bool ColumnType::setType(const std::string& name, 
			 const std::string& lang) {
  reset();
  src_name = name;
  src_lang = lang;
  if (name=="INT"||name=="INTEGER") {
    family = COLUMN_FAMILY_INTEGER;
  } else if (name=="TEXT") {
    family = COLUMN_FAMILY_TEXT;
  } else if (name == "Long Integer") {
    family = COLUMN_FAMILY_INTEGER;
  } else if (name == "DateTime (Short)") {
    family = COLUMN_FAMILY_DATETIME;
  } else if (name=="Text") {
    family = COLUMN_FAMILY_TEXT;
  }

  return family!=COLUMN_FAMILY_NONE;
}


std::string ColumnType::asSqlite() const {
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
  return result;
}


