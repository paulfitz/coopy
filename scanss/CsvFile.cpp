#include "CsvFile.h"

extern "C" { 
#include "csv.h"
}

#include <stdio.h>
#include <stdlib.h>

extern "C" void csvfile_merge_cb1 (void *s, size_t i, void *p) {
  ((CsvSheet*)p)->addField((char *)s, i);
}

extern "C" void csvfile_merge_cb2 (int c, void *p) {
  ((CsvSheet*)p)->addRecord();
}


int CsvFile::read(const char *src, CsvSheet& dest) {
  FILE *fp;
  char buf[32768];
  size_t bytes_read;
  struct csv_parser p;
  if (csv_init(&p,0)!=0) {
    fprintf(stderr,"csv failed to initialize\n");
    exit(1);
  }
  fp = fopen(src,"rb");
  if (!fp) {
    fprintf(stderr,"could not open %s\n", src);
    exit(1);
  }
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
  csv_fini(&p,
	   csvfile_merge_cb1,
	   csvfile_merge_cb2,
	   (void*)(&dest));
  fclose(fp);
  csv_free(&p);
  return 0;
}

