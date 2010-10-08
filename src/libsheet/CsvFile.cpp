extern "C" { 
#include "csv.h"
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>

#include <coopy/CsvFile.h>
#include <coopy/CsvSheet.h>

using namespace std;
using namespace coopy::store;

extern "C" void csvfile_merge_cb1 (void *s, size_t i, void *p) {
  ((CsvSheet*)p)->addField((char *)s, i);
}

extern "C" void csvfile_merge_cb2 (int c, void *p) {
  ((CsvSheet*)p)->addRecord();
}

int CsvFile::read(coopy::format::Reader& reader, CsvSheet& dest) {
  string cache = "";
  struct csv_parser p;
  if (csv_init(&p,0)!=0) {
    fprintf(stderr,"csv failed to initialize\n");
    exit(1);
  }
  SheetStyle style;
  dest.clear();
  dest.setStyle(style);
  csv_set_delim(&p,style.getDelimiter()[0]);

  do {
    cache = reader.read();
    if (cache!="") {
      if (csv_parse(&p,
		    cache.c_str(),
		    cache.length(),
		    csvfile_merge_cb1,
		    csvfile_merge_cb2,
		    (void*)(&dest)) != cache.length()) {
	fprintf(stderr,"error parsing standard input\n");
	exit(1);
      }
    }
  } while (cache!="");
  csv_fini(&p,
	   csvfile_merge_cb1,
	   csvfile_merge_cb2,
	   (void*)(&dest));
  csv_free(&p);
  return 0;
}

int CsvFile::read(const char *src, CsvSheet& dest) {
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
    fprintf(stderr,"could not open %s\n", src);
    exit(1);
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
  return 0;
}

