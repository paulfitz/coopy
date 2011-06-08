
#include <coopy/PatchParser.h>
#include <coopy/CsvFile.h>
#include <coopy/Format.h>
#include <coopy/Dbg.h>
#include <coopy/PolyBook.h>

#include <stdio.h>

#include <string>
#include <vector>
#include <algorithm>

#define WANT_VECTOR2STRING
#include <coopy/Stringer.h>

using namespace coopy::cmp;
using namespace coopy::format;
using namespace coopy::store;
using namespace std;

class PatchColumnNames {
public:
  vector<string> lst;
  vector<SheetCell> data;
  vector<int> indices;

  void read(const vector<string>& vals, int offset) {
    for (int i=0; i<(int)vals.size(); i++) {
      lst.push_back(vals[i]);
      indices.push_back(i);
    }
  }

  void read(const DataSheet& sheet, int x, int y, int len = -1) {
    lst.clear();
    indices.clear();
    int last = sheet.width()-1;
    if (len>=0) {
      last = x+len-1;
    }
    for (int i=x; i<=last; i++) {
      string v = sheet.cellString(i,y);
      if (v==""&&len==-1) break;
      lst.push_back(v);
      indices.push_back(i-x);
    }
  }
  
  void readData(const DataSheet& sheet, int x, int y, int len) {
    data.clear();
    for (int i=x; i<x+len; i++) {
      SheetCell v = sheet.cellSummary(i,y);
      data.push_back(v);
    }
  }
  
  string toString() {
    return vector2string(lst);
  }

  string dataString() {
    return vector2string(data);
  }

  string inferInsert(const PatchColumnNames& prior,
		     int *dest) {
    COOPY_ASSERT(lst.size()==prior.lst.size()+1);
    int i;
    for (i=0; i<(int)prior.lst.size(); i++) {
      if (lst[i]!=prior.lst[i]) {
	break;
      }
    }
    // the inserted element is named by lst[i]
    string mover = lst[i];
    indices = prior.indices;
    int subject = i;
    if (subject>=prior.lst.size()) {
      subject = -1;
    }
    int neg = 0;
    for (int j=0; j<(int)indices.size(); j++) {
      if (indices[j]<neg) {
	neg = indices[j];
      }
    }
    neg--;
    if (subject>=0) {
      indices.insert(indices.begin()+subject,neg);
    } else {
      indices.push_back(neg);
    }
    if (dest!=NULL) *dest = neg;
    //if (dest!=NULL) *dest = subject;
    return mover;
  }

  string inferDelete(const PatchColumnNames& prior,
		     int *src) {
    COOPY_ASSERT(lst.size()==prior.lst.size()-1);
    int i;
    for (i=0; i<(int)lst.size(); i++) {
      if (lst[i]!=prior.lst[i]) {
	break;
      }
    }
    // the deleted element is named by prior.lst[i]
    string mover = prior.lst[i];
    indices = prior.indices;
    int subject = i;
    if (src!=NULL) *src = indices[subject];
    indices.erase(indices.begin()+subject);
    return mover;
  }

  string inferMove(const PatchColumnNames& prior,
		   int *src,
		   int *dest) {
    COOPY_ASSERT(lst.size()==prior.lst.size());
    int i;
    for (i=0; i<(int)lst.size(); i++) {
      if (lst[i]!=prior.lst[i]) {
	break;
      }
    }
    // The moved element is named by either lst[i] or prior.lst[i]
    // We are guaranteed at least one element after this one.
    string mover;
    if (prior.lst[i+1]==lst[i]) {
      mover = prior.lst[i];
    } else {
      mover = lst[i];
    }

    int isrc = -1;
    int idest = -1;
    vector<string>::const_iterator it = 
      find(prior.lst.begin(),prior.lst.end(),mover);
    int rem = it-prior.lst.begin();
    isrc = rem;
    vector<string>::const_iterator it2 = 
      find(lst.begin(),lst.end(),mover);
    int add = it2-lst.begin();
    bool append = false;
    if (add==((int)lst.size())-1) {
      idest = -1;
      append = true;
    } else {
      vector<string>::const_iterator it_post = it2;
      it_post++;
      vector<string>::const_iterator it_post_org = 
	find(prior.lst.begin(),prior.lst.end(),*it_post);
      idest = it_post_org-prior.lst.begin();
    }
    indices = prior.indices;
    int v = indices[rem];
    if (src!=NULL) *src = v;
    if (dest!=NULL) *dest = v;
    indices.erase(indices.begin()+rem);
    //printf("removing at %d, inserting at %d\n", rem, add);
    if (add>rem) add--;
    if (append) {
      indices.push_back(v);
    } else {
      indices.insert(indices.begin()+add,v);
    }

    //if (dest!=NULL) *dest = v;

    //if (src!=NULL) *src = isrc;
    //if (dest!=NULL) *dest = idest;

    return mover;
  }
};


