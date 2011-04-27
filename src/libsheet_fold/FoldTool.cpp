#include <coopy/FoldTool.h>
#include <coopy/FoldedSheet.h>
#include <coopy/CsvSheet.h>
#include <coopy/SchemaSniffer.h>
#include <coopy/ShortTextBook.h>
#include <coopy/CsvFile.h>

#include <vector>
#include <map>
#include <set>

using namespace std;
using namespace coopy::fold;
using namespace coopy::store;

typedef vector<int> ints;

class SheetCellCmp {
public:
  bool operator() (const SheetCell& e1, const SheetCell& e2) {
    if (e1.text<e2.text) return true;
    if (e1.text>e2.text) return false;
    return (e1.escaped?1:0)<(e2.escaped?1:0);
  }
};

class SheetAccess {
public:
  PolySheet sheet;
  SchemaSniffer schema;

  typedef multimap<SheetCell,int,SheetCellCmp> Mapper;
  typedef map<int,Mapper> Mappers;
  Mappers mappers;

  SheetAccess() {
  }

  void setSheet(const PolySheet& sheet) {
    this->sheet = sheet;
    schema.setSheet(sheet);
  }

  bool isValid() const {
    return sheet.isValid();
  }

  std::string getName() {
    SheetSchema *ss = schema.suggestSchema();
    if (!ss) {
      printf("No schema!\n");
      return "";
    }
    return ss->getSheetName();
  }

  std::string getName(int id) {
    SheetSchema *ss = schema.suggestSchema();
    if (!ss) {
      printf("No schema!\n");
      return "";
    }
    ColumnInfo info = ss->getColumnInfo(id);
    return info.getName();
  }

  int getId(const char *name) {
    SheetSchema *ss = schema.suggestSchema();
    if (!ss) {
      printf("No schema!\n");
      return -1;
    }
    int result = ss->getColumnIndexByName(name);
    if (result<0) {
      printf("Could not find column %s\n", name);
    }
    return result;
  }

  ints selectFrom(int id, const SheetCell& cell) {
    if (id==-1) {
      ints result;
      for (int i=0; i<sheet.height(); i++) {
	result.push_back(i); 
      }
      //printf("selectFrom generated %d results\n", result.size());
      return result;
    }

    if (mappers.find(id)==mappers.end()) {
      mappers[id] = Mapper();
      Mapper& m = mappers[id];
      dbg_printf("Generating index for %s:%d\n", getName().c_str(),id);
      for (int i=0; i<sheet.height(); i++) {
	//if (sheet.cellSummary(id,i)==cell) {
	m.insert(std::make_pair(sheet.cellSummary(id,i),i));
	//}
      }
    }
    Mapper& m = mappers[id];
    ints result;
    for (Mapper::iterator it = m.lower_bound(cell);
	   it != m.upper_bound(cell); it++) {	
      result.push_back(it->second);
    }
    //printf("selectFrom generated %d results\n", result.size());
    return result;
  }
};

class FoldLayout {
public:
  SheetAccess *src;
  SheetAccess *dest;
  string srcIdName;
  string destIdName;
  int offset;

  PolySheet srcTable;
  PolySheet destTable;
  int srcId;
  int destId;
  typedef multimap<int,int> mapper;
  mapper indexedSrc;
  mapper indexedDest;

  FoldLayout() {
    src = dest = NULL;
    srcId = -1;
    destId = -1;
    offset = 0;
  }

  bool updateBase() {
    if (src!=NULL) {
      srcTable = src->sheet;
    }
    if (dest!=NULL) {
      destTable = dest->sheet;
    }
  }

  bool updateIds() {
    if (srcId!=-1 && destId!=-1) return true;
    COOPY_ASSERT(src);
    COOPY_ASSERT(dest);
    srcId = src->getId(srcIdName.c_str());
    destId = dest->getId(destIdName.c_str());
    printf("IDs %s %s %d %d\n", srcIdName.c_str(), destIdName.c_str(), 
	   srcId, destId);
    return (srcId!=-1 && destId!=-1);
  }

