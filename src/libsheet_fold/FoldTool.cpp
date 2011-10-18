#include <coopy/FoldTool.h>
#include <coopy/FoldedSheet.h>
#include <coopy/CsvSheet.h>
#include <coopy/SchemaSniffer.h>
#include <coopy/ShortTextBook.h>
#include <coopy/CsvFile.h>
#include <coopy/IntSheet.h>

#include <vector>
#include <map>
#include <set>

#include <coopy/Stringer.h>

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
      for (int i=0; i<ss->getColumnCount(); i++) {
	ColumnInfo info = ss->getColumnInfo(i);
	printf("  col %d %s\n", i, info.getName().c_str());
      }
      exit(1);
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

};


class FoldEdge {
public:
  string table;
  string key;

  FoldEdge() {}

  FoldEdge(string ntable, string nkey) {
    table = ntable;
    key = nkey;
  }

  string toString() const {
    return table + ":" + key;
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

  int minCt;
  int maxCt;

  int actualMinCt;
  int actualMaxCt;
  int targetLength;

  string title;

  FoldSelector() {
    id = -1;
    idLocal = -1;
    minCt = maxCt = -1;
    actualMinCt = -1;
    actualMaxCt = 0;
    targetLength = -1;
  }

  string toString() const {
    return tableName + ":" + keyName + "=" + val.toString();
  }
};

  
class Folds;

struct Expansion {
public:
  vector<FoldSelector>& expanded;
  SheetAccess& base;
  PolySheet& src;
  int selId;
};

class FoldCache {
public:
  SheetAccess dud;
  PolySheet dud_sheet;
  PolyBook book;

  map<FoldEdge,vector<FoldSelector>, FoldEdgeCmp > expanded_map;

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
      fprintf(stderr,"Could not find table \"%s\"\n", name);
      return dud;
    }
    sheets[name] = SheetAccess();
    sheets[name].setSheet(sheet);
    return sheets[name];
  }

  //vector<FoldSelector>& 
  Expansion getExpansion(const FoldEdge& edge, Folds *folds);
};

FoldTool::FoldTool() {
}

class FoldEdgePair {
public:
  FoldEdge to, from;
  string allowed;
  int minCt, maxCt;
  string label;

  FoldEdgePair() {
    minCt = maxCt = -1;
  }

  string toString() const {
    string result = to.toString() + "->" + from.toString() + "(" + allowed + ")";
    result += ",";
    result += stringer_encoder(minCt);
    result += ",";
    result += stringer_encoder(maxCt);
    result += ",";
    result += label;
    return result;
  }

  string desc(const string& ref) const {
    if (label!="-") return label;

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
	   const char *allowedTable, 
	   int minCt, int maxCt,
	   const char *label) {
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
    p1.minCt = minCt;
    p1.maxCt = maxCt;
    p1.label = label;
    allowed.insert(p1);
    FoldEdgePair p2;
    p2.from = e2;
    p2.to = e1;
    p2.allowed = allowedTable;
    p2.minCt = minCt;
    p2.maxCt = maxCt;
    p2.label = label;
    allowed.insert(p2);
    //printf("Added %s -> %s\n", p1.toString().c_str(), p2.toString().c_str());
    //printf("Allowed %s\n", p1.toString().c_str());
    //printf("Allowed %s\n", p2.toString().c_str());
  }
};

class FakeBook : public TextBook {
public:
  std::string name;
  PolySheet sheet;

