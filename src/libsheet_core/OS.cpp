#include <coopy/OS.h>

#include <stdio.h>
#include <stdlib.h>

using namespace coopy::os;

std::string OS::getTemporaryFilename() {
  // replace with better method!
  std::string fname = tempnam(NULL,"_cpy_");
  //fprintf(stderr,"Created %s\n", fname.c_str());
  return fname;
}


bool OS::deleteFile(const std::string& name) {
  int result = unlink(name.c_str());
  if (result!=0) {
    fprintf(stderr,"Problem deleting %s\n", name.c_str());
  }
  //fprintf(stderr,"Deleted %s\n", name.c_str());
  return result==0;
}
