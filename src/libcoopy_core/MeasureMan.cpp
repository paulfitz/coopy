#include <coopy/MeasureMan.h>
#include <coopy/Viterbi.h>

using namespace coopy::store;
using namespace coopy::cmp;

using namespace std;

#include <math.h>

#define dbg2_printf if(0) printf

static float costify(float x) {
  //if (x<0.01) x = 0.01;
  //return 1/x;
  return -x;
}

void MeasureMan::setup() {
  main.setup(main_pass);
  anorm.setup(anorm_pass);
  bnorm.setup(bnorm_pass);
}

void MeasureMan::compare() {
  compareCore();
  if (coopy_is_verbose()) {
    if (main_pass.bsel.height()!=main_pass.asel.height()) {
      for (int j=0; j<main_pass.bsel.height(); j++) {
	int i = main_pass.bsel.cell(0,j);
	if (i>=0) {
	  dbg_printf("%d -> %d ", i, j);
	  for (int x=0; x<4&&x<main_pass.a.width(); x++) {
	    if (i<main_pass.a.height()) {
	      dbg_printf("%s ", main_pass.a.cellString(x,i).c_str());
	    }
	  }
	  dbg_printf("// ");
	  for (int x=0; x<4&&x<main_pass.b.width(); x++) {
	    if (j<main_pass.b.height()) {
	      dbg_printf("%s ", main_pass.b.cellString(x,j).c_str());
	    }
	  }
	  dbg_printf("\n");
	}
      }
    }
  }
}

void MeasureMan::compareCore() {
  int rem = -1;
  int ctrl = 0;
  int ctrlMax = main.getCtrlMax();
  bool allOrNothing = (flags.trust_ids && rowLike);

  int remaining = 0;
  for (int j=0; j<main_pass.bsel.height(); j++) {
    if (main_pass.bsel.cell(0,j)==-1) {
      remaining++;
    }
  }
  if (remaining==0) {
    dbg_printf("MeasureMan::compare finished\n");
    return;
  }

  for (int i=0; i<20; i++) {
    dbg_printf("\n====================================================\n");
    dbg_printf("== MeasureMan::compare pass %d\n", i);
    compare1(ctrl);
    int processed = 0;
    remaining = 0;
    for (int j=0; j<main_pass.bsel.height(); j++) {
      if (main_pass.bsel.cell(0,j)==-1) {
	remaining++;
      } else {
	processed++;
      }
    }
    if (remaining == 0) {
      dbg_printf("Everything allocated\n");
      break;
    }
    dbg_printf("Not everything allocated, %d processed, %d remain (a-total %d b-total %d)\n",
	       processed,
	       remaining,
	       main_pass.asel.height(),
	       main_pass.bsel.height());
    if (remaining<=(main_pass.bsel.height()-main_pass.asel.height())) {
      dbg_printf("No more could be allocated\n");
      break;
    }
    if (processed>remaining&&sampled) {
      dbg_printf("Good enough, sampling only, quitting\n");
      break;
    }

    if (rem==remaining) {
      dbg_printf("No progress\n");
      if (ctrl<ctrlMax) {
	dbg_printf("Increasing desperation\n");
	if (allOrNothing) {
	  dbg_printf(" ... but we've been told to trust ids\n");
	  break;
	}
	ctrl++;
      } else {
	break;
      }
    }
    rem = remaining;
  }
}