  bool updateMap() {
    if (indexedSrc.size()!=0) return true;
    if (indexedDest.size()!=0) return true;
    if (!updateIds()) {
      printf("Failed to get IDs\n");
      return false;
    }
    COOPY_ASSERT(src);
    COOPY_ASSERT(dest);
    for (int y=0; y<srcTable.height(); y++) {
      int id = srcTable.cellSummary(srcId,y).asInt();
      indexedSrc.insert(std::make_pair(y,id));
      //printf("src %d -> %d\n", y, id);
    }
    for (int y=0; y<destTable.height(); y++) {
      int id = destTable.cellSummary(destId,y).asInt();
      indexedDest.insert(std::make_pair(id,y));
      //printf("dest %d -> %d\n", id, y);
    }
    return true;
  }

  int transformForward(int y) {
    mapper::iterator it1 = indexedSrc.find(y);
    if (it1==indexedSrc.end()) {
      printf("src %d went nowhere\n", y);
      return -1;
    }
    mapper::iterator it2 = indexedDest.find(it1->second);
    if (it2==indexedDest.end()) {
      printf("dest %d from src %d went nowhere\n", it1->second, y);
      return -1;
    }
    return it2->second;
  }

  /*
  ints transform(ints y) {
    ints result;
    for (int i=0; i<(int)y.size(); i++) {
      for (mapper::iterator it1 = indexedSrc.lower_bound(y);
	   it1 != indexedSrc.upper_bound(y); it1++) {	
	for (mapper::iterator it2 = indexedDest.lower_bound(it1->second);
	     it2 != indexedDest.upper_bound(it1->second); it2++) {	
	  results.push_back(it2->second);
	}
      }
    }
    return results;
  }
  */
};
  
class FoldCache {
public:
  SheetAccess dud;
  PolyBook book;

  void setBook(const PolyBook& book) {
    this->book = book;
  }

  map<string,SheetAccess> sheets;

  SheetAccess& getSheet(const char *name) {
    map<string,SheetAccess>::iterator it = sheets.find(name);
    if (it!=sheets.end()) {
      return it->second;
    }
    PolySheet sheet = book.readSheet(name);
    if (!sheet.isValid()) {
      fprintf(stderr,"Could not find table %s\n", name);
      return dud;
    }
    sheets[name] = SheetAccess();
    sheets[name].setSheet(sheet);
    return sheets[name];
  }
};

FoldTool::FoldTool() {
}

class FoldEdge {
public:
  string table;
  string key;

  string toString() const {
    return table + ":" + key;
  }
};

class FoldEdgePair {
public:
  FoldEdge to, from;
  string allowed;

  string toString() const {
    return to.toString() + "->" + from.toString() + "(" + allowed + ")";
  }

  string desc(const string& ref) {
    string result = "";
    if (from.table!=ref) {
      result += from.table + ":";
    }
    result += from.key;
    result += "->";
    if (to.table!=ref) {
      result += to.table + ":";
    }
    result += to.key;
    return result;
  }
};

class FoldEdgeCmp {
public:
  bool operator() (const FoldEdge& e1, const FoldEdge& e2) {
    if (e1.table<e2.table) return true;
    if (e1.table>e2.table) return false;
    return e1.key<e2.key;
  }
};

class FoldEdgePairCmp {
public:
  bool operator() (const FoldEdgePair& e1, const FoldEdgePair& e2) {
    FoldEdgeCmp cmp;
    if (cmp(e1.from,e2.from)) return true;
    if (cmp(e2.from,e1.from)) return false;
    if (cmp(e1.to,e2.to)) return true;
    if (cmp(e2.to,e1.to)) return false;
    return e1.allowed<e2.allowed;
  }
};

class Folds {
public:
  typedef multimap<FoldEdge,FoldEdge,FoldEdgeCmp> EdgeMap;
  typedef set<FoldEdgePair,FoldEdgePairCmp> AllowedPair;

  EdgeMap fwd;
  EdgeMap rev;
  AllowedPair allowed;