static SheetCell nully(string x) {
  // does nothing for now
  return SheetCell(x,false);
}

bool PatchParser::apply() {
  if (patcher==NULL) return false;

  if (fname!="") {
    sniffer.open(fname.c_str());
  } else {
    sniffer.setString(oneliner.c_str());
  }

  Format format = sniffer.getFormat();
  if (format.id==FORMAT_PATCH_CSV) {
    dbg_printf("Looks like DTBL\n");
    return applyCsv();
  }
  if (format.id==FORMAT_PATCH_TDIFF) {
    dbg_printf("Looks like TDIFF\n");
    return applyTdiff();
  }
  //fprintf(stderr,"Unsupported format\n");
  dbg_printf("Try color\n");
  return applyColor();
}

bool PatchParser::applyCsv() {

  CsvSheet patch;
  if (CsvFile::read(sniffer,patch)!=0) {
    fprintf(stderr,"Failed to read patch\n");
    return false;
  }

  patcher->mergeStart();

  PatchColumnNames names;
  vector<string> allNames;

  map<string,bool> match_column;
  map<string,bool> assign_column;

  vector<SheetCell> selector;
  bool needSelector = true;
  int len = 0;
  int row_index = -1;
  bool ok = true;
  bool sequential = true;

  bool configSent = false;
  bool configSet = false;
  ConfigChange cc;
  bool support_name_ROW = false;
  bool support_name_star = false;
  int name_start = 2;
  for (int i=0; i<patch.height(); i++) {
    dbg_printf("[%d] ", i);
    string cmd0 = patch.cell(0,i);
    string cmd1 = patch.cell(1,i);
    bool fail = false;
    if (cmd0!="config" && configSet && !configSent) {
      patcher->changeConfig(cc);
    }
    if (cmd0=="dtbl") {
      string ver = patch.cell(3,i);
      dbg_printf("Processed header version %s\n", ver.c_str());
      if (ver == "0.2") {
	support_name_ROW = true;
	support_name_star = true;
      }
    } else if (cmd0=="config") {
      string key = patch.cell(1,i);
      string val = patch.cell(2,i);
      config.put(key.c_str(),val.c_str());
      dbg_printf("Set config variable %s -> %s\n", key.c_str(), val.c_str());
      if (key=="order") {
	if (val=="named") {
	  cc.ordered = false;
	  cc.complete = false;
	  cc.trustNames = true;
	} else if (val=="map") {
	  cc.ordered = true;
	  cc.complete = true;
	  cc.trustNames = false;
	} else {
	  fprintf(stderr,"key configuration unrecognized\n");
	  exit(1);
	}
	configSet = true;
      }
    } else if ((cmd0=="sheet"||cmd0=="table")&&cmd1=="name") {
      patcher->setSheet(patch.cell(2,i).c_str());
    } else if (cmd0=="column") {
      OrderChange change;
      PatchColumnNames names2;
      names2.read(patch,name_start,i);
      len = (int)names2.lst.size();
      if (names2.lst.size()>=1) {
	if (names2.lst[0]=="ROW" && support_name_ROW && name_start == 2) {
	  name_start = 3;
	  names2.read(patch,name_start,i);
	  len = (int)names2.lst.size();
	}
      }
      change.indicesBefore = names.indices;
      change.namesBefore = names.lst;
      change.namesAfter = names2.lst;
      allNames = names2.lst;
      match_column.clear();
      assign_column.clear();
      for (int i=0; i<len; i++) {
	match_column[names2.lst[i]] = true;
	assign_column[names2.lst[i]] = true;
      }
      if (cmd1=="name") {
	dbg_printf("Set column names to %s\n", names2.toString().c_str());
	NameChange nc;
	nc.mode = NAME_CHANGE_DECLARE;
	nc.final = false;
	nc.names = names2.lst;
	allNames = nc.names;
	patcher->changeName(nc);
	//patcher->declareNames(names2.lst,false);
      } else if (cmd1=="move") {
	string mover = names2.inferMove(names,&change.subject,&change.object);
	dbg_printf("Moving columns to %s (%s moves // subj %d obj %d)\n", 
		   names2.toString().c_str(),
		   mover.c_str(),
		   change.subject,
		   change.object);
	change.indicesAfter = names2.indices;
	change.mode = ORDER_CHANGE_MOVE;
	patcher->changeColumn(change);
      } else if (cmd1=="insert") {
	string mover = names2.inferInsert(names,&change.subject);
	dbg_printf("Inserting columns to %s (%s insert // subj %d)\n", 
		   names2.toString().c_str(),
		   mover.c_str(),
		   change.subject);
	change.indicesAfter = names2.indices;
	change.mode = ORDER_CHANGE_INSERT;
	patcher->changeColumn(change);	
      } else if (cmd1=="delete") {
	string mover = names2.inferDelete(names,&change.subject);
	dbg_printf("Deleting columns from %s (%s delete // subj %d)\n", 
		   names2.toString().c_str(),
		   mover.c_str(),
		   change.subject);
	change.indicesAfter = names2.indices;
	change.mode = ORDER_CHANGE_DELETE;
	patcher->changeColumn(change);	
      } else {
	fail = true;
      }
      if (!fail) {
	names = names2;
      }
    } else if (cmd0=="row"||cmd0=="link") {
      RowChange change;
      change.sequential = sequential;
      change.names = names.lst;
      change.allNames = allNames;
      for (int k=0; k<(int)change.names.size(); k++) {
	change.indexes[names.lst[k]] = true;
      }
      PatchColumnNames names2;
      if (cmd1=="columns"||cmd1=="name") {
	names2.read(patch,name_start,i);
      } else {
	names2.readData(patch,name_start,i,len);
      }
      if (cmd1=="columns"||cmd1=="name") {
	dbg_printf("Active columns %s\n", names2.toString().c_str());
	names.lst = names2.lst;
	len = (int)names2.lst.size();
	match_column.clear();
	assign_column.clear();
	for (int i=0; i<len; i++) {
	  match_column[names.lst[i]] = true;
	  assign_column[names.lst[i]] = true;
	}
      } else if (cmd1=="operate"||cmd1=="act") {
	dbg_printf("Operate columns %s: ", names2.dataString().c_str());
	for (int i=0; i<len; i++) {
	  string name = names.lst[i];
	  string code = names2.data[i].text;
	  match_column[name] = code.find('*')!=string::npos;
	  assign_column[name] = code.find('=')!=string::npos;
	  dbg_printf("%d:%d ", match_column[name],
		     assign_column[name]);
	}
	dbg_printf("\n");
      } else if (cmd1=="select"||cmd1=="*") {
	sequential = true;
	dbg_printf("Selecting %s\n", names2.dataString().c_str());
	selector = names2.data;
	needSelector = false;
      } else if (cmd1=="start") {
	sequential = true;
	RowChange change2;
	change2.mode = ROW_CHANGE_CONTEXT;
	ok = patcher->changeRow(change2);
      } else if (cmd1=="after") {
	sequential = true;
	dbg_printf("%s %s\n", cmd1.c_str(), names2.dataString().c_str());
	change.mode = ROW_CHANGE_CONTEXT;
	for (int i=0; i<len; i++) {
	  string name = names.lst[i];
	  const SheetCell& val = names2.data[i];
	  bool ok = match_column[name];
	  if (support_name_star && val.text=="*") ok = false;
	  if (ok) {
	    change.cond[change.names[i]] = val;
	  }
	}
	ok = patcher->changeRow(change);
      } else if (cmd1=="move") {
	sequential = true;
	if (needSelector) {
	  selector = names2.data;
	}
	dbg_printf("Moving to %s\n", names2.dataString().c_str());	
	change.mode = ROW_CHANGE_MOVE;
	for (int i=0; i<len; i++) {
	  const SheetCell& sel = selector[i];
	  string name = names.lst[i];
	  bool ok = match_column[name];
	  if (support_name_star && sel.text=="*") ok = false;	
	  if (ok) {
	    change.cond[change.names[i]] = sel;
	  }
	  const SheetCell& val = names2.data[i];
	  ok = assign_column[name];
	  if (support_name_star && val.text=="*") ok = false;
	  if (ok) {
	    change.val[change.names[i]] = val;
	    //printf("assign %s %s\n", change.names[i].c_str(), val.toString().c_str());
	  }
	}
	ok = patcher->changeRow(change);
	needSelector = true;
      } else if (cmd1=="etc") {
	sequential = false;
      } else if (cmd1=="update"||cmd1=="=") {
	sequential = true;
	if (needSelector) {
	  selector = names2.data;
	}
	dbg_printf("Updating to %s\n", names2.dataString().c_str());	
	change.mode = ROW_CHANGE_UPDATE;
	for (int i=0; i<len; i++) {
	  const SheetCell& sel = selector[i];
	  string name = names.lst[i];
	  bool ok = match_column[name];
	  if (support_name_star && sel.text=="*") ok = false;	
	  if (ok) {
	    change.cond[change.names[i]] = sel;
	  }
	  const SheetCell& val = names2.data[i];
	  ok = assign_column[name];
	  if (support_name_star && val.text=="*") ok = false;
	  if (ok) {
	    change.val[change.names[i]] = val;
	    //printf("assign %s %s\n", change.names[i].c_str(), val.toString().c_str());
	  }
	}
	ok = patcher->changeRow(change);
	needSelector = true;
      } else if (cmd1=="insert") {
	sequential = true;
	if (needSelector) {
	  selector = names2.data;
	}
	dbg_printf("Inserting %s\n", names2.dataString().c_str());
	change.mode = ROW_CHANGE_INSERT;
	for (int i=0; i<len; i++) {
	  string name = names.lst[i];
	  const SheetCell& val = names2.data[i];
	  bool ok = assign_column[name];
	  if (support_name_star && val.text=="*") ok = false;
	  if (ok) {
	    change.val[change.names[i]] = val;
	  }
	}
	patcher->changeRow(change);	
	needSelector = true;
      } else if (cmd1=="delete") {
	sequential = true;
	if (needSelector) {
	  selector = names2.data;
	}
	dbg_printf("Deleting %s\n", names2.dataString().c_str());
	change.mode = ROW_CHANGE_DELETE;
	for (int i=0; i<len; i++) {
	  string name = names.lst[i];
	  const SheetCell& val = names2.data[i];
	  bool ok = match_column[name];
	  if (support_name_star && val.text=="*") ok = false;
	  if (ok) {
	    change.cond[change.names[i]] = val;
	  }
	}
	ok = patcher->changeRow(change);	
	needSelector = true;
      } else {
	fail = true;
      }
    } else {
      fail = true;
    }
    if (fail) {
      dbg_printf("%s %s ?\n", cmd0.c_str(), cmd1.c_str());
      if (!coopy_is_verbose()) {
	fprintf(stderr,"%s %s ?\n", cmd0.c_str(), cmd1.c_str());
      }
      fail = false;
    } else if (!ok) {
      dbg_printf("*** %s %s failed\n", cmd0.c_str(), cmd1.c_str());
      if (!coopy_is_verbose()) {
	fprintf(stderr,"*** %s %s failed\n", cmd0.c_str(), cmd1.c_str());
      }
      fail = false;
    }
  }

  patcher->mergeDone();
  patcher->mergeAllDone();

  return true;
}

