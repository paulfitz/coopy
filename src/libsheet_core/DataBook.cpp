#include <coopy/DataBook.h>
#include <coopy/SchemaSniffer.h>

#include <stdio.h>
#include <stdlib.h>

#include <algorithm>

using namespace std;
using namespace coopy::store;

bool DataBook::operator==(const DataBook& alt) const {
  DataBook *b1 = (DataBook *)this;
  DataBook *b2 = (DataBook *)&alt;
  vector<string> names = b1->getNames();
  vector<string> altNames = b2->getNames();
  if (names.size()!=altNames.size()) {
    return false;
  }
  for (int k=0; k<names.size(); k++) {
    PolySheet s1 = b1->readSheet(names[k]);
    SchemaSniffer sniffer1(s1,names[k].c_str());
    if (s1.getSchema()==NULL) {
      s1.setSchema(sniffer1.suggestSchema(),false);
    }
    s1.hideHeaders();
    PolySheet s2 = b2->readSheet(altNames[k]);
    SchemaSniffer sniffer2(s2,altNames[k].c_str());
    if (s2.getSchema()==NULL) {
      s2.setSchema(sniffer2.suggestSchema(),false);
    }
    s2.hideHeaders();
    if (s1.width()!=s2.width() || s1.height()!=s2.height()) {
      dbg_printf("Size mismatch, %dx%d vs %dx%d\n",
		 s1.width(), s1.height(),
		 s2.width(), s2.height());
      return false;
    }
    for (int j=0; j<s1.height(); j++) {
      for (int i=0; i<s1.width(); i++) {
	if (s1.cellSummary(i,j)!=s2.cellSummary(i,j)) {
	  printf("Mismatch on cell %d,%d: '%s' vs '%s'\n",
		 i,j, 
		 s1.cellSummary(i,j).toString().c_str(),
		 s2.cellSummary(i,j).toString().c_str());
	  return false;
	}
      }
    }
  }
  
  return true;
}


bool DataBook::copy(const DataBook& alt, const Property& options) {
  dbg_printf("Copying book\n");
  DataBook& src = (DataBook &) alt;
  vector<string> names = src.getNames();
  vector<string> names0 = getNames();

  string sel = options.get("sheet",PolyValue::makeString("")).asString();

  for (int i=0; i<(int)names.size(); i++) {
    string name = names[i];
    if (sel!="") {
      dbg_printf("Comparing %s and %s\n", sel.c_str(), name.c_str());
      if (sel!=name) {
	continue;
      }
    }
    dbg_printf("Working on %s\n", name.c_str());
    vector<string>::const_iterator it = find(names0.begin(),names0.end(),name);
    PolySheet sheet = src.readSheet(name);
    string target_name = name;
    SheetSchema *schema = sheet.getSchema();
    SchemaSniffer sniffer(sheet,name.c_str());
    //if (schema==NULL) {
    //dbg_printf(" - No schema available, sniffing...\n");
    schema = sniffer.suggestSchema();
    //}
    if (schema==NULL) {
      fprintf(stderr, "Cannot determine sheet schema: %s\n", name.c_str());
      return false;
    }
    dbg_printf("  - Have a schema with %d columns\n", 
	       schema->getColumnCount());
    dbg_printf("  - Have a table with %d rows\n", 
	       sheet.height());
    dbg_printf("  - Sheet name [%s]\n", 
	       schema->getSheetName().c_str());
    if (coopy_is_verbose()) {
      for (int i=0; i<schema->getColumnCount(); i++) {
	ColumnInfo info = schema->getColumnInfo(i);
	dbg_printf("    - column %s type %s\n", info.getName().c_str(),
		   info.getColumnType().asSqlite(true).c_str());
      }
    }
    PolySheet target = readSheet(target_name);
    if (!target.isValid()) {
      if (!addSheet(*schema)) {
	fprintf(stderr, "Failed to create sheet %s\n", name.c_str());
	return false;
      }
      target = readSheet(target_name);
    }
    if (!target.isValid()) {
      fprintf(stderr, "Failed to localize sheet %s\n", name.c_str());
      return false;
    }
    if (target.width()!=sheet.width()) {
      if (!target.hasDimension()) {
	target.forceWidth(sheet.width());
      }
    }
    if (target.width()!=sheet.width()) {
      fprintf(stderr, "Column mismatch %s\n", name.c_str());
      fprintf(stderr, "Src width %d, dest width %d\n", 
	      sheet.width(), target.width());
      return false;
    }
    if (target.height()!=0) {
      target.deleteData();
    }
    if (target.height()!=0) {
      if (target.hasDimension()) {
	fprintf(stderr, "Could not remove existing data: %s\n", name.c_str());
	return false;
      }
    }
    int start = 0;
    bool ext = target.hasExternalColumnNames()||target.getSchema()!=NULL;
    dbg_printf("  - target.hasExternalColumnNames? %d\n", 
	       target.hasExternalColumnNames());
    if (schema->headerHeight()>0) {
      if (ext) {
	start += schema->headerHeight();
      }
    }
    if (!ext) {
      if (schema->headerHeight()<=0 && !schema->isGuess()) {
	bool named = false;
	for (int j=0; j<schema->getColumnCount(); j++) {
	  ColumnInfo info = schema->getColumnInfo(j);
	  if (info.hasName()) { named = true; break; }
	}
	if (named) {
	  Poly<SheetRow> pRow = target.insertRow();
	  SheetRow& row = *pRow;
	  for (int j=0; j<schema->getColumnCount(); j++) {
	    ColumnInfo info = schema->getColumnInfo(j);
	    row.setCell(j,SheetCell(info.getName(),false));
	  }
	  row.flush();
	  target.addedHeader();
	}
      }
    }
    for (int i=start; i<sheet.height(); i++) {
      dbg_printf("Row %d\n", i);
      Poly<SheetRow> pRow = target.insertRow();
      SheetRow& row = *pRow;
      for (int j=0; j<sheet.width(); j++) {
	row.setCell(j,sheet.getCell(j,i));
      }
      row.flush();
    }
    dbg_printf("Final size for %s: src %dx%d target %dx%d\n", 
	       name.c_str(),
	       sheet.width(), sheet.height(),
	       target.width(), target.height());
  }
  return true;
}


PolySheet DataBook::provideSheet(const SheetSchema& schema) {
  dbg_printf("provideSheet %s for %s\n", schema.getSheetName().c_str(),
	     desc().c_str());
  PolySheet result = readSheet(schema.getSheetName());
  if (result.isValid()) {
    return result;
  }
  if (addSheet(schema)) {
    return readSheet(schema.getSheetName());
  }
  return result;
}
