#include <coopy/JackTextBook.h>
#include <coopy/JackSheet.h>

#include <gcj/cni.h>
#include <java/lang/System.h>
#include <java/io/PrintStream.h>
#include <java/lang/Throwable.h>
#include <JackBox.h>

#include <stdio.h>

#define WANT_VECTOR2STRING
#include <coopy/Stringer.h>

#include <algorithm>

using namespace std;
using namespace coopy::store;
using namespace coopy::store::mdb;
using namespace java::lang;
using namespace java::util;
using namespace com::healthmarketscience::jackcess;
 
#define JS(x) JvNewStringLatin1(x)

int JackTextBook::uses = 0;

static JackBox *box = NULL;


#define DB(x) ((Database *)(x))

JackTextBook::JackTextBook() {
  implementation = NULL;
  uses++;
  if (uses==1) {
    JvCreateJavaVM(NULL);
    JvAttachCurrentThread(NULL, NULL);
    box = new JackBox();
    COOPY_MEMORY(box);
  }
}

JackTextBook::~JackTextBook() {
  clear();
  uses--;
  if (uses==0) {
    box = NULL; // Java destroys this
    JvDetachCurrentThread();
  }
}

void JackTextBook::clear() {
  if (implementation!=NULL) { 
    //mdb_close(DB(implementation));
    implementation = NULL;
  }
}

bool JackTextBook::read(const char *fname) {
  clear();
  Database *mdb = box->read(JS(fname));
  if (!mdb) {
    fprintf(stderr,"Failed to open database %s\n", fname);
    return false;
  }
  implementation = (void *)mdb;
  names = getNamesImpl();
  return true;
}

bool JackTextBook::open(const Property& config) {
  clear();
  if (!config.check("file")) {
    fprintf(stderr,"file parameter needed\n");
    return false;
  }
  if (!read(config.get("file").asString().c_str())) {
    fprintf(stderr,"failed to read %s\n", config.get("file").asString().c_str());
    return false;
  }
  this->config = config;
  if (!config.check("table")) {
    dbg_printf("Loaded access workbook\n");
    return true;
  }
  names.clear();
  names.push_back(config.get("table").asString().c_str());
  dbg_printf("Loaded access workbook, table: %s\n",
	     vector2string(names).c_str());
  return true;
}


vector<string> JackTextBook::getNamesImpl() {
  std::vector<std::string> tables;
  List *names = box->getTableNames(DB(implementation));
  if (!names) return tables;
  int len = box->getCount(names);
  for (int i=0; i<len; i++) {
    String *str = box->getElement(names,i);
    if (!str) continue;
    int slen = JvGetStringUTFLength(str);
    string str2(slen,'\0');
    JvGetStringUTFRegion(str,0,slen,(char*)str2.c_str());
    //printf("NAME %s\n",str2.c_str());
    tables.push_back(str2);
  }
  return tables;
}

PolySheet JackTextBook::readSheet(const std::string& name) {
  //printf("Trying to find %s, names %s\n", name.c_str(),
  //vector2string(names).c_str());
  getNames();
  if (find(names.begin(),names.end(),name)==names.end()) {
    return PolySheet();
  }
  //printf("Trying to make %s\n", name.c_str());
  JackSheet *sheet = new JackSheet(implementation,box,name.c_str(),config);
  if (sheet!=NULL) sheet->connect();
  return PolySheet(sheet,true);
}

bool JackTextBook::addSheet(const SheetSchema& schema) {
  return false;
}