vector<string> normalizedMessage(const string& line) {
  vector<string> all;
  string result;
  bool white = true;
  bool quote = false;
  bool pending = false;
  bool commit = false;
  for (int i=0; i<(int)line.length(); i++) {
    char ch = line[i];
    if (ch=='\'') {
      quote = !quote;
      result += ch;
      pending = true;
    } else if (quote) {
      result += ch;
      pending = true;
    } else if (((ch==' '||ch=='\r'||ch=='\t')&&!commit)||ch=='|') {
      if (pending) {
	all.push_back(result);
	pending = false;
      }
      result = "";
      white = true;
      if (ch=='|') {
	pending = true;
	commit = true;
      }
    } else {
      white = false;
      result += ch;
      pending = true;
    }
  }
  if (pending&&result!="") {
    all.push_back(result);
    pending = false;
  }
  return all;
}

class TDiffPart {
public:
  string orig;

  string key;
  SheetCell val;
  SheetCell nval;
  bool hasKey;
  bool hasVal;
  bool hasNval;

  bool isId;
  bool isFresh;

  TDiffPart() {
    isId = false;
    isFresh = false;
    hasKey = false;
    hasVal = false;
    hasNval = false;
  }

  TDiffPart(const string& s, bool columnLike) {
    apply(s, columnLike);
  }