  void add(const char *fromTable, const char *toTable, 
	   const char *fromField, const char *toField,
	   const char *allowedTable) {
    FoldEdge e1, e2;
    e1.table = fromTable;
    e1.key = fromField;
    e2.table = toTable;
    e2.key = toField;
    fwd.insert(make_pair(e1,e2));
    rev.insert(make_pair(e2,e1));
    FoldEdgePair p1;
    p1.from = e1;
    p1.to = e2;
    p1.allowed = allowedTable; 
    allowed.insert(p1);
    FoldEdgePair p2;
    p2.from = e2;
    p2.to = e1;
    p2.allowed = allowedTable;
    allowed.insert(p2);
    //printf("Added %s -> %s\n", e1.toString().c_str(), e2.toString().c_str());
    //printf("Allowed %s\n", p1.toString().c_str());
    //printf("Allowed %s\n", p2.toString().c_str());
  }
};

class FoldSelector {
public:
  // table to select from
  string tableName;
  // key name in that table
  string keyName;
  // id of key name in that table
  int id;

  // value to match
  SheetCell val;
  // id in local table
  int idLocal;

  FoldSelector() {
    id = -1;
    idLocal = -1;
  }

  string toString() const {
    return tableName + ":" + keyName + "=" + val.toString();
  }
};

class FakeBook : public TextBook {
public:
  std::string name;
  PolySheet sheet;

  FakeBook() : name("sheet") {
  }

  virtual std::vector<std::string> getNames() {
    std::vector<std::string> result;
    result.push_back(name);
    return result;
  }

  virtual PolySheet readSheet(const std::string& name) {
    if (name==this->name) {
      return sheet;
    }
    return PolySheet();
  }

  virtual bool open(const Property& config) {
    return false;
  }

  virtual PolySheet provideSheet(const SheetSchema& schema) {
    return PolySheet();
  }

  virtual std::string desc() const {
    return "FakeBook";
  }

  virtual bool namedSheets() const {
    return false;
  }
};


