#ifndef COOPY_STAT
#define COOPY_STAT

class Stat {
public:
  double mean;
  double stddev;
  bool valid;

  Stat() {
    mean = stddev = 0;
    valid = false;
  }

  bool isValid() { 
    return valid;
  }

  double evaluate(double x) {
    if (stddev<0.001) {
      return 1;
    }
    return (x-mean)/stddev;
  }

  double rescale(double factor) {
    mean *= factor;
    stddev *= factor;
    return mean;
  }
  
};


#endif

