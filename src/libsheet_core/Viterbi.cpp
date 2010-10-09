#include <stdio.h>
#include <coopy/Viterbi.h>

#define DBG_LEVEL 0
#define DBG(x) if (DBG_LEVEL>=(x))

using namespace coopy::store;
using namespace coopy::cmp;

void Viterbi::setSize(int states, int sequence_length) {
  K = states;
  T = sequence_length;
  cost.resize(K,T);
  src.resize(K,T);
  path.resize(1,T);
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
	  assert(index<T);
	  //cout << "Reset for index=" << index << endl;
	  for (int i=0; i<K; i++)
	    {
	      cost(i,index) = 0;
	      src(i,index) = -1;
	    }
	}
      mode = 1;
      break;
    default:
      mode = n_mode;
      break;
    }
}

void Viterbi::addTransition(int s0, int s1, float c) {
  path_valid = 0;
  assertMode(1);
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
	  //   << s0 << endl;
	  cost(s1,index) = c;
	  src(s1,index) = s0;
	}
      else
	{
	  //cout << "Rejecting " << s1 << "," << index << " to "
	  //   << s0 << endl;
	}
    }
}

void Viterbi::calculatePath() {
  if (!path_valid)
    {
      endTransitions();
      float best = 0;
      int bestj = -1;
      assert(index>0);
      for (int j=0; j<K; j++)
	{
//	  DBG(50) cout << "j=" << j << " and bestj is " << bestj 
//		       << " and src(j,index-1)=" << src(j,index-2)
//		       << endl;
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
//	  DBG(50) cout << "i=" << i << " and bestj is " << bestj << endl;
	  assert(bestj!=-1);
	  assert(bestj>=0&&bestj<K);
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


