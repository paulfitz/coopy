#include <coopy/PolyBook.h>
#include <coopy/ShortTextBook.h>
#include <coopy/SqliteTextBook.h>
#include <coopy/CsvTextBook.h>
#include <coopy/CsvFile.h>
#include <coopy/FormatSniffer.h>
#include <coopy/Dbg.h>
#include <coopy/JsonProperty.h>
#include <coopy/TextBookFactory.h>
#include <coopy/ShortTextBookFactory.h>
#include <coopy/SqliteTextBook.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include <map>
#include <fstream>

#include <json/json.h>

using namespace std;
using namespace coopy::store;
using namespace coopy::format;

static bool exists(const char *fname) {
  struct stat s;
  int result = stat(fname,&s);
  return (result==0);
}

extern TextBook *readHelper(const char *fname,
			    const char *ext,
			    const char *data);

extern void getFactories(vector<TextBookFactory *>& lst);

class Factories {
public:
  vector<TextBookFactory *> all;

  Factories() {
    all.push_back(new ShortTextBookFactory);
    all.push_back(new SqliteTextBookFactory);
    getFactories(all);
  }

  ~Factories() {
    for (int i=0; i<(int)all.size(); i++) {
      if (all[i]==NULL) {
	delete all[i];
      }
    }
  }

  TextBook *open(AttachConfig& config, AttachReport& report) {
    string key = config.options.get("type").asString();
    for (int i=0; i<(int)all.size(); i++) {
      if (all[i]==NULL) {
	fprintf(stderr,"Failed to allocate a factory\n");
	return NULL;
      }
      if (all[i]->getName()==key) {
	dbg_printf("operating on %s\n", key.c_str());
	TextBook *result = all[i]->open(config,report);
	dbg_printf("success %d msg %s\n", (int)report.success,
		   report.msg.c_str());
	if (!report.success) {
	  fprintf(stderr,"* %s%s%s\n", 
		  key.c_str(),
		  (report.msg!="")?": ":" operation failed.",
		  report.msg.c_str());
	}
	return result;
      }
    }
    fprintf(stderr,"* Book type not known (%s)\n", key.c_str());
    return NULL;
  }
};

/*
bool PolyBook::read(const char *fname) {
  clear();
  string name = fname;
  if (name.length()>=4) {
    string ext = name.substr(name.rfind('.'),name.length());
    for (size_t i=0; i<ext.length(); i++) {
      ext[i] = tolower(ext[i]);
    }
    dbg_printf("Extension %s\n", ext.c_str());
    if (ext==".book") {
      dbg_printf("Trying %s out as CsvTextBook\n", ext.c_str());
      CsvTextBook *book0 = new CsvTextBook();
      if (!book0->read(fname)) {
	delete book0;
      } else {
	book = book0;
      }
      if (book!=NULL) return true;
    }
    book = readHelper(fname,ext.c_str(),NULL);
    if (book!=NULL) return true;
    FormatSniffer sniffer;
    sniffer.open(fname);
    Format f = sniffer.getFormat();
    if (f.id==FORMAT_BOOK_SQLITE) {
      dbg_printf("Trying %s out as Sqlite\n", ext.c_str());
      SqliteTextBook *book0 = new SqliteTextBook();
      if (!book0->read(fname)) {
	delete book0;
      } else {
	book = book0;
      }
    }
    if (book==NULL) {
      SheetStyle style;
      if (style.setFromFilename(fname)) {
	dbg_printf("Trying %s out as CSV\n", ext.c_str());
	ShortTextBook *b = new ShortTextBook();
	if (b==NULL) {
	  fprintf(stderr,"Failed to allocate ShortTextBook\n");
	  exit(1);
	}
	if (CsvFile::read(fname,b->sheet)!=0) {
	  delete b;
	  b = NULL;
	} else {
	  book = b;
	}
      }
    }
  }
  return book!=NULL;
}
*/

class Namer {
public:
  map<string,int> existing;
  string name(string input) {
    string sane = "";
    for (size_t i=0; i<input.length(); i++) {
      char ch = input[i];
      if (ch=='"') {
	sane += '"';
      }
      sane += ch;
    }
    if (existing.find(sane)!=existing.end()) {
      char buf[256];
      int at = 0;
      do {
	sprintf(buf,"%d",at);
	at++;
      } while (existing.find(sane+buf)==existing.end());
      sane += buf;
    }
    existing[sane] = 1;
    return sane;
  }
};

class Valuer {
public:
  string name(string input) {
    string sane = "";
    for (size_t i=0; i<input.length(); i++) {
      char ch = input[i];
      if (ch=='\'') {
	sane += '\'';
      }
      sane += ch;
    }
    return sane;
  }
};