  FakeBook() : name(coopy_get_default_table_name()) {
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



Expansion FoldCache::getExpansion(const FoldEdge& edge,
				  Folds *pfolds) {
  Folds& folds = *pfolds;
  FoldCache& cache = *this;

  bool exists = false;
  if (expanded_map.find(edge)==expanded_map.end()) {
    expanded_map[edge] = vector<FoldSelector>();
  } else {
    exists = true;
  }
  vector<FoldSelector>& expanded = expanded_map[edge];

  SheetAccess& base = cache.getSheet(edge.table.c_str());
  PolySheet& src = base.isValid()?base.sheet:dud_sheet;
  int selId = -1;
  if (edge.key!="") {
    selId = base.getId(edge.key.c_str());
  }
  Expansion result = {expanded,base,src,selId};
  if (exists) {
    return result;
  }

  for (int x=0; x<src.width(); x++) {
    FoldEdge e;
    e.table = edge.table;
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
      p.allowed = edge.table;
      if (selId==-1) {
	dbg_printf("Checking %s\n", p.toString().c_str());
      }
      set<FoldEdgePair,FoldEdgePairCmp>::iterator al = folds.allowed.find(p);
      if (al!=folds.allowed.end()) {
	if (selId==-1) {
	  dbg_printf("Should expand out %s (to %s)\n", e.key.c_str(),
		     it->second.toString().c_str());
	}
	FoldSelector f;
	f.tableName = it->second.table;
	f.keyName = it->second.key;
	SheetAccess& alt = cache.getSheet(f.tableName.c_str());
	if (!alt.isValid()) {
	  expanded.clear();
	  return result;
	}
	f.id = alt.getId(f.keyName.c_str());
	f.idLocal = base.getId(it->first.key.c_str());
	f.minCt = al->minCt;
	f.maxCt = al->maxCt;
	f.title = al->desc(base.getName());
	expanded.push_back(f);
      }
      it++;
    }
    Folds::EdgeMap::iterator it2 = folds.rev.lower_bound(e);
    Folds::EdgeMap::iterator it2_end = folds.rev.upper_bound(e);
    while (it2!=it2_end) {
      FoldEdgePair p;
      p.to = it2->first;
      p.from = it2->second;
      p.allowed = edge.table;
      if (selId==-1) {
	dbg_printf("Checking %s\n", p.toString().c_str());
      }
      set<FoldEdgePair,FoldEdgePairCmp>::iterator al = folds.allowed.find(p);
      if (al!=folds.allowed.end()) {
	if (selId==-1) {
	  dbg_printf("Should expand in %s (from %s)\n", e.key.c_str(),
		     it2->second.toString().c_str());
	}
	FoldSelector f;
	f.tableName = it2->second.table;
	f.keyName = it2->second.key;
	SheetAccess& alt = cache.getSheet(f.tableName.c_str());
	if (!alt.isValid()) {
	  expanded.clear();
	  return result;
	}
	f.id = alt.getId(f.keyName.c_str());
	f.idLocal = base.getId(it2->first.key.c_str());
	f.minCt = al->minCt;
	f.maxCt = al->maxCt;
	f.title = al->desc(base.getName());
	expanded.push_back(f);
      }
      it2++;
    }
  }
  return result;
}

class ColumnHistory {
public:
  string name;
  string from_table;
  string from_name;
};

class FoldFactor {
public:
  int ct;
  bool excess;
  bool wrap;
  int xoffset;
  int yoffset;
  int depth;
  int skips;
  bool practice;
  bool namer;
  string prefix;
  IntSheet *zebra;
  map<string,ColumnHistory> *history;
  FoldSelector *selector;

