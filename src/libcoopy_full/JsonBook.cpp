#include <coopy/JsonBook.h>
#include <coopy/FoldedSheet.h>
#include <coopy/Dbg.h>
#include <coopy/FormatSniffer.h>
#include <coopy/FileIO.h>

#include <fstream>

#include <json/json.h>

using namespace std;
using namespace coopy::store;
using namespace coopy::store::json;
using namespace coopy::format;
using namespace coopy::fold;

static Json::Value cellToJson(const SheetCell& cell, const ColumnInfo& info) {
  const ColumnType& t = info.getColumnType();
  if (t.family == ColumnType::COLUMN_FAMILY_INTEGER) return Json::Value(cell.asInt());
  return Json::Value(cell.text);
}

static bool readPart(Json::Value& rows,
		     FoldedSheet *psheet,
		     SheetSchema *schema) {
  FoldedSheet& sheet = *psheet;

  int y = 0;
  Json::Value arr(Json::arrayValue);
  for (Json::Value::iterator it2=rows.begin(); it2!=rows.end(); it2++) {
    dbg_printf("JSON Row %d\n", y);
    if ((*it2).isArray() || (schema && (*it2).isObject())) {
      bool subst = false;
      if ((*it2).isObject()) {
	arr = Json::Value(Json::arrayValue);
	for (int x=0; x<schema->getColumnCount(); x++) {
	  ColumnInfo info = schema->getColumnInfo(x);
	  arr.append((*it2)[info.getName()]);
	}
	subst = true;
      }
      Json::Value& cols = subst?arr:*it2;
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
	  char buf[1000];
	  snprintf(buf,sizeof(buf),"%d",(*it3).asInt());
	  s = buf;
	} else if ((*it3).isNumeric()) {
	  char buf[1000];
	  snprintf(buf,sizeof(buf),"%g",(*it3).asDouble());
	  s = buf;
	} else if ((*it3).isNull()) {
	  s = "NULL";
	  escaped = true;
	} else if ((*it3).isArray()||(*it3).isObject()) {
	  if (!readPart((*it3),sheet.cell(x,y).getOrCreateSheet(),NULL)) {
	    return false;
	  }
	  nested = true;
	} else {
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

static bool readTable(JsonBook *self,
                      Json::Value& table,
                      const std::string& table_name) {

  FoldedSheet *psheet = new FoldedSheet;
  COOPY_ASSERT(psheet);
  FoldedSheet& sheet = *psheet;
  PolySheet p(psheet,true);
  SheetSchema *schema = NULL;

  Json::Value *prows = &table;
  if (prows->isObject()) {
    Json::Value& crows = table;
    prows = &(crows["rows"]);
    if (prows->isNull()) {
      fprintf(stderr,"Cannot find rows for %s\n", table_name.c_str());
      return false;
    }
    const Json::Value& cols = table["columns"];
    if (cols.isNull()) {
      fprintf(stderr,"Cannot find columns for %s\n", table_name.c_str());
      return false;
    }
    SimpleSheetSchema *ss = new SimpleSheetSchema();
    COOPY_ASSERT(ss);
    ss->setSheetName(table_name.c_str());
    for (Json::Value::iterator it=cols.begin(); it!=cols.end(); it++) {
      ss->addColumn((*it).asString().c_str());
    }
    p.setSchema(ss,true);
    schema = ss;
  }
  if (prows->isArray()) {
    dbg_printf("JSON Working on %s\n", table_name.c_str());
    Json::Value& rows = *prows;
    if (!readPart(rows,psheet,schema)) return false;
    self->name2index[table_name] = (int)self->sheets.size();
    self->sheets.push_back(p);
    self->names.push_back(table_name);
  }
  return true;
}

bool JsonBook::read(const char *fname, const Property& options) {
  clear();

  FileIO fio;
  FormatSniffer in;
  if (!fio.open(fname,options)) {
    fprintf(stderr,"Failed to open %s\n", fname);
    return false;
  }
  in.wrap(fio,true);

  Json::Value root;
  Json::Reader reader;
  if (!reader.parse(in.read(),root,false)) {
    fprintf(stderr,"Failed to parse %s\n", fname);
    return false;
  }

  if (root.isMember("names") && root.isMember("tables")) {
    Json::Value& names = root["names"];
    Json::Value& tables = root["tables"];
    if (names.isArray()&&tables.isObject()) {
      for (Json::Value::iterator it=names.begin(); it!=names.end(); it++) {
        std::string name = (*it).asString();
        Json::Value& sheet = tables[name];
        readTable(this,sheet,name);
      }
    }
    return true;
  }

  for (Json::Value::iterator it=root.begin(); it!=root.end(); it++) {
    Json::Value& table = *it;
    readTable(this,table,it.memberName());
  }

  return true;
}

static bool writePart(Json::Value& root2,
		      DataSheet *psheet,
		      bool hasSchema,
		      bool nestSchema) {
  DataSheet& sheet = *psheet;
  Json::Value *rows = &root2;
  vector<string> names;
  vector<ColumnInfo> infos;
  bool hasNames = false;
  if (hasSchema) {
    root2["columns"] = Json::Value(Json::arrayValue);
    Json::Value& cols = root2["columns"];
    SheetSchema *schema = sheet.getSchema();
    if (schema->getColumnCount()!=sheet.width()) {
      fprintf(stderr,"warning: partial schema information, %d columns in sheet, %d column names\n", sheet.width(), schema->getColumnCount());
    }
    for (int x=0; x<schema->getColumnCount(); x++) {
      ColumnInfo info = schema->getColumnInfo(x);
      names.push_back(info.getName());
      infos.push_back(info);
      hasNames = nestSchema;
      cols.append(Json::Value(info.getName()));
    }    
    root2["rows"] = Json::Value(Json::arrayValue);
    rows = &root2["rows"];
  }
  for (int y=0; y<sheet.height(); y++) {
    Json::Value row(hasNames?Json::objectValue:Json::arrayValue);
    for (int x=0; x<sheet.width(); x++) {
      DataSheet *next = sheet.getNestedSheet(x,y);
      if (next==NULL) {
	SheetCell c = sheet.cellSummary(x,y);
	if (c.escaped) {
	  if (hasNames) {
	    row[names[x]] = Json::Value(Json::nullValue);
	  } else {
	    row.append(Json::Value(Json::nullValue));
	  }
	} else {
	  if (hasNames) {
	    row[names[x]] = cellToJson(c,infos[x]);
	  } else {
	    row.append(Json::Value(sheet.cellString(x,y)));
	  }
	}
      } else {
	bool hasSchema2 = (next->getSchema()!=NULL);
	if (hasSchema2) {
	  row.append(Json::Value(Json::objectValue));
	} else {
	  row.append(Json::Value(Json::arrayValue));
	}
	if (!writePart(row[x],next,hasSchema2,nestSchema)) return false;
      }
    }
    rows->append(row);
  }
  return true;
}

static bool renderJsonBook(Json::Value& root, TextBook *book, 
			   const Property& options) {
  vector<string> names = book->getNames();
  for (int i=0; i<(int)names.size(); i++) {
    PolySheet sheet = book->readSheet(names[i]);
    bool hasSchema = false;
    bool nestSchema = true;
    if (options.check("hash")) {
      int h = options.get("hash").asInt();
      nestSchema = (h>0);
    }
    if (nestSchema) {
      sheet.mustHaveSchema();
    }
    if (sheet.getSchema()!=NULL) {
      hasSchema = true;
    }
    if (hasSchema) {
      root[names[i]] = Json::Value(Json::objectValue);
    } else {
      root[names[i]] = Json::Value(Json::arrayValue);
    }
    Json::Value& root2 = root[names[i]];
    if (!sheet.isValid()) return false;
    if (!writePart(root2,&sheet,hasSchema,nestSchema)) return false;
  }
  return true;
}

std::string JsonBook::render(TextBook *book, const Property& options) {
  Json::Value root(Json::objectValue);
  if (!renderJsonBook(root,book,options)) return "";
  return root.toStyledString();
}

bool JsonBook::write(const char *fname, TextBook *book, const Property& options) {
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
  if (!renderJsonBook(root,book,options)) return false;
  (*fout) << root;
  return true;
}

bool JsonBook::open(const Property& config) {
  if (!config.check("file")) return false;
  return read(config.get("file").asString().c_str(),config);
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