static bool fold_expander(Folds& folds, FoldCache& cache, FoldedSheet& sheet,
			  FoldSelector& sel, PolySheet *target = NULL) {
  //printf("EXPANDER from %s selecting %s\n", sel.tableName.c_str(),
  //	 sel.toString().c_str());
  SheetAccess& base = cache.getSheet(sel.tableName.c_str());
  if (!base.isValid()) {
    return false;
  }
  PolySheet& src = base.sheet;
  //SchemaSniffer& schema = base.schema;
  int selId = -1;
  if (sel.keyName!="") {
    selId = base.getId(sel.keyName.c_str());
  }
  SimpleSheetSchema *schema = NULL;
  if (selId==-1) {
    schema = new SimpleSheetSchema;
    COOPY_ASSERT(schema);
    schema->setSheetName("flat");
  }

  //printf("Working on sheet %dx%d\n", src.width(), src.height());
  FoldedCell fcell;
  vector<int> selected;
  selected = base.selectFrom(selId,sel.val);

  /*
  for (int y=0; y<src.height(); y++) {
    if (selId!=-1) {
      if (src.cellSummary(selId,y)==sel.val) {
	selected.push_back(y);
      }
    } else {
      selected.push_back(y);
    }
  }
  */

  vector<FoldSelector> expanded;
  for (int x=0; x<src.width(); x++) {
    if (selId==-1) {
      schema->addColumn(base.getName(x).c_str());
    }
  }
  for (int x=0; x<src.width(); x++) {
    FoldEdge e;
    e.table = sel.tableName;
    e.key = base.getName(x);
    if (selId==-1) {
      dbg_printf("Checking %s\n", e.toString().c_str());
    }
    Folds::EdgeMap::iterator it = folds.fwd.lower_bound(e);
    Folds::EdgeMap::iterator it_end = folds.fwd.upper_bound(e);
    while (it!=it_end) {
      FoldEdgePair p;
      p.to = it->first;
      p.from = it->second;
      p.allowed = sel.tableName;
      if (selId==-1) {
	dbg_printf("Checking %s\n", p.toString().c_str());
      }
      if (folds.allowed.find(p)!=folds.allowed.end()) {
	if (selId==-1) {
	  dbg_printf("Should expand out %s (to %s)\n", e.key.c_str(),
		     it->second.toString().c_str());
	}
	FoldSelector f;
	f.tableName = it->second.table;
	f.keyName = it->second.key;
	SheetAccess& alt = cache.getSheet(f.tableName.c_str());
	if (!alt.isValid()) {
	  return false;
	}
	f.id = alt.getId(f.keyName.c_str());
	f.idLocal = base.getId(it->first.key.c_str());
	expanded.push_back(f);
	if (selId==-1) {
	  schema->addColumn(p.desc(base.getName()).c_str());
	}
      }
      it++;
    }
    Folds::EdgeMap::iterator it2 = folds.rev.lower_bound(e);
    Folds::EdgeMap::iterator it2_end = folds.rev.upper_bound(e);
    while (it2!=it2_end) {
      FoldEdgePair p;
      p.to = it2->first;
      p.from = it2->second;
      p.allowed = sel.tableName;
      if (selId==-1) {
	dbg_printf("Checking %s\n", p.toString().c_str());
      }
      if (folds.allowed.find(p)!=folds.allowed.end()) {
	if (selId==-1) {
	  dbg_printf("Should expand in %s (from %s)\n", e.key.c_str(),
		     it2->second.toString().c_str());
	}
	FoldSelector f;
	f.tableName = it2->second.table;
	f.keyName = it2->second.key;
	SheetAccess& alt = cache.getSheet(f.tableName.c_str());
	if (!alt.isValid()) {
	  return false;
	}
	f.id = alt.getId(f.keyName.c_str());
	f.idLocal = base.getId(it2->first.key.c_str());
	expanded.push_back(f);
	if (selId==-1) {
	  schema->addColumn(p.desc(base.getName()).c_str());
	}
      }
      it2++;
    }
  }

  int hh = selected.size();
  int ww0 = src.width();
  int ww1 = expanded.size();
  int ww = ww0 + ww1;
  //printf("Producing sheet %dx%d\n", ww, hh);

  sheet.resize(ww,hh,fcell);
  for (vector<int>::iterator yit=selected.begin(); yit!=selected.end(); yit++) {
    int y = *yit;
    int y0 = yit-selected.begin();
    for (int x=0; x<ww0; x++) {
      FoldedCell& cell = sheet.cell(x,y0);
      cell.datum = src.cellSummary(x,y);
    }
    for (int x=0; x<ww1; x++) {
      FoldSelector f = expanded[x];
      f.val = src.cellSummary(f.idLocal,y);
      FoldedCell& cell = sheet.cell(x+ww0,y0);
      FoldedSheet *sheet = cell.getOrCreateSheet();
      COOPY_ASSERT(sheet);
      fold_expander(folds, cache, *sheet, f);
    }
  }

  if (selId==-1) {
    if (target!=NULL) {
      target->setSchema(schema,true);
    }
  }
  return true;
}

bool FoldTool::fold(PolyBook& src, PolyBook& dest, FoldOptions& options) {
  printf("Starting fold/unfold\n");

  if (options.tableName=="") {
    fprintf(stderr,"Please supply a root table name\n");
    return false;
  }

  FoldCache cache;
  cache.setBook(src);

  SheetAccess& base = cache.getSheet(options.tableName.c_str());
  if (!base.isValid()) {
    return false;
  }

  vector<FoldLayout> layout;
  FoldLayout baseLayout;
  baseLayout.src = &base;
  baseLayout.updateBase();
  layout.push_back(baseLayout);

  PolySheet recipe = options.recipe.readSheetByIndex(0);
  recipe.hideHeaders();
  Folds folds;
  for (int i=0; i<recipe.height(); i++) {
    string fromTable = recipe.cellString(0,i);
    string fromField = recipe.cellString(1,i);
    string toTable = recipe.cellString(2,i);
    string toField = recipe.cellString(3,i);
    string allowedTable = recipe.cellString(4,i);
    dbg_printf("Recipe line %d: %s %s %s %s (%s)\n", i,
	       fromTable.c_str(), toTable.c_str(), fromField.c_str(), toField.c_str(),allowedTable.c_str());
    folds.add(fromTable.c_str(), toTable.c_str(), fromField.c_str(), toField.c_str(),allowedTable.c_str());
    if (fromTable==options.tableName) {
      /*
      SheetAccess& alt = cache.getSheet(toTable.c_str());
      if (!alt.isValid()) {
	return false;
      }
      layout.push_back(FoldLayout());
      FoldLayout& l = layout.back();
      l.src = &base;
      l.dest = &alt;
      l.srcIdName = fromField;
      l.destIdName = toField;
      l.updateBase();
      l.updateMap();
      */
    }
  }

  FoldedSheet *fsheet = new FoldedSheet;
  PolySheet psheet(fsheet,true);
  COOPY_ASSERT(fsheet);
  FoldSelector sel;
  sel.tableName = options.tableName;
  fold_expander(folds, cache, *fsheet, sel, &psheet);

  //printf("Generated sheet %dx%d\n", fsheet->width(), fsheet->height());

  FakeBook *book = new FakeBook();
  if (book==NULL) {
    fprintf(stderr,"Failed to allocate output\n");
    return 1;
  }
  book->sheet = psheet;
  dest.take(book);

  return true;
}



