#include <coopy/MeasureMan.h>

void MeasureMan::setup() {
  main.setup(main_pass);
  anorm.setup(anorm_pass);
  bnorm.setup(bnorm_pass);
}

void MeasureMan::compare() {
  int rem = -1;
  int ctrl = 0;
  int ctrlMax = main.getCtrlMax();

  int remaining = 0;
  for (int j=0; j<main_pass.bsel.height(); j++) {
    if (main_pass.bsel.cell(0,j)==-1) {
      remaining++;
    }
  }
  if (remaining==0) {
    dbg_printf("\n\nNothing to do\n");
    return;
  }

  for (int i=0; i<20; i++) {
    dbg_printf("\n\nPass %d\n", i);
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
    dbg_printf("Not everything allocated, %d remain (a-total %d b-total %d)\n",
	       remaining,
	       main_pass.asel.height(),
	       main_pass.bsel.height());
    if (remaining<=(main_pass.bsel.height()-main_pass.asel.height())) {
      dbg_printf("No more could be allocated\n");
      break;
    }
    if (rem==remaining) {
      dbg_printf("No progress\n");
      if (ctrl<ctrlMax) {
	dbg_printf("Increasing desperation\n");
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
  dbg_printf("MeasureMan::compare1(%d)\n",ctrl);  
  main.measure(main_pass,ctrl);
  anorm_pass.asel = main_pass.asel;
  anorm_pass.bsel = main_pass.asel;
  //anorm_pass.bound = 40;  // bounds do not work yet
  anorm.measure(anorm_pass,ctrl);
  dbg_printf("Checking [local] statistics\n");
  astat = anorm_pass.flatten();
  bnorm_pass.asel = main_pass.bsel;
  bnorm_pass.bsel = main_pass.bsel;
  //bnorm_pass.bound = 40;  // bounds do not work yet  
  bnorm.measure(bnorm_pass,ctrl);
  dbg_printf("Checking [remote] statistics\n");
  bstat = bnorm_pass.flatten();
  double scale = 1;
  if (bstat.valid && astat.valid) {
    if (bstat.mean>0.01) {
      scale = astat.mean/bstat.mean;
    }
    dbg_printf("Rescaling bnorm by %g\n", scale);
    bnorm_pass.match.rescale(scale);
    dbg_printf("Done rescaling\n");
  }
    
  SparseFloatSheet match = main_pass.match;
  IntSheet& asel = main_pass.asel;
  IntSheet& bsel = main_pass.bsel;
  TextSheet& a = main_pass.a;
  TextSheet& b = main_pass.b;

  IntSheet bestIndices;
  FloatSheet bestValues, bestIncs;
  dbg_printf("Find best\n");
  match.findBest(bestIndices, bestValues, bestIncs);
  dbg_printf("Done find best\n");

  for (int y=0; y<match.height(); y++) {
    //dbg_printf("at %d / %d\n", y, match.height());
    if (bsel.cell(0,y)==-1) {
      int bestIndex = bestIndices.cell(0,y);
      double bestValue = bestValues.cell(0,y);
      double bestInc = bestIncs.cell(0,y);
      /*
      int bestIndex = -1;
      double bestValue = 0;
      double bestInc = 0;
      for (int x=0; x<match.width(); x++) {
	const double& val = match.cell_const(x,y);
	if (val>bestValue) {
	  bestIndex = x;
	  bestInc = val - bestValue;
	  bestValue = val;
	}
      }
      */
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
  dbg_printf("Done in MeasureMan::compare1\n");
}
