#include <coopy/FormatSniffer.h>
#include <coopy/Dbg.h>

#include <stdio.h>
#include <string.h>

using namespace std;
using namespace coopy::format;
using namespace coopy::store;

bool FormatSniffer::setString(const char *str) {
  close();
  cache = str;
  return true;
}

bool FormatSniffer::wrap(FileIO& fin, bool caching) {
  char buf[caching?32768:100];
  cache = "";
  size_t bytes_read;
  if (caching) {
    while ((bytes_read=fin.fread(buf,1,sizeof(buf)))>0) {
      cache.append(buf,bytes_read);
    }
  } else {
    bytes_read = fin.fread(buf,1,sizeof(buf));
    cache.append(buf,bytes_read);
  }
  return true;
}

bool FormatSniffer::open(const char *fname, bool caching) {
  close();
  dbg_printf("Looking at %s\n", fname);
  Property p;
  if (!fio.open(fname,p)) {
    fprintf(stderr,"FormatSniffer: could not open %s\n", fname);
    return false;
  }
  wrap(fio,caching);
  return true;
}


bool FormatSniffer::close() {
  fio.close();
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
  } else if (cache.substr(0,7)=="# tdiff") {
    Format f;
    f.id = FORMAT_PATCH_TDIFF;
    f.name = "coopy_patch_tdiff";
    return f;
  }

  if (cache.substr(0,15)=="SQLite format 3") {
    Format f;
    f.id = FORMAT_BOOK_SQLITE;
    f.name = "sqlite";
    return f;
  }

  if (cache.substr(0,4)==" == "||cache.substr(0,3)=="== ") {
    Format f;
    f.id = FORMAT_BOOK_CSVS;
    f.name = "csvs";
    return f;
  }

  if (cache.substr(0,15).find("|")!=string::npos) {
    Format f;
    f.id = FORMAT_PATCH_TDIFF;
    f.name = "coopy_patch_tdiff";
    return f;
  }

  if (cache.find("@@@ ")!=string::npos) {
    Format f;
    f.id = FORMAT_PATCH_TDIFF;
    f.name = "coopy_patch_tdiff";
    return f;
  }

  return Format();
}

