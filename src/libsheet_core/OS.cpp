#include <coopy/OS.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern "C" {
#include "tmpdir.h"
#include "tempname.h"
}

using namespace coopy::os;

std::string OS::getTemporaryFilename() {
  char buf[10000];
  int r = path_search(buf,sizeof(buf),NULL,"_cpy_",true);
  if (r==0) {
    r = gen_tempname(buf,0,0,GT_FILE);
    //r = mkstemp(buf);
    if (r!=-1) close(r);
    //fprintf(stderr,"Created %s\n", buf);
    return buf;
  }
  return "";
}


bool OS::deleteFile(const std::string& name) {
  int result = unlink(name.c_str());
  if (result!=0) {
    fprintf(stderr,"Problem deleting %s\n", name.c_str());
  }
  //fprintf(stderr,"Deleted %s\n", name.c_str());
  return result==0;
}