  string peel(const string& s, string& sep, string& rem, bool& hasQuote) {
    hasQuote = false;
    bool acceptSingle = true;
    bool acceptDouble = true;
    bool allowDouble = false;
    bool quote = false;
    int state = 0;
    int pre = -2;
    sep = "";
    for (int i=0; i<(int)s.length(); i++) {
      char ch = s[i];
      if ((allowDouble&&ch=='\"'&&acceptDouble)||(ch=='\''&&acceptSingle)) {
	if (i==0) {
	  hasQuote = true;
	}
	state = 0;
	quote = !quote;
	if (ch=='\"'&&quote) {
	  acceptSingle = false;
	}
	if (ch=='\''&&quote) {
	  acceptDouble = false;
	}
	if (!quote) {
	  acceptSingle = acceptDouble = true;
	}
      } else if (quote) {
	state = 0;
      } else if (ch==':'&&state==0&&pre==-2) {
	pre = i-1;
	sep = ":";
	break;
      } else if (ch=='='&&state==0&&pre==-2) {
	pre = i-1;
	sep = "=";
      } else if (ch=='-'&&state==0&&pre==-2) {
	state = 1;
      } else if (ch=='>'&&state==1) {
	pre = i-2;
	sep = "->";
      } else {
	state = 0;
      }
    }
    string result = s;
    rem = "";
    if (pre!=-2) {
      result = s.substr(0,pre+1);
      rem = s.substr(pre+sep.length()+1,s.length());
    }
    if (hasQuote) {
      result = result.substr(1,result.length()-2);
    }
    return result;
  }

