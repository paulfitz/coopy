
#include "CsvColumn.h"

using namespace std;

#define FMAX(x,y) (((x)>(y))?(x):(y))

void Nature::evaluate(const char *txt) {
  string s = txt;
  size_t i;

  // How URL-like are we?

  int webby = 0;

  i=s.find("http://");
  if (i!=string::npos) {
    if (i<=1) {
      webby += 100;
    } else {
      webby += 1;
    }
  }
  i=s.find("://");
  if (i!=string::npos) {
    if (i<=10) {
      webby += 5;
    } else {
      webby += 1;
    }
  }
  i=s.find("www.");
  if (i==string::npos) {
    i=s.find(".com");
  }
  if (i==string::npos) {
    i=s.find(".net");
  }
  if (i==string::npos) {
    i=s.find(".org");
  }
  if (i==string::npos) {
    i=s.find(".coop");
  }
  if (i!=string::npos) {
    webby += 1;
  }
  if (webby>=1) {
    web.vote(1,webby/10.0);
  } else {
    web.vote(-1,0.1);
  }

  // How email-like are we?
  int maily = 0;
  i=s.find("@");
  if (i!=string::npos) {
    i=s.find(".");
    if (i!=string::npos) {
      maily = 1;
    }
  }
  if (maily>=1) {
    email.vote(1,1);
  } else {
    email.vote(-1,0.1);
  }

  // How number-like are we?
  int numbery = 0;
  for (int j=0; j<s.length(); j++) {
    char ch = s[j];
    if (ch>='0'&&ch<='9') {
      numbery++;
    } else {
      if (ch!='.'&&ch!='-'&&ch!='e'&&ch!='E') {
	numbery--;
      }
    }
  }
  if (numbery>0) {
    number.vote(1,numbery/4.0);
  } else {
    number.vote(-1,0.1);
  }

  text.vote(1,0.1);

  /*
  printf("Results: web %g email %g text %g number %g\n",
	 web.result(),
	 email.result(),
	 text.result(),
	 number.result());
  */
}

float Nature::compare(const char *txt) {
  Nature n;
  n.evaluate(txt);
  float dot = n.web.result()*web.result() +
    n.email.result()*email.result() +
    n.text.result()*text.result() +
    n.number.result()*number.result();
  return dot;
}


float Nature::confidence() {
  float c = FMAX(FMAX(web.confidence,email.confidence),
		 FMAX(text.confidence,number.confidence));
  return c;
}


void CsvColumn::evaluate() {
  nmean.clear();
  for (int i=0; i<sheet->height(); i++) {
    nmean.evaluate(sheet->cell(index,i).c_str());
  }
  /*
  if (nmean.confidence()>0.3) {
    for (int i=0; i<sheet->height(); i++) {
      Nature nmean;
    }
  } else {
    printf("No confidence\n");
  }
  */
}

