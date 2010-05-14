#ifndef COOPY_MEASUREMAN
#define COOPY_MEASUREMAN

#include <coopy/Measure.h>

class MeasureMan {
public:
  Measure& main;
  Measure& anorm;
  Measure& bnorm;
  MeasurePass& main_pass;
  MeasurePass& anorm_pass;
  MeasurePass& bnorm_pass;
  bool rowLike;

  MeasureMan(Measure& main,
	     MeasurePass& main_pass,
	     Measure& anorm,
	     MeasurePass& anorm_pass,
	     Measure& bnorm,
	     MeasurePass& bnorm_pass,
	     bool rowLike) : main(main), 
			     main_pass(main_pass),
			     anorm(anorm), 
			     anorm_pass(anorm_pass),
			     bnorm(bnorm),
			     bnorm_pass(bnorm_pass),
			     rowLike(rowLike) // just for debugging
  {
  }
	     
  void compare() {
    main.setup(main_pass);
    anorm.setup(anorm_pass);
    bnorm.setup(bnorm_pass);

    int rem = -1;
    int ctrl = 0;
    int ctrlMax = main.getCtrlMax();
    for (int i=0; i<20; i++) {
      dbg_printf("\n\nPass %d\n", i);
      compare1(ctrl);
      //comparePass(a,b);
      int processed = 0;
      int remaining = 0;
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

  int cellLength(CsvSheet& a) {
    if (rowLike) {
      return a.width();
    }
    return a.height();
  }

  string cell(CsvSheet& a, int x, int y) {
    if (rowLike) {
      return a.cell(x,y);
    }
    return a.cell(y,x);
  }

  void compare1(int ctrl) {
    Stat astat, bstat;
    //feat.apply(a,b,asel,bsel);
    main.measure(main_pass,ctrl);
    //norm1.apply(a,a,asel,asel);
    anorm_pass.asel = main_pass.asel;
    anorm_pass.bsel = main_pass.asel;
    anorm.measure(anorm_pass,ctrl);
    dbg_printf("Checking [local] statistics\n");
    astat = anorm_pass.flatten();
    bnorm_pass.asel = main_pass.bsel;
    bnorm_pass.bsel = main_pass.bsel;
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
    }
    //CsvFile::write(feat.rowMatch,"match.csv");
    //CsvFile::write(norm1.rowMatch,"norm1.csv");
    //CsvFile::write(norm2.rowMatch,"norm2.csv");
    
    FloatSheet match = main_pass.match;
    IntSheet& asel = main_pass.asel;
    IntSheet& bsel = main_pass.bsel;
    CsvSheet& a = main_pass.a;
    CsvSheet& b = main_pass.b;
    for (int y=0; y<match.height(); y++) {
      if (bsel.cell(0,y)==-1) {
	int bestIndex = -1;
	double bestValue = 0;
	double bestInc = 0;
	for (int x=0; x<match.width(); x++) {
	  double val = match.cell(x,y);
	  if (val>bestValue) {
	    bestIndex = x;
	    bestInc = val - bestValue;
	    bestValue = val;
	  }
	}
	double ref = bnorm_pass.match.cell(0,y);
	bool ok = false;
	if (bestValue>ref/4) {// ||
	  //	    (bestValue>(bestValue-bestInc)*10 && bestValue>ref/8)) {
	  if (bestInc>bestValue/2 && bestIndex>=0) {
	    ok = true;
	  }
	}
	if (!ok) {
	  if (bestIndex>=0 && y>=0) {
	    // let's examine the best match in detail
	    printf("Detail check... %d->%d\n",y,bestIndex);
	    if (cellLength(a)==cellLength(b)) {
	      int misses = 0;
	      for (int kk=0; kk<cellLength(a); kk++) {
		if (cell(a,kk,bestIndex)!=cell(b,kk,y)) {
		  misses++;
		}
	      }
	      printf("Detailed examination gives %d misses for best match\n", 
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
	  dbg_printf("  [remote] %s\n", cell(b,0,y).c_str());
	  dbg_printf("  [local] %s\n", cell(a,0,bestIndex).c_str());
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
  }

};



#endif

