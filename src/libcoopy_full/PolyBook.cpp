#include <coopy/PolyBook.h>
#include <coopy/ShortTextBook.h>
#include <coopy/CsvTextBook.h>
#include <coopy/CsvFile.h>
#include <coopy/FormatSniffer.h>
#include <coopy/Dbg.h>
#include <coopy/TextBookFactory.h>
#include <coopy/ShortTextBookFactory.h>
#include <coopy/FormatDesc.h>

#include <stdlib.h>
#include <stdio.h>

#include <map>
#include <fstream>

using namespace std;
using namespace coopy::store;
using namespace coopy::format;

extern TextBook *readHelper(const char *fname,
			    const char *ext,
			    const char *data);

extern bool readHelperJson(coopy::store::Property& config,
			   const char *filename);

extern void getFactories(vector<TextBookFactory *>& lst);
extern void getFactoriesList(vector<FormatDesc>& descs);

class Factories {
public:
  vector<TextBookFactory *> all;

  Factories() {
    all.push_back(new ShortTextBookFactory);
    all.push_back(CsvTextBookFactory::makeFactory());
    all.push_back(CsvTextBookFactory::makeCompactFactory());
    getFactories(all);
  }

  ~Factories() {
    for (int i=0; i<(int)all.size(); i++) {
      if (all[i]!=NULL) {
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
	dbg_printf("Factories::open operating on %s (prev book: |%ld|%s)\n", key.c_str(),
		   (long int)config.prevBook,
		   (config.prevBook!=NULL)?config.prevBook->desc().c_str():"");
	TextBook *result = all[i]->open(config,report);
	dbg_printf("success [%s] msg [%s]\n", report.success?"ok":"fail",
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

bool PolyBook::expand(Property& config) {
  string filename = config.get("file").asString();
  string name = filename;
  string ext = config.get("ext").asString();
  if (ext=="") {
    size_t eid = name.rfind(".");
    if (eid!=string::npos) {
      ext = name.substr(eid);
    }
  } else {
    if (ext[0]!='.') {
      ext = string(".") + ext;
    }
  }
  for (size_t i=0; i<ext.length(); i++) {
    ext[i] = tolower(ext[i]);
  }
  //dbg_printf("Attach: extension is %s\n", ext.c_str());

  if (filename.substr(0,4)!="dbi:") {
    if (ext == ".json") {
      dbg_printf("Asked to attach, with json configuration\n");
      if (!readHelperJson(config,filename.c_str())) {
	return false;
      }
      filename = config.get("file",PolyValue::makeString("-")).asString();
      ext = "";
      config.put("ext","");
    } else {
      config.put("ext",ext);
    }
  }

  if (filename.substr(0,4)=="dbi:") {
    string s = filename + ":";
    int first = 0;
    vector<string> words;
    char prev = '*';
    bool last = false;
    for (int i=0; i<(int)s.length(); i++) {
      char ch = s[i];
      if (ch==':'&&(i==s.length()-1||!last)) {
	if (prev==ch) {
	  last = true;
	}
	string word = s.substr(first,i-first);
	size_t div = word.find('=');
	if (div==string::npos) {
	  words.push_back(word);
	  //dbg_printf("dbi: part %s\n", word.c_str());
	} else {
	  string key = word.substr(0,div);
	  string val = word.substr(div+1,word.length());
	  //dbg_printf("dbi: %s->%s\n", key.c_str(), val.c_str());
	  if (key=="port") {
	    config.put(key.c_str(),atoi(val.c_str()));
	  } else {
	    config.put(key.c_str(),val);
	  }
	}
	first = i+1;
      }
      prev = ch;
    }
    if (words.size()>1) {
      config.put("type",words[1]);
    }
    if (words.size()>2) {
      config.put("database",words[2]);
    }
    if (words.size()>3) {
      config.put("host",words[3]);
    }
    if (words.size()>4) {
      config.put("port",atoi(words[4].c_str()));
    }
  }

  string key = config.get("type",PolyValue::makeString("")).asString();

  if (key=="") {
    if (ext==".csv"||ext==".ssv"||ext==".tsv"||ext==".txt") {
      key = "csv";
    }
    if (ext==".csvs") {
      key = "csvs";
    }
    if (ext==".xls"||ext==".xlsx"||ext==".gnumeric"||ext==".ods") {
      key = "gnumeric";
    }
    if (ext==".socialcalc"||ext==".sc") {
      key = "socialcalc";
    }
    if (ext==".jsonbook") {
      key = "jsonbook";
    }
    if (ext==".mdb") {
      key = "access";
    }
    if (ext==".sqlite") {
      key = "sqlite";
    }
    if (ext==".sqlitext") {
      key = "sqlitext";
    }
    if (ext==".book") {
      key = "book";
    }
  }
  if (key==""&&filename=="-") {
    key = "csvs";
  }

  if (key=="") {
    //printf("SNIFFING %s\n",filename.c_str());
    FormatSniffer sniffer;
    sniffer.open(filename.c_str(),false);
    Format f = sniffer.getFormat();
    switch (f.id) {
    case FORMAT_BOOK_SQLITE:
      key = "sqlite";
      break;
    case FORMAT_BOOK_CSVS:
      key = "csvs";
      break;
    }
  }

  dbg_printf("FILE: type [%s] file [%s] settings: %s\n", key.c_str(), filename.c_str(), config.toString().c_str());

  if (config.get("file").asString()=="-"&&config.check("only_if_exists")) {
    config.put("skip",true);
  }

  if (key=="") {
    fprintf(stderr,"* Extension %s not known, maybe use a .json config file?\n",
	    ext.c_str());
    return false;
  }
  config.put("type",key);
  config.put("expanded_filename",filename.c_str());
  config.put("expanded_ext",ext.c_str());

  return true;
}

bool PolyBook::attach(Property& config, PolyBook *base) {
  dbg_printf("PolyBook::attach %s\n", config.toString().c_str());
  if (config.check("should_clear")) {
    if (config.get("should_clear").asBoolean()) {
      clear();
    }
  }

  bool ok = expand(config);
  if (!ok) return false;

  string filename = config.get("expanded_filename").asString();
  string ext = config.get("expanded_ext").asString();
  
  Factories f;
  AttachConfig ac;
  AttachReport ar;
  ac.fname = config.get("file").asString();
  if (ac.fname=="") ac.fname = filename;
  ac.ext = ext;
  ac.data = "";
  ac.options = config;
  ac.canCreate = config.get("can_create").asBoolean();
  ac.canOverwrite = true;
  ac.shouldRead = config.get("should_read").asBoolean();
  ac.shouldWrite = config.get("should_write").asBoolean();
  if (config.check("skip")) {
    ac.shouldRead = false;
    ac.shouldWrite = false;
  }
  ac.prevBook = book;
  ac.baseBook = base;
  ac.prevOptions = options;
  TextBook *nextBook = f.open(ac,ar);
  if (nextBook!=NULL) {
    if (book!=NULL) {
      if (nextBook!=book) {
	clear();
      }
    }
  }
  if (nextBook!=NULL && nextBook!=book) {
    nextBook->addReference();
  }
  book = nextBook;
  options = config;
  return book!=NULL;
}

bool PolyBook::flush() {
  if (!inplace()) {
    options.put("can_create",true);
    options.put("should_read",false);
    options.put("should_write",true);
    return attach(options);
  }
  return true;
}


#define STRVAL PolyValue::makeString

std::vector<FormatDesc> PolyBook::getFormatList() {

  vector<FormatDesc> descs;

  FormatDesc csv("CSV: plain-text delimiter-separated family of formats");
  csv.addExtension(".csv","Comma-separated values");
  csv.addExtension(".tsv","Tab-separated values");
  csv.addExtension(".ssv","Semicolon-separated values");
  csv.addOption("type",STRVAL("csv"),"CSV family",true);
  csv.addOption("file",STRVAL("fname.dsv"),"File name",true);
  csv.addOption("delimiter",STRVAL("|"),"Delimiter character",true);
  descs.push_back(csv);

  getFactoriesList(descs);
  
  return descs;
}

void PolyBook::showFormats() {
  vector<FormatDesc> descs = getFormatList();
  
  printf("Supported formats\n");
  printf("-----------------\n\n");
  for (int i=0; i<(int)descs.size(); i++) {
    descs[i].show();
  }

  printf("Tips:\n");
  printf("*  Most spreadsheet/database formats support a 'table' option to restrict\n");
  printf("   input/output to a named table or tables\n");
  printf("*  json options and dbi options are interchangeable\n");
}


bool PolyBook::copyFile(const char *src, const char *dest) {
  PolyBook bsrc;
  if (!bsrc.read(src)) {
    fprintf(stderr,"Failed to read %s\n", src);
    return false;
  }
  if (!bsrc.write(dest)) {
    fprintf(stderr,"Failed to write %s\n", dest);
    return false;
  }
  return true;
}


bool PolyBook::copy(const TextBook& alt, const Property& options) {
  if (!book)
    book = new CsvTextBook(true);
  if (book)
    return book->copy(alt,options);
  return false;
}