  FoldFactor() { 
    ct = -1; 
    excess = false; 
    wrap = false;
    xoffset = yoffset = 0;
    depth = 0;
    practice = false;
    skips = 0;
    zebra = NULL;
    history = NULL;
    namer = true;
    selector = NULL;
  }
};


void assertColumn(SimpleSheetSchema *s, int offset, const string& name,
		  const string& orig_name,
		  const FoldFactor& factor) {
  if (s==NULL) return;
  while (s->getColumnCount()<=offset) {
    s->addColumn("...");
  }
  ColumnInfo c = s->getColumnInfo(offset);
  if (c.getName()!=name) {
    s->modifyColumn(ColumnRef(offset),ColumnInfo(name));
    if (factor.history&&factor.selector) {
      ColumnHistory hist;
      hist.name = name;
      hist.from_table = factor.selector->tableName;
      hist.from_name = orig_name;
      (*factor.history)[hist.name] = hist;
    }
  }
}


static int fold_expander(const FoldFactor& factor,
			 Folds& folds, FoldCache& cache, 
			 FoldedSheet& sheet,
			 FoldSelector& sel,
			 SimpleSheetSchema *schema = NULL,
			 int *ywrap = NULL) {

  bool practice = factor.practice;

  Expansion exp = cache.getExpansion(FoldEdge(sel.tableName,sel.keyName),
				     &folds);

  // get selection
  vector<int> selected = exp.base.selectFrom(exp.selId,sel.val);

  // Prepare to iterate
  if (factor.depth==0) {
    if (!practice) {
      sheet.resize(exp.src.width(),selected.size()-factor.skips,FoldedCell());
    }
  }

  int xoffset = factor.xoffset;
  int yoffset = factor.yoffset;
  int initial_xoffset = xoffset;
  int fct = 0;
  int fskip = 0;
  int cell_length = 0;

  int at = 0;
  int z = 1;
  bool namer = factor.namer;
  for (vector<int>::iterator yit=selected.begin(); yit!=selected.end(); yit++) {
    z = 1-z;
    int y = *yit;
    int y0 = at; //yit-selected.begin();
    int dy = 0;

    string prefix = factor.prefix;
    if (prefix!=""&&factor.depth>0&&!practice) {
      if (sel.actualMaxCt>1) {
	prefix += "[";
	prefix += stringer_encoder(fct);
	//prefix += ":";
	//prefix += stringer_encoder(sel.actualMaxCt);
	prefix += "]";
      }
    }

    fct++;
    if (fskip<factor.skips) {
      fskip++;
      continue;
    }

    bool out_of_space = (fct>factor.ct && factor.ct!=-1);
    if (out_of_space&&!factor.wrap) {
      printf("TOO LONG! %d vs %d\n", fct, factor.ct);
      if (!practice) {
	if (xoffset>=sheet.width()) {
	  sheet.nonDestructiveResize(sheet.width()+1,sheet.height(),
				     FoldedCell());
	}
	if (namer) assertColumn(schema,xoffset,prefix + ".excess","excess",
				factor);
	FoldedCell& cell = sheet.cell(xoffset,yoffset);
	FoldedSheet *sheet = cell.getOrCreateSheet();
	COOPY_ASSERT(sheet);
	FoldFactor next_factor;
	next_factor.skips = fct-1;
	next_factor.namer = namer;
	next_factor.selector = &sel;
	next_factor.history = factor.history;
	fold_expander(next_factor, folds, cache, *sheet, sel);
      }
      printf("done\n");
      xoffset++;
      break;
    }

    if (out_of_space&&factor.wrap) {
      namer = false;
      yoffset++;
      xoffset = initial_xoffset;
      fct -= factor.ct;
      if (ywrap!=NULL) {
	(*ywrap)++;
      }
      /*
      printf("at %d %d / %d\n", sheet.width(), sheet.height(), yoffset);
      FoldedCell& cell = sheet.cell(0,yoffset);
      cell.datum = SheetCell("...",false);
      */
    }

    if (factor.depth==0) {
      xoffset = 0;
      yoffset = y0-fskip;
    }

    // add regular columns
    if (!practice) {
      if (yoffset>=sheet.height()) {
	sheet.nonDestructiveResize(sheet.width(),yoffset+1,
				   FoldedCell());
      }

      for (int x=0; x<exp.src.width(); x++) {
	if (xoffset>=sheet.width()) {
	  sheet.nonDestructiveResize(sheet.width()+1,sheet.height(),
				     FoldedCell());
	}
	string oname = exp.base.getName(x);
	string name = oname;
	if (prefix!="") {
	  name = prefix + "." + name;
	}
	if (namer) assertColumn(schema,xoffset,name,oname,factor);
	
	FoldedCell& cell = sheet.cell(xoffset,yoffset);
	cell.datum = exp.src.cellSummary(x,y);
	/*
	if (schema) {
	  printf("[%d:%d:%d] add %d %d -> %d %d [%s]\n", xoffset, 
		 schema->getColumnCount(),
		 sheet.width(),
		 x, y, xoffset,yoffset,
		 cell.datum.toString().c_str());
	}
	*/
	xoffset++;
      }
    } else {
      xoffset += exp.src.width();
    }

    // add expansions
    for (int x=0; x<exp.expanded.size(); x++) {
      FoldSelector& f = exp.expanded[x];
      f.val = exp.src.cellSummary(f.idLocal,y);
      string oname = f.title;
      string name = oname;
      if (prefix!="") {
	name = prefix + "." + name;
      }
      if (f.minCt==-1 && f.maxCt==-1) {
	if (!practice) {
	  if (xoffset>=sheet.width()) {
	    sheet.nonDestructiveResize(sheet.width()+1,sheet.height(),
				       FoldedCell());
	  }
	  if (namer) assertColumn(schema,xoffset,name,oname,factor); // PFHIT
	  FoldedCell& cell = sheet.cell(xoffset,yoffset);
	  FoldedSheet *sheet = cell.getOrCreateSheet();
	  COOPY_ASSERT(sheet);
	  fold_expander(FoldFactor(),folds, cache, *sheet, f);
	}
	xoffset++;
      } else {
	//if (!practice) printf("Go go\n");
	FoldFactor next_factor;
	next_factor.ct = f.maxCt;
	next_factor.excess = f.minCt==1;
	next_factor.wrap = f.minCt==2;
	next_factor.xoffset = xoffset;
	next_factor.yoffset = yoffset;
	next_factor.depth = factor.depth+1;
	next_factor.practice = practice;
	next_factor.prefix = name;
	next_factor.namer = namer;
	next_factor.selector = &f;
	next_factor.history = factor.history;
	int next_ywrap = 0;
	int o = fold_expander(next_factor, folds, cache, sheet, f, schema,
			      &next_ywrap);
	if (next_ywrap>dy) {
	  dy = next_ywrap;
	}
	xoffset += o;
      }
    }
    int ncell_length = xoffset-initial_xoffset;
    if (ncell_length>cell_length) {
      cell_length = ncell_length;
    }
    if (factor.zebra) {
      factor.zebra->nonDestructiveResize(1,sheet.height(),0);
      for (int i=at; i<at+dy+1; i++) {
	//printf("set %d %d (%d)\n", i, z, sheet.height());
	factor.zebra->cell(0,i) = z;
      }
    }
    if (factor.depth==0) {
      at += dy;
    }
    at++;
  }

  int ncell_length = xoffset-initial_xoffset;
  if (ncell_length>cell_length) {
    cell_length = ncell_length;
  }

  if (practice) {
    int len = (int)selected.size();
    if (len<sel.actualMinCt||sel.actualMinCt==-1) sel.actualMinCt = len;
    if (len>sel.actualMaxCt) sel.actualMaxCt = len;
    if (cell_length>sel.targetLength) sel.targetLength = cell_length;
  } 
  
  if (factor.depth>0) {
    while (cell_length<sel.targetLength) {
      if (xoffset>=sheet.width()) {
	sheet.nonDestructiveResize(sheet.width()+1,sheet.height(),
				   FoldedCell());
      }
      xoffset++;
      cell_length++;
    }
  }

  return cell_length;
}



static void replace(string& str, const string& old, const string& rep) {
  size_t pos = 0;
  while((pos = str.find(old, pos)) != std::string::npos) {
    str.replace(pos, old.length(), rep);
    pos += rep.length();
  }
}


bool FoldTool::fold(PolyBook& src, PolyBook& rdest, FoldOptions& options) {
  PolyBook dest;

  dbg_printf("Starting fold/unfold\n");

  if (options.tableName=="" && src.getSheetCount()>1) {
    fprintf(stderr,"Please supply a root table name\n");
    return false;
  }

  FoldCache cache;
  cache.setBook(src);

  PolySheet recipe = options.recipe.readSheet("Folds");
  IntSheet zebra;
  map<string,ColumnHistory> history;
  if (recipe.isValid()) {
    dbg_printf("Found folds\n");

    SheetAccess& base = cache.getSheet(options.tableName.c_str());
    if (!base.isValid()) {
      return false;
    }
    
    vector<FoldLayout> layout;
    FoldLayout baseLayout;
    baseLayout.src = &base;
    baseLayout.updateBase();
    layout.push_back(baseLayout);
    
    //options.recipe.write("/tmp/test.sqlite");
    //fprintf(stderr, "failed to read recipe (no Folds table)\n");
    //    exit(1);

    recipe.hideHeaders();
    
    Folds folds;
    for (int i=0; i<recipe.height(); i++) {
      string fromTable = recipe.cellString(0,i);
      string fromField = recipe.cellString(1,i);
      string toTable = recipe.cellString(2,i);
      string toField = recipe.cellString(3,i);
      string allowedTable = recipe.cellString(4,i);
      SheetCell minCtStr;
      SheetCell maxCtStr;
      int minCt = -1;
      int maxCt = -1;
      string label;
      if (recipe.width()>6) {
	minCtStr = recipe.cellSummary(6,i);
	if (!minCtStr.escaped)  minCt = atoi(minCtStr.text.c_str());
	maxCtStr = recipe.cellSummary(5,i);
	if (!maxCtStr.escaped)  maxCt = atoi(maxCtStr.text.c_str());
      }
      if (recipe.width()>7) {
	label = recipe.cellSummary(7,i).text.c_str();
      }
      dbg_printf("Recipe line %d: %s %s %s %s (%s) %d:%d '%s'\n", i,
		 fromTable.c_str(), toTable.c_str(), fromField.c_str(), toField.c_str(),allowedTable.c_str(),minCt,maxCt,label.c_str());
      folds.add(fromTable.c_str(), toTable.c_str(), fromField.c_str(), toField.c_str(),allowedTable.c_str(), minCt, maxCt,label.c_str());
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

    SimpleSheetSchema *schema = new SimpleSheetSchema;
    COOPY_ASSERT(schema);
    schema->setSheetName(coopy_get_default_table_name());
    FoldFactor factor;
    factor.practice = true;
    factor.selector = &sel;
    int prev_width = -1;
    int width = 0;
    while (prev_width!=width) {
      prev_width = width;
      width = fold_expander(factor, folds, cache, *fsheet, sel, schema);
      printf("On practice run, width is %d\n", width);
    }

    factor.practice = false;
    factor.zebra = &zebra;
    factor.history = &history;
    fold_expander(factor, folds, cache, *fsheet, sel, schema);
    printf("After actual run, data width is %d\n", fsheet->width());
    printf("After actual run, schema width is %d\n", schema->getColumnCount());

    psheet.setSchema(schema,true);
    //printf("Generated sheet %dx%d\n", fsheet->width(), fsheet->height());

    FakeBook *book = new FakeBook();
    if (book==NULL) {
      fprintf(stderr,"Failed to allocate output\n");
      return 1;
    }
    book->sheet = psheet;
    dest.take(book); 
    src = dest;
  } else {
    dest = src;
  }

  vector<string> drop_inventory;
  vector<string> orig_inventory;
  map<string,string> fate_inventory;
  map<string,bool> doom_inventory;
  PolySheet missing = options.recipe.readSheet("Missing");
  //printf("recipe? -- %s\n", options.recipe.toString().c_str());
  if (missing.isValid()) {
    dbg_printf("Processing list of \"Missing\" columns\n");
    for (int y=0; y<missing.height(); y++) {
      string name = missing.cellString(0,y);
      options.drops.insert(name);
    }
  }

  if (options.drops.size()>0) {
    dbg_printf("Working on drops...\n");
    vector<string> names = src.getNames();
    for (int i=0; i<src.getSheetCount(); i++) {
      PolySheet sheet = src.readSheet(names[i]);
      SchemaSniffer ss(sheet);
      SheetSchema *schema = ss.suggestSchema();
      if (!schema) {
	dbg_printf("No schema for %s\n", names[i].c_str());
	continue;
      }
      SimpleSheetSchema s;
      s.copy(*schema);
      int at = 0;
      for (int c=0; c<s.getColumnCount(); c++) {
	string iname = s.getColumnInfo(c).getName();
	orig_inventory.push_back(iname);
	string name;
	bool quoted = false;
	for (int i=0; i<(int)iname.length(); i++) {
	  if (iname[i]=='[') quoted = true;
	  if (!quoted) {
	    name += iname[i];
	  }
	  if (iname[i]==']') quoted = false;
	}
	if (options.drops.find(name)!=options.drops.end()) {
	  dbg_printf(" + Dropping column %s\n", iname.c_str());
	  sheet.deleteColumn(ColumnRef(at));
	  drop_inventory.push_back(iname);
	  doom_inventory[iname] = true;
	} else {
	  at++;
	}
      }
    }
  }

  PolySheet rename = options.recipe.readSheet("Rename");
  map<string,string> rename_map;
  if (rename.isValid()) {
    dbg_printf("Processing list of \"Rename\" columns\n");
    for (int y=0; y<rename.height(); y++) {
      string from = rename.cellString(0,y);
      string to = rename.cellString(1,y);
      rename_map[from] = to;
    }
  }

  if (rename_map.size()>0) {
    vector<string> names = src.getNames();
    for (int i=0; i<src.getSheetCount(); i++) {
      PolySheet sheet = src.readSheet(names[i]);
      SchemaSniffer ss(sheet);
      SheetSchema *schema = ss.suggestSchema();
      if (!schema) {
	dbg_printf("No schema for %s\n", names[i].c_str());
	continue;
      }
      SheetSchema& s = *schema;
      int at = 0;
      bool mod = false;
      for (int c=0; c<s.getColumnCount(); c++) {
	string iname = s.getColumnInfo(c).getName();
	string prev = iname;

	for (map<string,string>::iterator it=rename_map.begin();
	     it != rename_map.end(); it++) {
	  replace(iname,it->first,it->second);
	}
	if (iname!=prev) {
	  printf(">>> %s -> %s\n", prev.c_str(), iname.c_str());
	  s.modifyColumn(ColumnRef(c),ColumnInfo(iname));
	  fate_inventory[prev] = iname;
	  mod = true;
	}
      }
    }
    for (int i=0; i<src.getSheetCount(); i++) {
      //map
    }
  }

  // add zebra
  printf("Copying...\n");
  rdest.copy(dest,Property());
  printf("Sheets... %d\n",rdest.getNames().size());
  PolySheet sheet = rdest.readSheetByIndex(0);
  COOPY_ASSERT(sheet.isValid());
  sheet.hideHeaders();
  int LIGHT = 0xbb*0x100;
  int DARK = 0x99*0x100;
  for (int i=0; i<sheet.height(); i++) {
    //printf("i %d zebra %d\n", i, zebra.cell(0,i));
    if (i>0) {
      if (zebra.cell(0,i)==zebra.cell(0,i-1)) {
	sheet.cellString(0,i,"...");
      }
    }
    Poly<Appearance> app = sheet.getRowAppearance(i);
    int r = zebra.cell(0,i)?LIGHT:DARK;
    int g = r;
    int b = r;
    if (app.isValid()) {
      app->begin();
      app->setBackgroundRgb16(r,g,b,
			      AppearanceRange::full());
      app->end();
    }
  }
  
  SimpleSheetSchema adder_schema;
  adder_schema.setSheetName("mapping");
  adder_schema.addColumn("NAME");
  adder_schema.addColumn("FATE");
  adder_schema.addColumn("ALIAS");
  adder_schema.addColumn("ORIG_TABLE");
  adder_schema.addColumn("ORIG_COLUMN");
  PolySheet adder = rdest.provideSheet(adder_schema);
  adder.setSchema(&adder_schema,false);
  adder.resize(5,orig_inventory.size());
  adder.createHeaders();
  //adder.hideHeaders();
  printf("Size %d %d / %d\n", adder.width(), adder.height(),
	 orig_inventory.size());
  for (int i=0; i<(int)orig_inventory.size(); i++) {
    string n = orig_inventory[i];
    adder.cellString(0,i,n);
    if (fate_inventory.find(n)!=fate_inventory.end()) {
      adder.cellString(2,i,fate_inventory[n]);
      adder.cellString(1,i,"rename");
    }
    if (doom_inventory.find(n)!=doom_inventory.end()) {
      adder.cellString(1,i,"drop");
    }
    map<string,ColumnHistory>::iterator it = history.find(n);
    if (it!=history.end()) {
      adder.cellString(3,i,it->second.from_table.c_str());
      adder.cellString(4,i,it->second.from_name.c_str());
    }
  }

  return true;
}



bool FoldTool::unfold(coopy::store::PolyBook& src,
		      coopy::store::PolyBook& dest,
		      FoldOptions& options) {
  PolySheet mapping = src.readSheet("mapping");
  if (!mapping.isValid()) {
    mapping = options.recipe.readSheet("mapping");
  }
  if (!mapping.isValid()) {
    fprintf(stderr,"Need a sheet/table called 'mapping'\n");
    return false;
  }

  vector<string> names = src.getNames();
  PolySheet sheet;
  string sheet_name;
  for (int i=0; i<(int)names.size(); i++) {
    if (names[i]!="mapping") {
      sheet_name = names[i];
      sheet = src.readSheetByIndex(i);
      break;
    }
  }
  if (!sheet.isValid()) {
    fprintf(stderr,"No sheet to operate on\n");
    return false;
  }

  Property p;
  p.put("sheet",sheet_name.c_str());
  printf("Getting ready to copy...\n");
  dest.copy(src,p);
  printf("Copied.\n");

  PolySheet out = dest.readSheet(sheet_name.c_str());
  if (!out.isValid()) {
    fprintf(stderr,"Cannot open output\n");
    return false;
  }

  int at = 0;
  for (int i=0; i<mapping.height(); i++) {
    string name = mapping.cellString(0,i);
    string fate = mapping.cellString(1,i);
    string prev = mapping.cellString(2,i);
    printf("Operating on %s / %s / %s\n", name.c_str(),
	   fate.c_str(), prev.c_str());
    if (fate=="drop") {
      out.insertColumn(ColumnRef(at),ColumnInfo(name));
      //schema->insertColumn(ColumnRef(at),name.c_str());
      at++;
    } else if (fate=="rename") {
      out.modifyColumn(ColumnRef(at),ColumnInfo(name));
      at++;
    } else {
      at++;
    }
  }

  return true;
}
