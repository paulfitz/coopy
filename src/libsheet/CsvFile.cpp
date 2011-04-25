extern "C" { 
#include "csv.h"
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>

#include <coopy/CsvFile.h>
#include <coopy/CsvSheet.h>

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

  CsvSheetReaderState() {
    reader = NULL;
    sheet = NULL;
    expecting = true;
    ignore = false;
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

  bool addSheet(const char *name) {
    if (reader != NULL) {
      sheet = reader->nextSheet(name);
    }
    return sheet!=NULL;
  }

  void markBreak() {
    if (sheet!=NULL) {
      SimpleSheetSchema *schema = new SimpleSheetSchema;
      COOPY_ASSERT(schema);
      Poly<SheetSchema> pschema(schema,true);
      schema->setHeaderHeight(sheet->height()-1);
      sheet->setSchema(pschema);
    }
  }
};


extern "C" void csvfile_merge_cb1 (void *s, size_t i, void *p, int quoted) {
  CsvSheetReaderState *state = (CsvSheetReaderState*)p;
  CsvSheet *sheet = state->sheet;
  if (state->expecting && state->reader!=NULL) {
    char *str = (char *)s;
    if (i>4 && !quoted) {
      if (str[0]=='=' && str[1]=='=' && str[2]==' ') {
	size_t j;
	for (j=i-1; j>=3; j--) {
	  if (str[j]==' '&&str[j+1]=='=') break;
	}
	str[j] = '\0';
	char *name = str+3;
	//printf("Name is perhaps [%s]\n", name);
	state->addSheet(name);
	state->ignore = true;
	return;
      }
      if (str[0]=='-' && str[1]=='-' && str[2]=='-') {
	state->markBreak();
	state->ignore = true;
	return;
      }
    }
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


int read(const char *src, CsvSheetReaderState& dest, 
	 const Property& config) {
  FILE *fp;
  char buf[32768];
  size_t bytes_read;
  struct csv_parser p;
  dest.clear();
  if (csv_init(&p,0)!=0) {
    fprintf(stderr,"csv failed to initialize\n");
    exit(1);
  }
  SheetStyle style;
  style.setFromFilename(src);
  style.setFromProperty(config);
  dest.setStyle(style);
  csv_set_delim(&p,style.getDelimiter()[0]);

  bool need_close = true;
  if (strcmp(src,"-")==0) {
    fp = stdin;
    need_close = false;
  } else {
    fp = fopen(src,"rb");
  }
  if (!fp) {
    fprintf(stderr,"CsvRead: could not open %s\n", src);
    return 1;
  }

  if (fp==stdin) {
    string pre;
    while ((bytes_read=fread(buf,1,sizeof(buf),fp))>0) {
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
  } else {
    while ((bytes_read=fread(buf,1,sizeof(buf),fp))>0) {
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
  }
  csv_fini(&p,
	   csvfile_merge_cb1,
	   csvfile_merge_cb2,
	   (void*)(&dest));
  if (need_close) {
    fclose(fp);
  }
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
  return read(src,state,config);
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
  return read(src,state,config);  
}

