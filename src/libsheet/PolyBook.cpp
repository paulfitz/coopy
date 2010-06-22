#include <coopy/PolyBook.h>
#include <coopy/ShortTextBook.h>
#include <coopy/CsvFile.h>

#include <stdlib.h>
#include <stdio.h>

#include <map>

using namespace std;

bool PolyBook::read(const char *fname) {
  clear();
  string name = fname;
  if (name.length()>=4) {
    string ext = name.substr(name.length()-4);
    for (size_t i=0; i<ext.length(); i++) {
      ext[i] = tolower(ext[i]);
    }
    if (ext==".db") {
      // try to load an sqlite book
    }
    if (book==NULL) {
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
  return book!=NULL;
}


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

bool PolyBook::write(const char *fname) {
  if (book==NULL) {
    fprintf(stderr,"Nothing to write\n");
    return false;
  }
  vector<string> names = getNames();
  string name = fname;
  string ext = name.substr(name.rfind("."));
  for (size_t i=0; i<ext.length(); i++) {
    ext[i] = tolower(ext[i]);
  }
  if (ext==".sql") {
    CsvSheet sheet;
    FILE *fout = fopen(fname,"w");
    if (fout==NULL) {
      fprintf(stderr,"Could not open %s for writing\n", fname);
      return false;
    }
    for (size_t i=0; i<names.size(); i++) {
      bool ok = readSheet(names[i],sheet);
      if (!ok) { 
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
	  fprintf(fout,"   \"%s\"%s\n", namer2.name(sheet.cell(x,0)).c_str(),
		  (x==sheet.width()-1)?"":",");
	}
      }
      fprintf(fout,");\n");
      for (int y=1; y<sheet.height(); y++) {
	fprintf(fout, "INSERT INTO \"%s\" VALUES ( ", table.c_str());
	for (int x=0; x<sheet.width(); x++) {
	  fprintf(fout,"\'%s\'%s ", val.name(sheet.cell(x,y)).c_str(),
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
	    names.size());
    return false;
  }
  CsvSheet sheet;
  bool ok = readSheet(names[0],sheet);
  if (!ok) { 
    fprintf(stderr,"Could not access sheet %s\n", names[0].c_str());
    return false;
  }
  return CsvFile::write(sheet,fname)==0;
}

