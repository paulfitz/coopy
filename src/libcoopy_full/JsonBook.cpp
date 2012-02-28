#include <coopy/JsonBook.h>
#include <coopy/FoldedSheet.h>
#include <coopy/Dbg.h>

#include <fstream>

#include <json/json.h>

using namespace std;
using namespace coopy::store;
using namespace coopy::store::json;
using namespace coopy::fold;

static bool readPart(Json::Value& rows,
		     FoldedSheet *psheet) {
  FoldedSheet& sheet = *psheet;

  int y = 0;
  for (Json::Value::iterator it2=rows.begin(); it2!=rows.end(); it2++) {
    //for (int i=0; i<rows.size(); i++) {
    dbg_printf("JSON Row %d\n", y);
    if ((*it2).isArray()) {
      Json::Value& cols = *it2;
      if (y==0) {
	FoldedCell zero;
	sheet.resize((int)cols.size(),1,zero);
      } else {
	sheet.insertRow(RowRef(-1));
      }
      int x = 0;
      for (Json::Value::iterator it3=cols.begin(); it3!=cols.end(); it3++) {
	string s;
	bool escaped = false;
	bool nested = false;
	if ((*it3).isInt()) {
	  //printf("At %d %d -> %d\n", x, y, (*it3).asInt());
	  char buf[1000];
	  snprintf(buf,sizeof(buf),"%d",(*it3).asInt());
	  s = buf;
	} else if ((*it3).isNumeric()) {
	  //printf("At %d %d -> %g\n", x, y, (*it3).asDouble());	
	  char buf[1000];
	  snprintf(buf,sizeof(buf),"%g",(*it3).asDouble());
	  s = buf;
	} else if ((*it3).isNull()) {
	  s = "NULL";
	  escaped = true;
	} else if ((*it3).isArray()||(*it3).isObject()) {
	  if (!readPart((*it3),sheet.cell(x,y).getOrCreateSheet())) {
	    return false;
	  }
	  nested = true;
	} else {
	  //printf("At %d %d -> %s\n", x, y, (*it3).asString().c_str());
	  s = (*it3).asString();
	}
	if (!nested) {
	  sheet.cellString(x,y,s,escaped);
	}
	x++;
      }
    }
    y++;
  }

  return true;
}

bool JsonBook::read(const char *fname) {
  clear();

  ifstream in(fname);
  Json::Value root;
  Json::Reader reader;
  if (!reader.parse(in,root,false)) {
    fprintf(stderr,"Failed to parse %s\n", fname);
    return false;
  }
  for (Json::Value::iterator it=root.begin(); it!=root.end(); it++) {
    FoldedSheet *psheet = new FoldedSheet;
    COOPY_ASSERT(psheet);
    FoldedSheet& sheet = *psheet;
    PolySheet p(psheet,true);

    Json::Value *prows = &(*it);
    if (prows->isObject()) {
      Json::Value& crows = (*it);
      prows = &(crows["rows"]);
      if (prows->isNull()) {
	fprintf(stderr,"Cannot find rows for %s\n", it.memberName());
	return false;
      }
      const Json::Value& cols = (*it)["columns"];
      if (cols.isNull()) {
	fprintf(stderr,"Cannot find columns for %s\n", it.memberName());
	return false;
      }
      SimpleSheetSchema *ss = new SimpleSheetSchema();
      COOPY_ASSERT(ss);
      ss->setSheetName(it.memberName());
      for (Json::Value::iterator it=cols.begin(); it!=cols.end(); it++) {
	//printf("Got col %s\n", (*it).asString().c_str());
	ss->addColumn((*it).asString().c_str());
      }
      p.setSchema(ss,true);
      //printf("Configured sheet\n");
    }
    if (prows->isArray()) {
      dbg_printf("JSON Working on %s\n", it.memberName());
      Json::Value& rows = *prows;
      if (!readPart(rows,psheet)) return false;
      name2index[it.memberName()] = (int)sheets.size();
      sheets.push_back(p);
      names.push_back(it.memberName());
    }
  }

  return true;
}

