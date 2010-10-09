#include <coopy/Viterbi.h>

using namespace coopy::store;
using namespace coopy::cmp;

int main(int argc, char *argv[]) {
  Viterbi v;
  v.setSize(10,100);
  v.beginTransitions();
  v.addTransition(0,0,1);
  v.endTransitions();
  for (int i=0; i<20; i++) {
    v.beginTransitions();
    v.addTransition(0,1,1);
    v.addTransition(1,0,1);
    v.endTransitions();
  }
  int out = 0;
  for (int i=0; i<20; i++) {
    if (v(i)!=i%2) {
      out++;
    }
  }
  v.showPath();
  if (out!=0) {
    printf("Path is incorrect\n");
  }
  return (out==0)?0:1;
}
