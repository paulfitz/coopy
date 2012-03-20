#include <coopy/Dbg.h>
#include <coopy/JackSheet.h>

#include <gcj/cni.h>
#include <JackBox.h>

using namespace coopy::store;
using namespace coopy::store::mdb;
using namespace std;
using namespace java::lang;
using namespace java::util;
using namespace com::healthmarketscience::jackcess;

#define Savepoint Cursor$Savepoint 

#define DB(x) ((Database *)(x))
#define TABLE(x) ((Table *)(x))
#define BOX(x) ((JackBox *)(x))
#define CURSOR(x) ((Cursor *)(x))

#define JS(x) JvNewStringLatin1(x)

JackSheet::JackSheet(void *db1, void *box,
		     const char *name, const Property& config) {
  this->box_implementation = box;
  implementation = db1;
  table_implementation = NULL;
  cursor_implementation = NULL;
  this->name = name;
  this->config = config;
  w = h = 0;

  schema = new JackSheetSchema;
  COOPY_MEMORY(schema);
  schema->sheet = this;
}

bool JackSheet::connect() {

  Database *mdb = DB(implementation);
  if (!mdb) return false;

  JackBox *box = BOX(box_implementation);
  if (!box) return false;

  Table *tbl = box->getTable(mdb,JS(name.c_str()));
  if (!tbl) return false;

  table_implementation = tbl;

  //////////////////////////////////////////////////////////////////
  // Check column names

  List *cols = box->getColumns(tbl);
  if (!cols) return false;

  w = box->getCount(cols);

  for (int j=0; j<w; j++) {
    String *str = box->getElement(cols,j);
    if (!str) return false;
    int slen = JvGetStringUTFLength(str);
    string str2(slen,'\0');
    JvGetStringUTFRegion(str,0,slen,(char*)str2.c_str());
    //printf("COL %s\n", str2.c_str());
    col2sql.push_back(str2);
    col2pk.push_back(false);
    col2type.push_back("TEXT");
  }

  //////////////////////////////////////////////////////////////////
  // Cache data - haven't figured out random access yet

  Cursor *cursor = box->createCursor(tbl);
  if (!cursor) return false;
  cursor_implementation = cursor;

  List *row = NULL;
  h = 0;
  cache.resize(w,h,"");
  cacheFlag.resize(w,h,0);
  do {
    row = box->getNextRow(cursor);
    if (row) {
      //printf("row found\n");
      h++;
      cache.resize(w,h,"");
      cacheFlag.resize(w,h,0);
      for (int i=0; i<w; i++) {
	String *str = box->getElement(row,i);
	if (!str) return false;
	int slen = JvGetStringUTFLength(str);
	string str2(slen,'\0');
	JvGetStringUTFRegion(str,0,slen,(char*)str2.c_str());
	cache.cell(i,h-1) = str2;
	//printf("Value %d,%d [%s]\n", i, h-1, str2.c_str());
	savepoint.push_back((void*)box->getSavepoint(cursor));
      }
    }
  } while (row);

  //printf("Final dimensions: %d %d\n", w, h);

  return true;
}

JackSheet::~JackSheet() {
  /*
  if (table_implementation!=NULL) {
    mdb_free_tabledef(TABLE(table_implementation));
    table_implementation = NULL;
  }
  */
  if (schema!=NULL) delete schema;
  schema = NULL;
}

SheetSchema *JackSheet::getSchema() const {
  return schema;
}


std::string JackSheet::cellString(int x, int y) const {
  bool tmp;
  return cellString(x,y,tmp);
}

std::string JackSheet::cellString(int x, int y, bool& escaped) const {
  escaped = false;
  const unsigned char *f = cacheFlag.pcell_const(x,y);
  if (f!=NULL) {
    escaped = true;
    return "NULL";
  }
  const string *c = cache.pcell_const(x,y);
  if (c!=NULL) {
    return *c;
  }

  escaped = true;
  return "";
}

bool JackSheet::cellString(int x, int y, const std::string& str, 
			   bool escaped) {
  if (escaped) {
    fprintf(stderr,"JackSheet cannot deal with nulls yet\n");
  }
  printf("SHOULD WRITE!\n");
  JackBox *box = BOX(box_implementation);
  Table *tbl = TABLE(table_implementation);
  Cursor *cursor = CURSOR(cursor_implementation);
  if (!(box&&cursor&&tbl)) return false;
  box->restoreSavepoint(cursor,(Savepoint *)savepoint[y]);
  printf("Found row!\n");
  box->setCurrentRowValue(tbl,cursor,JS(col2sql[x].c_str()),JS(str.c_str()));

  return false;
}