  SheetCell getCell(const string& txt, bool quoted) {
    SheetCell result;
    if (quoted) {
      result.text = txt;
      result.escaped = false;
      //printf("Made cell %s from %s\n", result.toString().c_str(), txt.c_str());
      return result;
    }
    if (txt=="NULL") {
      result.escaped = true;
      result.text = txt;
      return result;
    }
    result.text = txt;
    result.escaped = false;
    return result;
  }

  bool checkCell(const string& txt, bool quoted) {
    if (quoted) return true;
    return (txt!="*");
  }

  void applyElement(const string& txt,
		    const string& pre,
		    const string& post,
		    bool quoted,
		    bool columnLike) {
    /*
    printf("applyElement [%s] [%s] [%s], %s, %s\n",
	   pre.c_str(), txt.c_str(), post.c_str(),
	   quoted?"quoted":"unquoted",
	   columnLike?"column":"value");
    */
    if (pre=="") {
      if ((!columnLike)&&(post!="="&&post!=":")) {
	if (checkCell(txt,quoted)) {
	  val = getCell(txt,quoted);
	  hasVal = true;
	}
      } else {
	key = txt;
	hasKey = true;
      }
    } else if (pre=="->") {
      if (!columnLike) {
	if (checkCell(txt,quoted)) {
	  nval = getCell(txt,quoted);
	  hasNval = true;
	}
	if (!hasVal) {
	  isFresh = true;
	}
      } else {
	isFresh = true;
      }
    } else if (pre=="="||pre==":") {
      isId = (pre=="=");
      if (!columnLike) {
	if (checkCell(txt,quoted)) {
	  val = getCell(txt,quoted);
	  hasVal = true;
	}
      }
    }
  }

  void apply(const string& s, bool columnLike) {
    orig = s;
    hasKey = false;
    hasVal = false;
    hasNval = false;

    isId = false;
    isFresh = false;

    string sep1, r1, rem1;
    string sep2, r2, rem2;
    string sep3, r3, rem3;
    bool q1, q2, q3;
    bool set1, set2, set3;
    set1 = set2 = set3 = false;

    r1 = peel(s,sep1,rem1, q1);
    set1 = true;
    if (sep1!="") {
      r2 = peel(rem1,sep2,rem2, q2);
      set2 = true;
      if (sep2!="") {
	r3 = peel(rem2,sep3,rem3, q3);
	set3 = true;
      }
    }

    //printf("Input %s\n", s.c_str());
    //printf("  %s %s %s\n", r1.c_str(), sep1.c_str(), rem1.c_str());
    //printf("  %s %s %s\n", r2.c_str(), sep2.c_str(), rem2.c_str());
    //printf("  %s %s %s\n", r3.c_str(), sep3.c_str(), rem3.c_str());

    if (set1) applyElement(r1,"",sep1,q1,columnLike);
    if (set2) applyElement(r2,sep1,sep2,q2,columnLike);
    if (set3) applyElement(r3,sep2,sep3,q3,columnLike);
  }
};