static bool writePart(Json::Value& root2,
		      DataSheet *psheet,
		      bool hasSchema) {
  DataSheet& sheet = *psheet;
  Json::Value *rows = &root2;
  if (hasSchema) {
    root2["columns"] = Json::Value(Json::arrayValue);
    Json::Value& cols = root2["columns"];
    SheetSchema *schema = sheet.getSchema();
    if (schema->getColumnCount()!=sheet.width()) {
      fprintf(stderr,"warning: partial schema information, %d columns in sheet, %d column names\n", sheet.width(), schema->getColumnCount());
    }
    for (int x=0; x<schema->getColumnCount(); x++) {
      ColumnInfo info = schema->getColumnInfo(x);
      cols.append(Json::Value(info.getName()));
    }    
    root2["rows"] = Json::Value(Json::arrayValue);
    rows = &root2["rows"];
  }
  for (int y=0; y<sheet.height(); y++) {
    Json::Value row(Json::arrayValue);
    for (int x=0; x<sheet.width(); x++) {
      DataSheet *next = sheet.getNestedSheet(x,y);
      if (next==NULL) {
	SheetCell c = sheet.cellSummary(x,y);
	if (c.escaped) {
	  row.append(Json::Value(Json::nullValue));
	} else {
	  row.append(Json::Value(sheet.cellString(x,y)));
	}
      } else {
	bool hasSchema2 = (next->getSchema()!=NULL);
	if (hasSchema2) {
	  row.append(Json::Value(Json::objectValue));
	} else {
	  row.append(Json::Value(Json::arrayValue));
	}
	if (!writePart(row[x],next,hasSchema2)) return false;
      }
    }
    rows->append(row);
  }
  return true;
}

bool JsonBook::write(const char *fname, TextBook *book) {
  if (book==NULL) return false;
  Json::Value root(Json::objectValue);
  ostream *fout = &cout;
  ofstream out;
  if (string(fname)!="-") {
    out.open(fname);
    fout = &out;
  }


  if (fout->bad()) {
    fprintf(stderr,"Failed to open %s for writing\n", fname);
    return false;
  }
  vector<string> names = book->getNames();
  for (int i=0; i<(int)names.size(); i++) {
    PolySheet sheet = book->readSheet(names[i]);
    bool hasSchema = (sheet.getSchema()!=NULL);
    if (hasSchema) {
      root[names[i]] = Json::Value(Json::objectValue);
    } else {
      root[names[i]] = Json::Value(Json::arrayValue);
    }
    Json::Value& root2 = root[names[i]];
    if (!sheet.isValid()) return false;
    if (!writePart(root2,&sheet,hasSchema)) return false;
  }
  (*fout) << root;
  return true;
}

bool JsonBook::open(const Property& config) {
  if (!config.check("file")) return false;
  return read(config.get("file").asString().c_str());
}

bool JsonBook::addSheet(const SheetSchema& schema) {
  dbg_printf("jsonbook::addsheet %s\n", schema.getSheetName().c_str());
  FoldedSheet *psheet = new FoldedSheet;
  COOPY_ASSERT(psheet);
  FoldedSheet& sheet = *psheet;
  PolySheet p(psheet,true);
  SimpleSheetSchema *sss = new SimpleSheetSchema();
  COOPY_ASSERT(sss);
  sss->copy(schema);
  p.setSchema(sss,true);
  name2index[schema.getSheetName()] = (int)sheets.size();
  sheet.resize(schema.getColumnCount(),0,FoldedCell());
  dbg_printf("CREATED %d %d (%d)\n", p.width(), p.height(), schema.getColumnCount());
  sheets.push_back(p);
  names.push_back(schema.getSheetName());
  return true;
}

