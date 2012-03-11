extern "C" { 
#include "csv.h"
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>

#include <coopy/CsvRead.h>
#include <coopy/CsvSheet.h>
#include <coopy/Stringer.h>
#include <coopy/FileIO.h>

#include <string.h>

using namespace std;
using namespace coopy::store;

class CsvSheetReaderState {
public:
  CsvSheetReader *reader;
  CsvSheet *sheet;
  SheetStyle style;
  bool expecting;
  bool ignore;
  string name;
  bool named;
  bool broken;
  int headerLen;

  CsvSheetReaderState() {
    reader = NULL;
    sheet = NULL;
    expecting = true;
    ignore = false;
    name = "";
    named = false;
    broken = false;
    headerLen = 0;
  }

  void setStyle(const SheetStyle& style) {
    this->style = style;
    if (sheet!=NULL) {
      sheet->setStyle(style);
    }
  }

  void clear() {
    if (sheet!=NULL) {
      sheet->clear();
    }
  }

  bool addSheet(const char *name,bool named) {
    if (reader != NULL) {
      sheet = reader->nextSheet(name,named);
    }
    this->name = name;
    this->named = named;
    broken = false;
    return sheet!=NULL;
  }

  void markBreak() {
    broken = true;
    if (sheet!=NULL) {
      SimpleSheetSchema *schema = new SimpleSheetSchema;
      COOPY_ASSERT(schema);
      Poly<SheetSchema> pschema(schema,true);
      schema->setHeaderHeight(sheet->height()-1);
      for (int i=0; i<sheet->width(); i++) {
	string k = sheet->cellString(i,sheet->height()-1);

	/*
	bool pk = false;
	if (k.length()>1) {
	  if (k[k.length()-1]==':') {
	    if (k[k.length()-2]==':') {
	      pk = true;
	      k = k.substr(0,k.length()-2);
	      sheet->cellString(i,sheet->height()-1,k);
	    }
	  }
	}
	ColumnType kind;
	if (pk) {
	  kind.primaryKey = true;
	  kind.primaryKeySet = true;
	}
	schema->addColumn(k.c_str(),kind);
	*/

	schema->addColumn(k.c_str());
      }
      if (named) {
	schema->setSheetName(name.c_str());
      }
      sheet->setSchema(pschema);
    }
  }
};


extern "C" void csvfile_merge_cb1 (void *s, size_t i, void *p, int quoted) {
  CsvSheetReaderState *state = (CsvSheetReaderState*)p;
  CsvSheet *sheet = state->sheet;
  char *str = (char *)s;

  /*
  printf("Expecting? %d  Reader? %d  WORKING ON ",
	 state->expecting, state->reader!=NULL);
  for (int q=0; q<i; q++) {
    printf("%c", ((char*)s)[q]);
  }
  printf("\n");
  */

  if (state->expecting && state->reader!=NULL) {
    
    if (!quoted) {
      if (i>4) {
	if (str[0]=='=' && str[1]=='=' && str[2]==' ') {
	  size_t j;
	  for (j=i-1; j>=3; j--) {
	    if (str[j]==' '&&str[j+1]=='=') break;
	  }
	  str[j] = '\0';
	  char *name = str+3;
	  //printf("Name is perhaps [%s]\n", name);
	  state->addSheet(name,true);
	  state->ignore = true;
	  return;
	}
      }
      int q = 0;
      for (q=0; q<i; q++) {
	if (str[q]!='-'&&str[q]!='\n'&&str[q]!='\r') break;
      }
      if (q==i&&i>3&&state->named&&!state->broken) {
	state->markBreak();
	state->ignore = true;
	return;
      }
    }
  }
  if (sheet==NULL) {
    state->addSheet(coopy_get_default_table_name(),false);
    sheet = state->sheet;
  }
  if (sheet!=NULL) {
    state->expecting = false;
    if (sheet->getStyle().haveNullToken()) {
      string token = sheet->getStyle().getNullToken();
      if (token.length()==i){
	if (memcmp(s,token.c_str(),i)==0) {
	  sheet->addField((char *)s, i, true);
	  return;
	}
      }
      if (sheet->getStyle().quoteCollidingText()) {
	int score = 0;
	for (score=0; score<(int)i; score++) {
	  if (((char*)s)[score]!='_') {
	    break;
	  }
	}
	if (score>0) {
	  if (memcmp(((char*)s)+score,token.c_str(),i-score)==0) {
	    sheet->addField((char*)s+1,i-1,false);
	    return;
	  }
	}
      }
    }
    sheet->addField((char *)s, i, false);
  }
}

extern "C" void csvfile_merge_cb2 (int c, void *p) {
  CsvSheetReaderState *state = (CsvSheetReaderState*)p;
  CsvSheet *sheet = state->sheet;
  state->expecting = true;
  if (sheet==NULL) {
    return;
  }
  if (state->ignore) {
    state->ignore = false;
    return;
  }
  sheet->addRecord();
}

int CsvFile::read(coopy::format::Reader& reader, CsvSheet& dest, 
		  const Property& config) {
  string cache = "";
  struct csv_parser p;
  if (csv_init(&p,0)!=0) {
    fprintf(stderr,"csv failed to initialize\n");
    exit(1);
  }
  SheetStyle style;
  dest.clear();
  dest.setStyle(style);
  style.setFromProperty(config);
  csv_set_delim(&p,style.getDelimiter()[0]);

  CsvSheetReaderState state;
  state.sheet = &dest;

  do {
    cache = reader.read();
    if (cache!="") {
      if (csv_parse(&p,
		    cache.c_str(),
		    cache.length(),
		    csvfile_merge_cb1,
		    csvfile_merge_cb2,
		    (void*)(&state)) != cache.length()) {
	fprintf(stderr,"error parsing standard input\n");
	exit(1);
      }
    }
  } while (cache!="");
  csv_fini(&p,
	   csvfile_merge_cb1,
	   csvfile_merge_cb2,
	   (void*)(&state));
  csv_free(&p);
  return 0;
}


// len = -1: file
// len >= 0: in memory
static int read(const char *src, int len, CsvSheetReaderState& dest, 
	const Property& config) {
  bool fromFile = (len==-1);
  FileIO fp;
  char buf[32768];
  size_t bytes_read;
  struct csv_parser p;
  dest.clear();
  if (csv_init(&p,0)!=0) {
    fprintf(stderr,"csv failed to initialize\n");
    exit(1);
  }
  SheetStyle style;
  if (fromFile) {
    style.setFromFilename(src);
  }
  style.setFromProperty(config);
  dest.setStyle(style);
  csv_set_delim(&p,style.getDelimiter()[0]);

  bool need_close = true;

  if (fromFile) {
    if (!fp.open(src,config)) {
      fprintf(stderr,"CsvRead: could not open %s\n", src);
      return 1;
    }
  }

  if (fp.isPiped()) {
    string pre;
    while ((bytes_read=fp.fread(buf,1,sizeof(buf)))>0) {
      pre.append(buf,bytes_read);
    }
    SheetStyle style;
    style.setFromInspection(pre.c_str(),pre.length());
    csv_set_delim(&p,style.getDelimiter()[0]);
    dest.setStyle(style);
    if (csv_parse(&p,
		  pre.c_str(),
		  pre.length(),
		  csvfile_merge_cb1,
		  csvfile_merge_cb2,
		  (void*)(&dest)) != pre.length()) {
      fprintf(stderr,"error parsing standard input\n");
      exit(1);
    }
  } else if (fp.isValid()) {
    while ((bytes_read=fp.fread(buf,1,sizeof(buf)))>0) {
      if (csv_parse(&p,
		    buf,
		    bytes_read,
		    csvfile_merge_cb1,
		    csvfile_merge_cb2,
		    (void*)(&dest)) != bytes_read) {
	fprintf(stderr,"error parsing %s\n", src);
	exit(1);
      }
    }
  } else {
    // in memory
    if (csv_parse(&p,
		  src,
		  len,
		  csvfile_merge_cb1,
		  csvfile_merge_cb2,
		  (void*)(&dest)) != len) {
      fprintf(stderr,"error parsing %s\n", src);
      exit(1);
    }
  }
  csv_fini(&p,
	   csvfile_merge_cb1,
	   csvfile_merge_cb2,
	   (void*)(&dest));
  fp.close();
  csv_free(&p);

  if (config.get("flip_vertical").asInt()!=0) {
    CsvSheet *sheet = dest.sheet;
    for (int y=0; y<sheet->height()/2; y++) {
      int y2 = sheet->height()-1-y;
      for (int x=0; x<sheet->width(); x++) {
	SheetCell tmp = sheet->cellSummary(x,y);
	sheet->cellSummary(x,y,sheet->cellSummary(x,y2));
	sheet->cellSummary(x,y2,tmp);
      }
    }
  }

  return 0;
}

int CsvFile::read(const char *src, CsvSheet& dest, const Property& config) {
  dbg_printf("CsvFile::read %s options %s\n", src, config.toString().c_str());
  CsvSheetReaderState state;
  state.sheet = &dest;
  return read(src,-1,state,config);
  //return result;
}


int CsvFile::read(const char *src, CsvSheet& dest) {
  Property config;
  return read(src,dest,config);
}

int CsvFile::read(coopy::format::Reader& reader, CsvSheet& dest) {
  Property config;
  return read(reader,dest,config);
}


int CsvFile::read(const char *src, CsvSheetReader& dest, 
		  const Property& config) {
  CsvSheetReaderState state;
  state.reader = &dest;
  return read(src,-1,state,config);  
}

int CsvFile::read(const char *data, int len, CsvSheetReader& dest, 
		  const Property& config) {
  CsvSheetReaderState state;
  state.reader = &dest;
  if (len<0) return -1;
  return read(data,len,state,config);  
}