string stringer_encoder(const TDiffPart& part) {
  return string("[") + part.orig + (part.isId?";id":"") + (part.isFresh?";fresh":"") + "]";
}


bool PatchParser::applyTdiff() {
  vector<string> allNames;

  patcher->mergeStart();

  bool eof = false;
  vector<TDiffPart> cols;
  bool inComment = false;
  while (!eof) {
    string line = sniffer.readLine(eof);
    if (inComment) {
      if (line.find("*/")!=string::npos) {
	inComment = false;
      } else {
	dbg_printf("Ignoring comment [%s]\n", line.c_str());
      }
      continue;
    }
    vector<string> msg = normalizedMessage(line);
    if (msg.size()==0) {
      //dbg_printf("Empty line\n");
      continue;
    } 
    string first = msg[0];
    if (first[0]=='#') {
      dbg_printf("Ignoring comment [%s]\n", line.c_str());
      continue;
    }
    if (first.length()>=2) {
      if (first[0]=='/') {
	if (first[1]=='*') {
	  dbg_printf("Ignoring comment [%s]\n", line.c_str());
	  inComment = true;
	  continue;
	}
      }
    }
    dbg_printf("Got [%s] [%s]\n", line.c_str(), first.c_str());
    if (first=="@@@") {
      patcher->setSheet(msg[1].c_str());
    } else if (first=="@"||first=="@@") {
      vector<string> names;
      cols.clear();
      for (int i=1; i<(int)msg.size(); i++) {
	cols.push_back(TDiffPart(msg[i],true));
	names.push_back(cols[i-1].key);
      }
      if (first=="@@") {
	NameChange nc;
	nc.mode = NAME_CHANGE_DECLARE;
	nc.final = false;
	nc.names = names;
	patcher->changeName(nc);
      }
      /*
      printf("Columns set to: ");
      for (int i=0; i<(int)cols.size(); i++) {
	printf("\"%s\" ", cols[i].base.c_str());
      }
      printf("\n");
      */
    } else if (first=="@:"||first=="@+"||first=="@-") {
      vector<string> ocols;
      vector<string> ncols;
      for (int i=0; i<(int)cols.size(); i++) {
	ocols.push_back(cols[i].key);
      }
      for (int i=2; i<(int)msg.size(); i++) {
	ncols.push_back(msg[i]);
      }
      
      OrderChange change;
      PatchColumnNames names;
      PatchColumnNames names2;
      names.read(ocols,0);
      names2.read(ncols,0);
      if (first=="@:") {
	string mover = names2.inferMove(names,&change.subject,&change.object);
	dbg_printf("Moving columns to %s (%s moves // subj %d obj %d)\n", 
		   names2.toString().c_str(),
		   mover.c_str(),
		   change.subject,
		   change.object);
	change.mode = ORDER_CHANGE_MOVE;
      } else if (first=="@+") {
	string mover = names2.inferInsert(names,&change.subject);
	dbg_printf("Inserting columns to %s (%s insert // subj %d)\n", 
		   names2.toString().c_str(),
		   mover.c_str(),
		   change.subject);
	change.mode = ORDER_CHANGE_INSERT;
      } else if (first=="@-") {
	string mover = names2.inferDelete(names,&change.subject);
	dbg_printf("Deleting columns from %s (%s delete // subj %d)\n", 
		   names2.toString().c_str(),
		   mover.c_str(),
		   change.subject);
	change.mode = ORDER_CHANGE_DELETE;
      }
      change.indicesBefore = names.indices;
      change.indicesAfter = names2.indices;
      change.namesBefore = names.lst;
      change.namesAfter = names2.lst;
      allNames = change.namesAfter;
      patcher->changeColumn(change);

      cols.clear();
      for (int i=2; i<(int)msg.size(); i++) {
	cols.push_back(TDiffPart(msg[i],true));
      }
 
    } else if (first=="="||first=="-"||first=="+"||first=="*"||first==":") {
      vector<TDiffPart> assign;
      bool mod = false;
      for (int i=1; i<(int)msg.size(); i++) {
	TDiffPart part;
	part.apply(msg[i],false);
	//printf("key %d [%s] / val %d [%s] / nval %d [%s]\n", 
	//(int)part.hasKey, part.key.c_str(),
	//(int)part.hasVal, part.val.toString().c_str(),
	//     (int)part.hasNval, part.nval.toString().c_str());

	assign.push_back(part);
	if (part.hasKey) { mod = true; }
      }
      if (mod) {
	cols.clear();
	for (int i=1; i<(int)msg.size(); i++) {
	  TDiffPart& a = assign[i-1];
	  TDiffPart context = a;
	  context.hasVal = false;
	  context.hasNval = false;
	  a.hasKey = false;
	  cols.push_back(context);
	}
      }
      dbg_printf("  assign %s\n", vector2string(assign).c_str());
      dbg_printf("  cols %s\n", vector2string(cols).c_str());
      while (assign.size()<cols.size()) {
	cols.erase(cols.begin()+assign.size());
      }
      COOPY_ASSERT(assign.size()==cols.size());

      RowChange change;
      bool allIndex = false;
      if (first=="=") {
	change.mode = ROW_CHANGE_UPDATE;
      } else if (first=="-") {
	change.mode = ROW_CHANGE_DELETE;
      } else if (first=="+") {
	change.mode = ROW_CHANGE_INSERT;
      } else if (first=="*") {
	change.mode = ROW_CHANGE_CONTEXT;
      } else if (first==":") {
	change.mode = ROW_CHANGE_MOVE;
      }
      change.indexes.clear();
      for (int i=0; i<(int)assign.size(); i++) {
	TDiffPart& context = cols[i];
	TDiffPart& part = assign[i];
	bool conded = false;
	if (part.hasVal) {
	  if (context.hasKey&&change.mode!=ROW_CHANGE_INSERT) {
	    if (!context.isFresh) {
	      change.cond[context.key.c_str()] = part.val;
	      conded = true;
	    }
	  }
	}
	if (context.isId||allIndex) {
	  if (part.hasVal) {
	    change.indexes[context.key.c_str()] = true;
	  }
	}
	if (part.hasNval||change.mode==ROW_CHANGE_INSERT) {
	  if (part.hasNval) {
	    change.val[context.key.c_str()] = part.nval;
	  } else {
	    if (!conded) {
	      change.val[context.key.c_str()] = part.val;
	    }
	  }
	}
	if (context.isFresh) {
	  if (part.hasVal) {
	    if (!conded) {
	      change.val[context.key.c_str()] = part.val;
	    }
	  }
	}
	change.names.push_back(context.key.c_str());
      }
      if (change.names.size()>allNames.size()) {
	allNames = change.names;
      }
      change.allNames = allNames;
      dbg_printf("  ... change row ...\n");
      patcher->changeRow(change);
    }
  }

  patcher->mergeDone();
  patcher->mergeAllDone();

  return true;
}



