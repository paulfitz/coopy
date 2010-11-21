#include <coopy/ColumnInfo.h>

#include <stdio.h>

using namespace coopy::store;

bool ColumnType::setType(const std::string& name, 
			 const std::string& lang) {
  reset();
  if (name=="INT"||name=="INTEGER") {
    family = COLUMN_FAMILY_INTEGER;
  } else if (name=="TEXT") {
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
  }
  return result;
}