void MeasureMan::compare1(int ctrl) {
  Stat astat, bstat;
  dbg_printf("== Control level: %d, checking means...\n",ctrl);  
  main.measure(main_pass,ctrl);

  anorm_pass.asel = main_pass.asel;
  anorm_pass.bsel = main_pass.asel;
  anorm.measure(anorm_pass,ctrl);
  //dbg_printf("Checking [local] statistics\n");
  astat = anorm_pass.flatten();
  bnorm_pass.asel = main_pass.bsel;
  bnorm_pass.bsel = main_pass.bsel;
  bnorm.measure(bnorm_pass,ctrl);
  //dbg_printf("Checking [remote] statistics\n");
  bstat = bnorm_pass.flatten();

  if (bstat.valid && astat.valid) {
    // make bnorm look like anorm, statistically
    if (bstat.stddev>0.001) {
      bnorm_pass.match.offset(-bstat.mean);
      bnorm_pass.match.rescale(astat.stddev/bstat.stddev);
      bnorm_pass.match.offset(+astat.mean);
    }
  }
    
  SparseFloatSheet match = main_pass.match;
  IntSheet& asel = main_pass.asel;
  IntSheet& bsel = main_pass.bsel;
  DataSheet& a = main_pass.a;
  DataSheet& b = main_pass.b;

  Viterbi v;
  /*
    state interpretation:
       x: "matches against remote column x-1" (or lingering without match)
       0: an unmatching state
   */
  v.setSize(match.width()+1,match.height());
  for (int i=0; i<match.height(); i++) {
    const set<int>& idx0 = match.getCellsInRow(i-1);
    const set<int>& idx1 = match.getCellsInRow(i);
    const set<int> *pidx0 = &idx0;
    v.beginTransitions();
    for (set<int>::const_iterator it1=idx1.begin(); it1!=idx1.end(); it1++) {
      int i1 = (*it1);
      float c = costify(match.cell(i1,i));
      for (set<int>::const_iterator it0=pidx0->begin(); it0!=pidx0->end(); 
	   it0++) {
	int i0 = (*it0);
	double mod = 0.1*log(1+fabs((double)(i-i1)))/log(2);
	if (i0!=i1) {
	  v.addTransition(i0+1,i1+1,c+mod);
	}
	//printf("%d: %d to %d (diff %g) costs %g\n", i, i0+1, i1+1, mod, c+mod);
      }
      v.addTransition(0,i1+1,c);
    }
    for (set<int>::const_iterator it0=pidx0->begin(); it0!=pidx0->end(); 
	 it0++) {
      int i0 = (*it0);
      v.addTransition(i0+1,0,0);
    }
    v.addTransition(0,0,0);
    v.endTransitions();
  }

  if (_csv_verbose) {
    dbg_printf("Viterbi calculation:\n");
    v.showPath();
  }


  for (int y=0; y<match.height(); y++) {
    if (bsel.cell(0,y)==-1) {
      int bestIndex = v(y)-1;
      double bestValue = 0;
      if (bestIndex>=0) {
	bestValue = match.cell(bestIndex,y);
      }
      double ref = bnorm_pass.match.cell(0,y);
      double ref2 = anorm_pass.match.cell(0,bestIndex);
      if (ref2<ref) ref = ref2;
      bool ok = false;
      if (bestValue>ref/4 && bestIndex>=0 && ref>0.01) {
	ok = true;
      }
      if (!ok) {
	if (bestIndex>=0 && y>=0) {
	  // let's examine the best match in detail
	  dbg2_printf("Detail check... %d->%d\n",y,bestIndex);
	  if (cellLength(a)==cellLength(b)) {
	    int misses = 0;
	    for (int kk=0; kk<cellLength(a); kk++) {
	      if (cell(a,kk,bestIndex)!=cell(b,kk,y)) {
		misses++;
	      }
	    }
	    dbg2_printf("Detailed examination gives %d misses for best match\n", 
			misses);
	    if (misses==0) {
	      // perfect match, let it through
	      ok = true;
	    }
	  }
	}
      }
      if (ok) {
       
	dbg2_printf("%d->%d, remote unit %d maps to local unit %d (%d %g : %g)\n",
		   y,bestIndex,y,bestIndex,
		   bestIndex, bestValue, ref);
	dbg2_printf("  [remote/local] %s %s\n", cell(b,0,y).c_str(), cell(a,0,bestIndex).c_str());
	
	if (asel.cell(0,bestIndex)!=-1 && asel.cell(0,bestIndex)!=y) {
	  dbg2_printf("COLLISION! Ignoring unavailable match\n");
	  dbg2_printf("This case has not been optimized\n");
	} else {
	  bsel.cell(0,y) = bestIndex;
	  asel.cell(0,bestIndex) = y;
	}
      }
      if (!ok) {
	dbg2_printf("%d->?, do not know what to make of remote unit %d (%d %g %g : %g)\n",
		   y, y, bestIndex, bestValue, -1.0, ref);
	dbg2_printf("  [remote] %s\n", cell(b,0,y).c_str());
	if (bestIndex>=0) {
	  dbg2_printf("  [local] [MISS] %s\n", cell(a,0,bestIndex).c_str());
	}
      }
    }
  }

  /*
  IntSheet bestIndices;
  FloatSheet bestValues, bestIncs;
  dbg_printf("Find best\n");
  match.findBest(bestIndices, bestValues, bestIncs);
  dbg_printf("Done find best\n");

  for (int y=0; y<match.height(); y++) {
    if (bsel.cell(0,y)==-1) {
      int bestIndex = bestIndices.cell(0,y);
      double bestValue = bestValues.cell(0,y);
      double bestInc = bestIncs.cell(0,y);
      double ref = bnorm_pass.match.cell(0,y);
      bool ok = false;
      if (bestValue>ref/4) {
	if (bestInc>bestValue/2 && bestIndex>=0) {
	  ok = true;
	}
      }
      if (!ok) {
	if (bestIndex>=0 && y>=0) {
	  // let's examine the best match in detail
	  dbg_printf("Detail check... %d->%d\n",y,bestIndex);
	  if (cellLength(a)==cellLength(b)) {
	    int misses = 0;
	    for (int kk=0; kk<cellLength(a); kk++) {
	      if (cell(a,kk,bestIndex)!=cell(b,kk,y)) {
		misses++;
	      }
	    }
	    dbg_printf("Detailed examination gives %d misses for best match\n", 
		       misses);
	    if (misses==0) {
	      // perfect match, let it through
	      ok = true;
	    }
	  }
	}
      }
      if (ok) {
	dbg_printf("%d->%d, remote unit %d maps to local unit %d (%d %g %g : %g)\n",
		   y,bestIndex,y,bestIndex,
		   bestIndex, bestValue, bestInc, ref);
	dbg_printf("  [remote/local] %s %s\n", cell(b,0,y).c_str(), cell(a,0,bestIndex).c_str());
	if (asel.cell(0,bestIndex)!=-1 && asel.cell(0,bestIndex)!=y) {
	  dbg_printf("COLLISION! Ignoring unavailable match\n");
	  dbg_printf("This case has not been optimized\n");
	} else {
	  bsel.cell(0,y) = bestIndex;
	  asel.cell(0,bestIndex) = y;
	}
      }
      if (!ok) {
	dbg_printf("%d->?, do not know what to make of remote unit %d (%d %g %g : %g)\n",
		   y, y, bestIndex, bestValue, bestInc, ref);
	dbg_printf("  [remote] %s\n", cell(b,0,y).c_str());
	if (bestIndex>=0) {
	  dbg_printf("  [local] [MISS] %s\n", cell(a,0,bestIndex).c_str());
	}
      }
    }
  }
  */
  //dbg_printf("Done in MeasureMan::compare1\n");
}
