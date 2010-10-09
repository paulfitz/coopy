#include <coopy/CsvPatcher.h>

#include <algorithm>

#include <stdio.h>

using namespace std;
using namespace coopy::cmp;
using namespace coopy::store;

bool CsvPatcher::changeColumn(const OrderChange& change) {
  if (sheet==NULL) return false;
  switch (change.mode) {
  case ORDER_CHANGE_DELETE:
    return sheet->deleteColumn(ColumnRef(change.identityToIndex(change.subject)));
    break;
  case ORDER_CHANGE_INSERT:
    return sheet->insertColumn(ColumnRef(change.identityToIndex(change.subject))).isValid();
    break;
  case ORDER_CHANGE_MOVE:
    return sheet->moveColumn(ColumnRef(change.identityToIndex(change.subject)),
			     ColumnRef(change.identityToIndex(change.object))
			     ).isValid();
  default:
    fprintf(stderr,"* ERROR: Unknown column operation\n");
    break;
  }
  return false;
}

bool CsvPatcher::changeRow(const RowChange& change) {
  if (sheet==NULL) return false;
  fprintf(stderr,"* ERROR: Not ready for rows\n");
  return false;
}

bool CsvPatcher::declareNames(const std::vector<std::string>& names, 
			      bool final) {
  if (sheet==NULL) return false;
  if ((int)names.size()!=sheet->width()) {
    fprintf(stderr,"* ERROR: name mismatch\n");
    return false;
  }
  return true;
}

