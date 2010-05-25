#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <string>

#include <coopy/CsvPatch.h>
#include <coopy/IntSheet.h>

using namespace std;

class Patcher {
private:
  string out_of_range;
public:
  vector<string> address;
  vector<string> action;
  vector<string> compare;
  IntSheet rows_o2r;
  CsvSheet orig;
  
  void apply(CsvSheet& original, CsvSheet& patch, CsvSheet& result);
  
  vector<string> extract(CsvSheet& sheet, int row, int col_start = 0,
			 int col_end = -1);
  
  
  string& safe_cell(CsvSheet& sheet, int x, int y) { 
    if (x<0 || y<0 || x>=sheet.width() || y>=sheet.height()) {
      out_of_range = "__OUT_OF_RANGE__";
      return out_of_range;
    }
    return sheet.cell(x,y);
  }

  int row_o2r(int o) {
    if (o<orig.height() && o>=0) {
      return rows_o2r.cell(0,o);
    }
    return -1;
  }
};

vector<string> Patcher::extract(CsvSheet& sheet, int row, int col_start,
				int col_end) {
  if (col_end==-1) { col_end = sheet.width()-1; }
  vector<string> out;
  for (int i=col_start; i<=col_end; i++) {
    out.push_back(sheet.cell(i,row));
  }
  return out;
}

void Patcher::apply(CsvSheet& original, CsvSheet& patch, CsvSheet& result) {
  if (patch.width()<1) {
    // nothing to do
    return;
  }
  result = original;
  orig = original;
  rows_o2r.resize(3,original.height(),0);
  for (int i=0; i<original.height(); i++) {
    rows_o2r.cell(0,i) = i;
  }

  for (int i=0; i<patch.height(); i++) {
    string cmd = patch.cell(0,i);
    if (cmd == "[for]") {
      address = extract(patch,i,1,-1);
      action.clear();
      for (int j=0; j<(int)address.size(); j++) {
	if (atoi(address[j].c_str())!=j) {
	  fprintf(stderr,"[for] column setup is not yet supported\n");
	  exit(1);
	}
      }
      if ((int)address.size()!=original.width()+1) {
	fprintf(stderr,"[for] column setup is not yet supported\n");
	exit(1);
      }
    } else if (cmd == "[do]") {
      action = extract(patch,i,1,-1);
      if (action.size()!=address.size()) {
	fprintf(stderr,"[do] does not match a [for], line %d\n", i);
	exit(1);
      }
      for (int j=0; j<(int)action.size(); j++) {
	string expected = (j==0)?"select":"";
	if (action[j]!=expected) {
	  fprintf(stderr,"[do] action \"%s\" not yet supported in this position; expected \"%s\"\n",
		  action[j].c_str(), expected.c_str());
	  exit(1);
	}
      }
    } else if (cmd == "[+++]") {
      vector<string> target = extract(patch,i,1,-1);
      if (target.size()!=action.size()) {
	fprintf(stderr,"[+++] does not match a [do], line %d\n", i);
	exit(1);
      }
      string placement = target[0];
      int row = -1;
      int off = -1;
      for (int j=0; j<(int)placement.length(); j++) {
	if (placement[j] == '+') {
	  placement[j] = '\0';
	  row = atoi(placement.c_str());
	  off = atoi(placement.c_str() + j + 1);
	}
      }
      //fprintf(stderr,"Insert at row %d off %d\n",row,off);
      int row_out = row_o2r(row-1) + off;
      result.insertRow(row_out);
      for (int j=row; j<rows_o2r.height(); j++) {
	rows_o2r.cell(0,j)++;
      }
      for (int j=1; j<(int)target.size(); j++) {
	result.cell(j-1,row_out) = target[j];
      }
    } else if (cmd == "[---]") {
      vector<string> target = extract(patch,i,1,-1);
      if (target.size()!=action.size()) {
	fprintf(stderr,"[---] does not match a [do], line %d\n", i);
	exit(1);
      }
      string placement = target[0];
      int row = -1;
      row = atoi(placement.c_str())-1;
      for (int j=1; j<(int)target.size(); j++) {
	if (safe_cell(original,j-1,row) != target[j]) {
	  fprintf(stderr,"[---] does not match row (%s vs %s), line %d\n", 
		  safe_cell(original,j-1,row).c_str(),
		  target[j].c_str(),
		  i);
	  exit(1);
	}
      }
      int row_out = row_o2r(row);
      result.removeRow(row_out);
      for (int j=row; j<rows_o2r.height(); j++) {
	rows_o2r.cell(0,j)--;
      }
    } else if (cmd == "[-]") {
      compare = extract(patch,i,1,-1);
      if (compare.size()!=action.size()) {
	fprintf(stderr,"[-] does not match a [do], line %d\n", i);
	exit(1);
      }
    } else if (cmd == "[+]") {
      vector<string> replace = extract(patch,i,1,-1);
      if (replace.size()!=compare.size()) {
	fprintf(stderr,"[+] does not match a [-], line %d\n", i);
      }
      int idx = atoi(replace[0].c_str());
      int idx2 = atoi(compare[0].c_str());
      if (idx!=idx2) {
	fprintf(stderr,"[+]/[-] mismatch, line %d\n", i);
	exit(1);
      }
      for (int j=1; j<(int)replace.size(); j++) {
	if (replace[j]!=compare[j]) {
	  if (compare[j] != safe_cell(original,j-1,idx-1)) {
	    //printf("Check %d %d\n", j-1, idx-1);
	    fprintf(stderr,"[-] mismatch with original (%s vs %s), line %d\n", 
		    compare[j].c_str(),
		    safe_cell(original,j-1,idx-1).c_str(),
		    i);
	    exit(1);
	  }
	  safe_cell(result,j-1,row_o2r(idx-1)) = replace[j];
	}
      }
      compare.clear();
    } else {
      fprintf(stderr,"Patch contains unsupported command %s\n", cmd.c_str());
      exit(1);
    }

    //printf("*** %d %%%\n", i);
    //printf("%s\n", result.encode().c_str());
  }
}


void CsvPatch::apply(CsvSheet& original, CsvSheet& patch) {
  Patcher patcher;
  patcher.apply(original,patch,result);
}