/*
bool FoldTool::fold(PolyBook& src, PolyBook& dest, FoldOptions& options) {
  printf("Starting fold/unfold\n");

  if (options.tableName=="") {
    fprintf(stderr,"Please supply a root table name\n");
    return false;
  }

  FoldCache cache;
  cache.setBook(src);

  SheetAccess& base = cache.getSheet(options.tableName.c_str());
  if (!base.isValid()) {
    return false;
  }

  vector<FoldLayout> layout;
  FoldLayout baseLayout;
  baseLayout.src = &base;
  baseLayout.updateBase();
  layout.push_back(baseLayout);

  PolySheet recipe = options.recipe.readSheetByIndex(0);
  recipe.hideHeaders();
  for (int i=0; i<recipe.height(); i++) {
    string fromTable = recipe.cellString(0,i);
    string fromField = recipe.cellString(1,i);
    string toTable = recipe.cellString(2,i);
    string toField = recipe.cellString(3,i);
    printf("Recipe line %d: %s %s %s %s\n", i,
	   fromTable.c_str(), toTable.c_str(), fromField.c_str(), toField.c_str());
    if (fromTable==options.tableName) {
      SheetAccess& alt = cache.getSheet(toTable.c_str());
      if (!alt.isValid()) {
	return false;
      }
      layout.push_back(FoldLayout());
      FoldLayout& l = layout.back();
      l.src = &base;
      l.dest = &alt;
      l.srcIdName = fromField;
      l.destIdName = toField;
      l.updateBase();
      l.updateMap();
    }
  }

  ShortTextBook *book = new ShortTextBook();
  if (book==NULL) {
    fprintf(stderr,"Failed to allocate output\n");
    return 1;
  }
  printf("Starting to generate output\n");
  SimpleSheetSchema schema;
  schema.setSheetName("flat");
  for (int i=0; i<(int)layout.size(); i++) {
    FoldLayout& unit = layout[i];
    if (i==0) {
      for (int x=0; x<unit.srcTable.width(); x++) {
	schema.addColumn(unit.src->getName(x).c_str());
      }
    } else {
      for (int x=0; x<unit.destTable.width(); x++) {
	string remoteField = unit.dest->getName(x);
	string localField = unit.srcIdName;
	string remoteTable = unit.dest->getName();
	string name = remoteField + ":" + remoteTable + "[" + localField + "]";
	schema.addColumn(name.c_str());
      }
    }
  }
  book->provideSheet(schema);
  CsvSheet& accum = book->sheet;

  for (int y=0; y<base.sheet.height(); y++) {
    for (int i=0; i<(int)layout.size(); i++) {
      FoldLayout& unit = layout[i];
      if (i==0) {
	for (int x=0; x<unit.srcTable.width(); x++) {
	  accum.addField(unit.srcTable.cellSummary(x,y));
	}
      } else {
	int yy = unit.transformForward(y);
	for (int x=0; x<unit.destTable.width(); x++) {
	  if (yy>=0) {
	    accum.addField(unit.destTable.cellSummary(x,yy));
	  } else {
	    accum.addField(SheetCell());
	  }
	}
      }
    }
    accum.addRecord();
  }

  dest.take(book);

  return true;
}


*/
