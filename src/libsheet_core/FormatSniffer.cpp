#include <coopy/FormatSniffer.h>
#include <coopy/Dbg.h>

#include <stdio.h>
#include <string.h>

using namespace std;
using namespace coopy::format;

bool FormatSniffer::open(const char *fname, bool caching) {
  close();
  FILE *fp;
  dbg_printf("Looking at %s\n", fname);
  if (strcmp(fname,"-")==0) {
    fp = stdin;
    need_close = false;
  } else {
    fp = fopen(fname,"rb");
    need_close = true;
  }
  if (!fp) {
    fprintf(stderr,"could not open %s\n", fname);
    need_close = false;
    return false;
  }

  char buf[caching?32768:100];
  cache = "";
  size_t bytes_read;
  if (caching) {
    while ((bytes_read=fread(buf,1,sizeof(buf),fp))>0) {
      cache.append(buf,bytes_read);
    }
  } else {
    fread(buf,1,sizeof(buf),fp);
  }
  return true;
}


bool FormatSniffer::close() {
  if (need_close) {
    if (impl!=NULL&&need_close) {
      fclose((FILE*)impl);
      impl = NULL;
    }
    need_close = false;
  }
  cache = "";
  return true;
}


Format FormatSniffer::getFormat() {
  if (cache.substr(0,4)=="dtbl"||cache.substr(0,4)=="\"dtbl") {
    size_t brk = cache.find('\n');
    string header = cache.substr(0,brk);
    bool csv = header.find("csv")!=string::npos;
    bool human = header.find("human")!=string::npos;
    //printf("HAVE brk at %d\n", (int) brk);
    //printf("HAVE string [%s]\n", cache.substr(0,10).c_str());
    Format f;
    if (csv) {
      f.id = FORMAT_PATCH_CSV;
      f.name = "coopy_patch_csv";
    }
    if (human) {
      f.id = FORMAT_PATCH_HUMAN;
      f.name = "coopy_patch_human";
    }
    return f;
  }

  if (cache.substr(0,15)=="SQLite format 3") {
    Format f;
    f.id = FORMAT_BOOK_SQLITE;
    f.name = "sqlite";
    return f;
  }

  if (cache.substr(0,4)==" == ") {
    Format f;
    f.id = FORMAT_BOOK_CSVS;
    f.name = "csvs";
    return f;
  }

  return Format();
}