/*
bool PolyBook::write(const char *fname, const char *format) {
  string name = fname;
  size_t eid = name.rfind(".");
  string ext = ".csv";
  if (eid!=string::npos) {
    ext = name.substr(eid);
  }
  for (size_t i=0; i<ext.length(); i++) {
    ext[i] = tolower(ext[i]);
  }
  dbg_printf("Write: extension is %s\n", ext.c_str());
  if (book==NULL) {
    fprintf(stderr,"Nothing to write\n");
    return false;
  }
  vector<string> names = getNames();
  if (ext == ".json") {
    dbg_printf("Asked to write, with json configuration\n");
    ifstream in(fname);
    Property p;
    if (!JsonProperty::add(p,fname)) {
      return false;
    }
    string key = p.get("type",PolyValue::makeString("none")).asString();
    string fname2 = p.get("file",PolyValue::makeString("-")).asString();
    if (key=="csv") {
      if (names.size()!=1) {
	fprintf(stderr,"Unsupported number of sheets during write: %d\n",
		(int)names.size());
	return false;
      }
      PolySheet sheet = readSheet(names[0]);
      if (!sheet.isValid()) { 
	fprintf(stderr,"Could not access sheet %s\n", names[0].c_str());
	return false;
      }
      return CsvFile::write(sheet,p)==0;
    } else {
     fprintf(stderr,"Output type not recognized: %s\n", key.c_str());
     return false;
    }
  }
  if (book->save(fname,format)) {
    return true;
  }
  if (format!=NULL) {
    if (string(format)!=""&&string(format)!="-") {
      fprintf(stderr,"Setting output format is not yet supported\n");
      exit(1);
    }
  }
  if (ext==".sql") {
    FILE *fout = fopen(fname,"w");
    if (fout==NULL) {
      fprintf(stderr,"Could not open %s for writing\n", fname);
      return false;
    }
    for (size_t i=0; i<names.size(); i++) {
      PolySheet sheet = readSheet(names[i]);
      if (!sheet.isValid()) { 
	fprintf(stderr,"Could not access sheet %s\n", names[i].c_str());
	return false;
      }
      printf("Pumping out %s\n", names[i].c_str());
      Namer namer;
      Namer namer2;
      Valuer val;
      string table = namer.name(names[i]);
      fprintf(fout,"CREATE TABLE \"%s\" (\n", table.c_str());
      if (sheet.height()>=1) {
	for (int x=0; x<sheet.width(); x++) {
	  fprintf(fout,"   \"%s\"%s\n", namer2.name(sheet.cellString(x,0)).c_str(),
		  (x==sheet.width()-1)?"":",");
	}
      }
      fprintf(fout,");\n");
      for (int y=1; y<sheet.height(); y++) {
	fprintf(fout, "INSERT INTO \"%s\" VALUES ( ", table.c_str());
	for (int x=0; x<sheet.width(); x++) {
	  fprintf(fout,"\'%s\'%s ", val.name(sheet.cellString(x,y)).c_str(),
		  (x==sheet.width()-1)?"":",");
	}
	fprintf(fout, ");\n");
      }
    }
    fclose(fout);
    return true;
  }

  if (names.size()!=1) {
    fprintf(stderr,"Unsupported number of sheets during write: %d\n",
	    (int)names.size());
    return false;
  }
  PolySheet sheet = readSheet(names[0]);
  if (!sheet.isValid()) { 
    fprintf(stderr,"Could not access sheet %s\n", names[0].c_str());
    return false;
  }
  return CsvFile::write(sheet,fname)==0;
}

*/

bool PolyBook::attach(Property& config) {
  if (config.check("should_clear")) {
    if (config.get("should_clear").asBoolean()) {
      clear();
    }
  }
  string filename = config.get("file").asString();
  string name = filename;
  size_t eid = name.rfind(".");
  string ext = ".csv";
  if (eid!=string::npos) {
    ext = name.substr(eid);
  }
  for (size_t i=0; i<ext.length(); i++) {
    ext[i] = tolower(ext[i]);
  }
  dbg_printf("Attach: extension is %s\n", ext.c_str());

  if (ext == ".json") {
    dbg_printf("Asked to attach, with json configuration\n");
    if (!JsonProperty::add(config,filename)) {
      return false;
    }
    filename = config.get("file",PolyValue::makeString("-")).asString();
    ext = "";
    config.put("ext","");
  } else {
    config.put("ext",ext);
  }

  string key = config.get("type",PolyValue::makeString("")).asString();

  if (key=="") {
    if (ext==".csv"||ext==".ssv"||ext==".tsv"||ext==".txt") {
      key = "csv";
    }
    if (ext==".xls"||ext==".xlsx"||ext==".gnumeric") {
      key = "gnumeric";
    }
    if (ext==".sqlite") {
      key = "sqlite";
    }
  }
  if (key==""&&filename=="-") {
    key = "csv";
  }

  dbg_printf("Attach: type %s file %s\n", key.c_str(), filename.c_str());

  if (key=="") {
    fprintf(stderr,"* Extension %s not known, maybe use a .json config file?\n",
	    ext.c_str());
    return false;
  }
  config.put("type",key);

  Factories f;
  AttachConfig ac;
  AttachReport ar;
  ac.fname = filename;
  ac.ext = ext;
  ac.data = "";
  ac.options = config;
  ac.canCreate = config.get("can_create").asBoolean();
  ac.canOverwrite = true;
  ac.shouldRead = config.get("should_read").asBoolean();
  ac.shouldWrite = config.get("should_write").asBoolean();
  ac.prevBook = book;
  ac.prevOptions = options;
  TextBook *nextBook = f.open(ac,ar);
  if (nextBook!=NULL) {
    if (book!=NULL) {
      if (nextBook!=book) {
	clear();
      }
    }
  }
  book = nextBook;
  options = config;
  return book!=NULL;
}

/*
bool PolyBook::load() {
  if (filename=="") {
    fprintf(stderr, "No file to read.\n");
    return false;
  }
  if (!exists(filename)) {
    fprintf(stderr, "Cannot read %s\n", filename.c_str());
    return false;
  }
  return false;
}

bool PolyBook::save() {
  if (inplace) {
    return true;
  }
  if (filename=="") {
    fprintf(stderr,"No filename to save to.\n");
    return false;
  }
  return false;
}
*/
