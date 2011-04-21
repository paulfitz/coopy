
#include <coopy/PatchParser.h>
#include <coopy/CsvFile.h>
#include <coopy/Format.h>
#include <coopy/Dbg.h>

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


bool PatchParser::apply() {
  if (reader==NULL) return false;
  if (patcher==NULL) return false;

  Format format = reader->getFormat();
  if (format.id==FORMAT_PATCH_CSV) {
    return applyCsv();
  }
  if (format.id==FORMAT_PATCH_TDIFF) {
    return applyTdiff();
  }
  fprintf(stderr,"Unsupported format\n");
  return false;
}

bool PatchParser::applyCsv() {

  CsvSheet patch;
  if (CsvFile::read(*reader,patch)!=0) {
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
      } else if (cmd1=="after"||cmd1=="move") {
	sequential = true;
	dbg_printf("%s %s\n", cmd1.c_str(), names2.dataString().c_str());
	change.mode = (cmd1=="after")?ROW_CHANGE_CONTEXT:ROW_CHANGE_MOVE;
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
    if (ch=='\"') {
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
  string base;
  string mod;
  bool hasMod;
  bool isId;
  bool isWild;

  TDiffPart() {
    isId = false;
    hasMod = false;
    isWild = false;
  }

  TDiffPart(const string& s) {
    apply(s);
  }

  void apply(string s) {
    orig = s;
    isId = false;
    hasMod = false;
    isWild = false;
    if (s=="*") {
      base = "*";
      isWild = true;
      return;
    }
    if (s.length()>0) {
      if (s[s.length()-1]=='=') {
	isId = true;
	s = s.substr(0,s.length()-1);
      }
    }

    bool hasQuote = false;
    bool quote = false;
    int state = 0;
    int pre = -2;
    int post = -2;
    for (int i=0; i<(int)s.length(); i++) {
      char ch = s[i];
      if (ch=='\"') {
	hasQuote = true;
	state = 0;
	quote = !quote;
      } else if (quote) {
	state = 0;
      } else if (ch=='-'&&state==0&&pre==-2) {
	state = 1;
      } else if (ch=='>'&&state==1) {
	state = 2;
	pre = i-2;
	post = i+1;
      } else {
	state = 0;
      }
    }
    if (pre!=-2) {
      base = s.substr(0,pre+1);
      mod = s.substr(post,s.length());
      hasMod = true;
    } else {
      base = s;
      mod = "";
    }
    //printf(">> %s %s %d\n", base.c_str(), mod.c_str(), hasMod);
  }

  SheetCell baseCell() {
    return SheetCell(base,false);
  }

  SheetCell modCell() {
    return SheetCell(mod,false);
  }
};

string stringer_encoder(const TDiffPart& part) {
  return part.orig;
}


bool PatchParser::applyTdiff() {
  vector<string> allNames;

  patcher->mergeStart();

  bool eof = false;
  vector<TDiffPart> cols;
  while (!eof) {
    string line = reader->readLine(eof);
    vector<string> msg = normalizedMessage(line);
    if (msg.size()==0) {
      //dbg_printf("Empty line\n");
      continue;
    } 
    string first = msg[0];
    if (first[0]=='#'||first[0]=='/') {
      dbg_printf("Ignoring comment [%s]\n", line.c_str());
      continue;
    }
    dbg_printf("Got [%s] [%s]\n", line.c_str(), first.c_str());
    if (first=="@@@") {
      patcher->setSheet(msg[1].c_str());
    } else if (first=="@"||first=="@@") {
      vector<string> names;
      cols.clear();
      for (int i=1; i<(int)msg.size(); i++) {
	cols.push_back(TDiffPart(msg[i]));
	names.push_back(cols[i-1].base);
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
	ocols.push_back(cols[i].base);
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
	cols.push_back(TDiffPart(msg[i]));
      }
 
    } else if (first=="="||first=="-"||first=="+"||first=="*"||first==":") {
      vector<TDiffPart> assign;
      for (int i=1; i<(int)msg.size(); i++) {
	TDiffPart part;
	part.apply(msg[i]);
	//printf("[%d] [%s] [%s]\n", (int)part.hasMod, part.base.c_str(), part.mod.c_str());
	assign.push_back(part);
      }
      dbg_printf("  assign %s\n", vector2string(assign).c_str());
      dbg_printf("  assign %s\n", vector2string(cols).c_str());
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
	if (!part.isWild) {
	  if (!context.hasMod) {
	    change.cond[context.base.c_str()] = part.baseCell();
	  }
	}
	if (context.isId||allIndex) {
	  if (!part.isWild) {
	    change.indexes[context.base.c_str()] = true;
	  }
	}
	if (part.hasMod||context.hasMod||change.mode==ROW_CHANGE_INSERT) {
	  if (part.hasMod) {
	    change.val[context.base.c_str()] = part.modCell();
	  } else {
	    change.val[context.base.c_str()] = part.baseCell();
	  }
	}
	change.names.push_back(context.base.c_str());
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

