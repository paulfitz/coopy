
#include <coopy/FileIO.h>

#include <string.h>

using namespace coopy::store;
using namespace std;

bool FileIO::openForWrite(const char *dest, const Property& config) {
  if (strcmp(dest,"-")==0) {
    fp = stdout;
    need_close = false;
  } else {
    fp = fopen(dest,"wb");
    if (fp!=NULL) {
      need_close = true;
    }
  }
  return fp!=NULL;
}

bool FileIO::open(const char *src, const Property& config) {
  //printf("OPEN with %s\n", config.toString().c_str());
  if (strcmp(src,"-")==0) {
    fp = stdin;
    need_close = false;
  } else {
    fp = fopen(src,"rb");
    if (fp!=NULL) {
      need_close = true;
    }
  }
  if (fp!=NULL) {
    if (config.get("length").asString()=="header") {
      char buf[100] = "x";
      while (buf[0]!=':') {
	size_t r = fread(&buf[0],1,1);
	if (r==0) break;
      }
      while (buf[0]!=' ') {
	size_t r = fread(&buf[0],1,1);
	if (r==0) break;
      }
      int at = 0;
      char ch = 'x';
      do {
	size_t r = fread(&buf[at],1,1);
	if (r==0) break;
	ch = buf[at];
	at++;
      } while (ch>='0'&&ch<='9'&&at<=10);
      buf[at] = '\0';
      at = atoi(buf);
      dbg_printf("Got length %d\n", at);
      {
	size_t r = fread(&buf[0],1,1);
	if (buf[0]!=ch) {
	  r = fread(&buf[0],2,2);
	}
      }
      
      has_length = true;
      pending_length = at;
    }
  }

  return fp!=NULL;
}

bool FileIO::close() {
  if (need_close) {
    fclose(fp);
    need_close = false;
  }
  fp = NULL;
  return true;
}


size_t FileIO::fread(void *ptr, size_t size, size_t nmemb) {
  if (fp==NULL) return 0;
  if (!has_length) {
    return ::fread(ptr,size,nmemb,fp);
  }
  if (pending_length==0) return 0;
  size_t top = size*nmemb;
  if (pending_length<top) top = pending_length;
  size_t r = ::fread(ptr,1,top,fp);
  if (r == 0) {
    pending_length = 0;
  }
  pending_length -= r;
  return r;
}


bool FileIO::openAndWrite(const std::string& txt, const Property& config) {
  string name = config.get("file").asString();
  if (name=="-") {
    fp = stdout;
    need_close = false;
  } else {
    fp = fopen(name.c_str(),"wb");
    if (!fp) return false;
    need_close = true;
  }
  if (fp!=NULL) {
    fwrite(txt.c_str(),1,txt.length(),fp);
  }
  if (need_close) {
    fclose(fp);
  }
  fp = NULL;
  need_close = false;

  return true;
}
