#include <stdio.h>
#include <coopy/Viterbi.h>
#include <coopy/Dbg.h>

//#include <iostream>
//using namespace std;

//#define DBG_LEVEL 100
//#define DBG(x) if (DBG_LEVEL>=(x))

using namespace coopy::store;
using namespace coopy::cmp;

void Viterbi::setSize(int states, int sequence_length) {
  K = states;
  T = sequence_length;
  cost.resize(K,T);
  src.resize(K,T,-1);
  path.resize(1,T,-1);
  reset();
}


void Viterbi::assertMode(int n_mode) {
  switch (n_mode) {
    case 0:
      if (mode==1)
	{
	  index++;
	}
      mode = 0;
      break;
    case 1:
      if (mode==0)
	{
	  //assert(index<T);

	  // Zeroing is now implicit.
	  // The absence of a cell(x,y) should imply:
	  //   cost(x,y) = 0
	  //   src(x,y)  = -1
	  /*
	  for (int i=0; i<K; i++)
	    {
	      cost(i,index) = 0;
	      src(i,index) = -1;
	    }
	  */
	}
      mode = 1;
      break;
    default:
      mode = n_mode;
      break;
    }
}

void Viterbi::addTransition(int s0, int s1, float c) {
  bool resize = false;
  if (s0>=K) {
    K = s0+1;
    resize = true;
  }
  if (s1>=K) {
    K = s1+1;
    resize = true;
  }
  if (resize) {
    cost.nonDestructiveResize(K,T);
    src.nonDestructiveResize(K,T,-1);
    path.nonDestructiveResize(1,T,-1);
  }
  path_valid = 0;
  assertMode(1);
  if (index>=T) {
    T=index+1;
    //printf("RESIZE! index %d T %d\n", index, T);
    cost.nonDestructiveResize(K,T);
    src.nonDestructiveResize(K,T,-1);
    path.nonDestructiveResize(1,T,-1);
  }
  int sourced = 0;
  if (index>0)
    {
      c += cost(s0,index-1);
      sourced = (src(s0,index-1)!=-1);
    }
  else
    {
      sourced = 1;
    }
  
  if (sourced)
    {
      if (c<cost(s1,index)||src(s1,index)==-1)
	{
	  //cout << "Setting " << s1 << "," << index << " to "
	  //<< s0 << endl;
	  cost(s1,index) = c;
	  src(s1,index) = s0;
	}
      else
	{
	  //cout << "Rejecting " << s1 << "," << index << " to "
	  //<< s0 << endl;
	}
    }
}

void Viterbi::calculatePath() {
  if (!path_valid)
    {
      endTransitions();
      float best = 0;
      int bestj = -1;
      if (index<=0) {
	// declare victory and exit
	path_valid = 1;
	return;
      }
      for (int j=0; j<K; j++)
	{
	  //DBG(50) cout << "j=" << j << " and bestj is " << bestj 
	  //	       << " and src(j,index-1)=" << src(j,index-2)
	  //	       << endl;
	  if ((cost(j,index-1)<best||bestj==-1)&&src(j,index-1)!=-1)
	    {
	      best = cost(j,index-1);
	      bestj = j;
	    }
	}
      best_cost = best;

      for (int i=index-1; i>=0; i--)
	{
	  path(0,i) = bestj;
	  //DBG(50) cout << "i=" << i << " and bestj is " << bestj << endl;
	  COOPY_ASSERT(bestj!=-1);
	  COOPY_ASSERT(bestj>=0&&bestj<K);
	  bestj = src(bestj,i);
	}
      path_valid = 1;
    }
}

void Viterbi::showPath() {
  calculatePath();
  for (int i=0; i<index; i++)
    {
      if (path(0,i)==-1)
	{
	  printf("*");
	}
      else
	{
	  printf("%d",path(0,i));
	}
      if (K>=10)
	{
	  printf(" ");
	}
    }
  printf(" costs %g\n", getCost());
}


