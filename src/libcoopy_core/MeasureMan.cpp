#include <coopy/MeasureMan.h>
#include <coopy/Viterbi.h>

using namespace coopy::store;
using namespace coopy::cmp;

using namespace std;

#include <math.h>

#define dbg2_printf if(coopy_is_verbose()) printf

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
  bool flip = match.width()<match.height();

  IntSheet asel;
  IntSheet bsel;
  DataSheet& a = flip?main_pass.b:main_pass.a;
  DataSheet& b = flip?main_pass.a:main_pass.b;
  int match_width = flip?match.height():match.width();
  int match_height = flip?match.width():match.height();

  bool repeatNeeded = false;
  map<int,int> stateNode, stateNodePrev;
  int stateNodeCount = 0;
  int stateNodePower = 1;

  do {
    asel = flip?main_pass.bsel:main_pass.asel;
    bsel = flip?main_pass.asel:main_pass.bsel;

    repeatNeeded = false;

    Viterbi v;
    /*
      There are match.width() = W nodes

      stride = W+1

      state interpretation:
      0: an unmatching state
      x from 1 to W: "matches against remote column x-1, no preexisting-nodes" 
                  (or lingering without match)
    */
    int column_stride = match_width+1;
    int history_stride = stateNodePower;
    vector<int> history_offset;
    for (int i=0; i<match_width; i++) {
      if (stateNode.find(i)!=stateNode.end()) {
	history_offset.push_back(stateNode[i]);
      } else {
	history_offset.push_back(0);
      }
    }
    v.setSize(column_stride*history_stride,match_height);
    for (int i=0; i<match_height; i++) {
      const set<int>& idx0 = flip?match.getCellsInCol(i-1):match.getCellsInRow(i-1);
      const set<int>& idx1 = flip?match.getCellsInCol(i):match.getCellsInRow(i);
      const set<int> *pidx0 = &idx0;
      v.beginTransitions();
      for (int k=0; k<history_stride; k++) {
	int ksrc = k*column_stride;
	for (set<int>::const_iterator it1=idx1.begin(); it1!=idx1.end(); 
	     it1++) {
	  int i1 = (*it1);
	  int offset = history_offset[i1];
	  if (offset&k) continue; // cannot transition here - was there already
	  int kdest = (k+offset)*column_stride;
	  
	  float c = costify(flip?match.cell(i,i1):match.cell(i1,i));
	  double mod = 0.1*log(1+fabs((double)(i-i1)))/log(2);
	  for (set<int>::const_iterator it0=pidx0->begin(); it0!=pidx0->end(); 
	       it0++) {
	    int i0 = (*it0);
	    if (i0!=i1) {
	      int offset0 = 0;
	      //int offset0 = history_offset[i0];
	      //if (offset0&k) continue;
	      v.addTransition(ksrc+i0+1,
			      kdest+offset0*column_stride+i1+1,c+mod);
	    }
	    //printf("%d: %d to %d (diff %g) costs %g\n", i, i0+1, i1+1, mod, c+mod);
	  }
	  v.addTransition(ksrc+0,kdest+i1+1,c);
	}
	for (set<int>::const_iterator it0=pidx0->begin(); it0!=pidx0->end(); 
	     it0++) {
	  int i0 = (*it0);
	  int offset0 = 0;
	  //int offset0 = history_offset[i0];
	  //if (offset0&k) continue;
	  v.addTransition(ksrc+i0+1,ksrc+offset0*column_stride+0,0);
	}
	v.addTransition(ksrc+0,ksrc+0,0);
      }
      v.endTransitions();
    }

    if (_csv_verbose) {
      dbg_printf("Viterbi calculation: ");
      for (int i=0; i<v.length(); i++) {
	int idx = v(i)%column_stride;
	int k = v(i)/column_stride;
	if (k>0) {
	  dbg_printf("%d:",k);
	}
	if (idx>0) {
	  dbg_printf("%d ",idx-1);
	} else {
	  dbg_printf(". ");
	}
      }
      dbg_printf(" (cost %g)\n", v.getCost());
      //v.showPath();
    }

    stateNodePrev = stateNode;
    for (int y=0; y<match_height; y++) {
      if (bsel.cell(0,y)==-1) {
	int bestIndex = v(y)%column_stride-1;
	double bestValue = 0;
	if (bestIndex>=0) {
	  bestValue = flip?match.cell(y,bestIndex):match.cell(bestIndex,y);
	}
	double ref = bnorm_pass.match.cell(0,flip?bestIndex:y);
	double ref2 = anorm_pass.match.cell(0,flip?y:bestIndex);
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
	    dbg2_printf("COLLISION! Unavailable match: %d (%d,%d)\n",
			bestIndex, y, asel.cell(0,bestIndex));
	    // want to say    asel[bestIndex] = y
	    // but currently  asel[bestIndex] = y'

	    if (stateNode.find(bestIndex)==stateNode.end()) {
	      stateNode[bestIndex] = stateNodePower;
	      stateNodeCount++;
	      stateNodePower *= 2;

	      if (stateNodeCount<4) {
		repeatNeeded = true;
	      } else {
		//fprintf(stderr,"Warning: there's a lot of ambiguity\n");
		repeatNeeded = false;
	      }
	    } else {
	      if (stateNodePrev.find(bestIndex)!=stateNodePrev.end()) {
		dbg_printf("Catastrophe in MeasureMan\n");
		//exit(1);
	      }
	    }

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
    if (repeatNeeded) {
      dbg_printf("====================================\n");
      dbg_printf("====================================\n");
      dbg_printf("====================================\n");
      dbg_printf("====================================\n");
      dbg_printf("Repeating with more state\n");
    }
  } while (repeatNeeded);
  main_pass.asel = flip?bsel:asel;
  main_pass.bsel = flip?asel:bsel;
}
