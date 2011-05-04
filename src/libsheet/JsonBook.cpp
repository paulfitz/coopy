#include <coopy/JsonBook.h>
#include <coopy/FoldedSheet.h>
#include <coopy/Dbg.h>

#include <fstream>

#include <json/json.h>

using namespace std;
using namespace coopy::store;
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
     if ((*it).isArray()) {
       dbg_printf("JSON Working on %s\n", it.memberName());
       Json::Value& rows = *it;
       FoldedSheet *psheet = new FoldedSheet;
       COOPY_ASSERT(psheet);
       FoldedSheet& sheet = *psheet;
       PolySheet p(psheet,true);
       if (!readPart(rows,psheet)) return false;
       name2index[it.memberName()] = (int)sheets.size();
       sheets.push_back(p);
       names.push_back(it.memberName());
     }
  }

  return true;
}

bool JsonBook::write(const char *fname, TextBook *book) {
  if (book==NULL) return false;
  Json::Value root(Json::objectValue);
  ofstream out(fname);
  if (out.bad()) {
    fprintf(stderr,"Failed to open %s for writing\n", fname);
    return false;
  }
  vector<string> names = book->getNames();
  for (int i=0; i<(int)names.size(); i++) {
    root[names[i]] = Json::Value(Json::arrayValue);
    Json::Value& root2 = root[names[i]];
    PolySheet sheet = book->readSheet(names[i]);
    if (!sheet.isValid()) return false;
    for (int y=0; y<sheet.height(); y++) {
      Json::Value row(Json::arrayValue);
      for (int x=0; x<sheet.width(); x++) {
	SheetCell c = sheet.cellSummary(x,y);
	if (c.escaped) {
	  row.append(Json::Value(Json::nullValue));
	} else {
	  row.append(Json::Value(sheet.cellString(x,y)));
	}
      }
      root2.append(row);
    }
  }
  out << root;
  return true;
}

bool JsonBook::open(const Property& config) {
  if (!config.check("file")) return false;
  return read(config.get("file").asString().c_str());
}

bool JsonBook::addSheet(const SheetSchema& schema) {
  return false;
}