bool PatchParser::applyColor() {
  sniffer.close();
  PolyBook book;
  if (!book.read(fname.c_str())) {
    fprintf(stderr, "Don't know what to do with %s\n", fname.c_str());
    return false;
  }

  patcher->mergeStart();

  vector<string> names = book.getNames();
  bool needDeclare = false;
  for (int s=0; s<(int)names.size(); s++) {
    string name = names[s];
    patcher->setSheet(name.c_str());
    PolySheet sheet = book.readSheet(name);
    if (sheet.width()<2) {
      fprintf(stderr, "Not a diff: %s\n", fname.c_str());
      return false;
    }
    vector<string> cols;
    vector<string> activeCol;
    vector<string> statusCol;
    RowChange::txt2bool indexes;
    for (int i=0; i<sheet.height(); i++) {
      RowChange change;
      change.names = activeCol;
      change.allNames = cols;
      //printf("Cols are %s\n",vector2string(cols).c_str());
      change.indexes = indexes;
      string code = sheet.cellString(0,i);
      string tail2 = "";
      if (code.length()>=2) {
	tail2 = code.substr(code.length()-2,2);
      }
      if (code[0] == '@') {
	cols.clear();
	bool acts = false;
	if (i>0) {
	  if (sheet.cellString(0,i-1)=="!") {
	    acts = true;
	  }
	}
	for (int j=1; j<sheet.width(); j++) {
	  TDiffPart p(sheet.cellString(j,i),false);
	  string txt = p.hasNval?p.nval.text:p.val.text;
	  cols.push_back(txt);
	  activeCol.push_back(txt);
	  statusCol.push_back("");
	  indexes[txt] = true;
	}
	if (acts) {
	  activeCol.clear();
	  for (int j=1; j<sheet.width(); j++) {
	    string act = sheet.cellString(j,i-1);
	    string col = cols[j-1];
	    //printf("[%s] [%s]\n", act.c_str(), col.c_str());
	    if (act=="+++") {
	      statusCol[j-1] = act;
	      indexes[col] = false;
	    }
	    if (act=="---") {
	      statusCol[j-1] = act;
	      indexes[col] = false;
	    } else {
	      activeCol.push_back(col);
	    }
	  }
	  for (int j=0; j<(int)cols.size(); j++) {
	    string act = statusCol[j];
	    //printf("   [%d] [%s]\n", j, act.c_str());
	    if (act=="") continue;
	    OrderChange order;
	    if (act=="+++") {
	      order.mode = ORDER_CHANGE_INSERT;
	    } else if (act=="---") {
	      order.mode = ORDER_CHANGE_DELETE;
	    }
	    for (int k=0; k<(int)cols.size(); k++) {	
	      string actk = statusCol[k];
	      if (actk=="" || (k>=j&&actk=="---") || (k<j&&actk=="+++")) {
		order.namesBefore.push_back(cols[k]);
		order.indicesBefore.push_back((actk=="+++")?-(k+1):k);
	      }
	    }
	    for (int k=0; k<(int)cols.size(); k++) {	
	      string actk = statusCol[k];
	      if (actk=="" || (k>j&&actk=="---") || (k<=j&&actk=="+++")) {
		order.namesAfter.push_back(cols[k]);
		order.indicesAfter.push_back((actk=="+++")?-(k+1):k);
	      }
	    }
	    order.subject = (act=="+++")?-(j+1):j;
	    patcher->changeColumn(order);
	  }
	}
	code = code.substr(1,code.length());

	NameChange nc;
	nc.mode = NAME_CHANGE_DECLARE;
	nc.final = true;
	nc.names = activeCol;
	patcher->changeName(nc);
      }
      if (code == "+++") {
	change.mode = ROW_CHANGE_INSERT;
	for (int j=1; j<sheet.width(); j++) {
	  SheetCell c = sheet.cellSummary(j,i);
	  change.val[cols[j-1]] = c;
	}
	patcher->changeRow(change);
      } else if (code == "---") {
	change.mode = ROW_CHANGE_DELETE;
	for (int j=1; j<sheet.width(); j++) {
	  SheetCell c = sheet.cellSummary(j,i);
	  change.cond[cols[j-1]] = c;
	}
	patcher->changeRow(change);
      } else if (tail2 == "->") {
	change.mode = ROW_CHANGE_UPDATE;
	int minuses = 0;
	string separator = code.substr(code.find("-"),code.length());
	for (int j=1; j<sheet.width(); j++) {
	  SheetCell c = sheet.cellSummary(j,i);
	  bool done = false;
	  string col = cols[j-1];
	  bool added = statusCol[j-1]=="+++";
	  if (!c.escaped) {
	    //printf("Looking at [%s], separator [%s]\n",
	    //c.toString().c_str(), separator.c_str());
	    string::size_type offset = c.text.find(separator);
	    if (offset!=string::npos) {
	      if (!added) {
		change.cond[col] = nully(c.text.substr(0,offset));
	      }
	      change.val[col] = nully(c.text.substr(offset+separator.length(),
						    c.text.length()));
	    } else {
	      change.val[col] = nully(c.text);
	    }
	    done = true;
	    /*
	    TDiffPart p(c.text,false,minuses);
	    if (p.hasNval) {
	      change.val[col] = p.nval;
	      if (!added) {
		change.cond[col] = p.val;
	      }
	      done = true;
	    }
	    */
	  }
	  if (!done) {
	    if (!added) {
	      change.cond[col] = c;
	    }
	  }
	}
	patcher->changeRow(change);
      } else {
	if (i<sheet.height()-1) {
	  if (sheet.cellString(0,i+1)=="+++") {
	    change.mode = ROW_CHANGE_CONTEXT;
	    for (int j=1; j<sheet.width(); j++) {
	      SheetCell c = sheet.cellSummary(j,i);
	      change.cond[cols[j-1]] = c;
	    }
	    patcher->changeRow(change);
	  }
	}
      }
    }
  }

  patcher->mergeDone();
  patcher->mergeAllDone();

  return true;
}


