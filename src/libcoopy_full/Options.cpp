#include <stdio.h>
#include <getopt.h>

#include <coopy/Options.h>
#include <coopy/CsvSheet.h>
#include <coopy/CsvWrite.h>
#include <coopy/Coopy.h>
#include <coopy/MergeOutputFilter.h>
#include <coopy/PatchParser.h>
#include <coopy/SheetPatcher.h>
#include <coopy/PoolImpl.h>
#include <coopy/Stringer.h>

#include <algorithm>

#define QUOTED_BASE(x) # x
#define QUOTED_VERSION(x) QUOTED_BASE(x)

using namespace std;
using namespace coopy::store;
using namespace coopy::cmp;
using namespace coopy::app;





class OptionCompare {
public:
  bool operator() (const Option& o1, const Option& o2) {
    if (o1.is_default&&!o2.is_default) return true;
    if (o2.is_default&&!o1.is_default) return false;
    return (o1.long_name<o2.long_name);
  }
};

class OptionRender {
public:
  virtual void render(const Options& opt) = 0;
};

class OptionRenderCmdLine : public OptionRender {
public:
  int w;

  OptionRenderCmdLine() {
    w = 78;
  }

  void showOptions(const Options& opt, int filter);
  virtual void render(const Options& opt);
};

class OptionRenderDoxygen : public OptionRender {
public:
  void showOptions(const Options& opt, int filter, const string& prefix,
		   bool detailed = false, bool compact = false);
  virtual void render(const Options& opt);
};

static void addSpaces(int n) {
  for (int i=0; i<n; i++) {
    printf(" ");
  }
}

static void wrapCode(const string& code, int off) {
  string add = "";
  for (int i=0; i<off; i++) { add += " "; }
  string txt = code;
  Stringer::replace(txt,"\\","\\\n");
  Stringer::replace(txt,"\n",string("\n")+add);
  printf("%s%s\n",add.c_str(),txt.c_str());
}

static void wrapText(const string& desc, int w, int tot) {
  string txt = desc;
  if (txt.length()>0) {
    txt[0] = toupper(txt[0]);
  }
  while (txt.length()>0) {
    string next = txt.substr(0,w-tot+1);
    if (next.length()>w-tot) {
      string::size_type at = next.rfind(" ");
      if (at!=string::npos) {
	next = txt.substr(0,at);
	txt = txt.substr(at+1,txt.length());
      } else {
	next = txt.substr(0,w-tot);
	txt = txt.substr(w-tot,txt.length());
      }
    } else {
      txt = "";
    }
    printf("%s\n", next.c_str());
    if (txt.length()>0) {
      if (tot>0) {
	addSpaces(tot);
      }
    }
  }
}

void OptionRenderCmdLine::showOptions(const Options& opt, int filter) {
  bool flaggy = true;
  int start = 2;
  int len = 15;
  if (filter==OPTION_PATCH_FORMAT) {
    flaggy = false;
    len = 6;
    start = 18;
  }

  vector<Option> mopts = opt.getOptionList();
  sort(mopts.begin(),mopts.end(),OptionCompare());

  for (int i=0; i<(int)mopts.size(); i++) {
    Option& o = mopts[i];
    int tot = start+len+1;
    if (o.coverage&filter) {
      addSpaces(start);
      string pre = o.long_name;
      if (flaggy) {
	pre = string("--")+pre;
      }
      if (o.arg!="") {
	pre += "=";
	pre += o.arg;
      }
      if (pre.length()>len) {
	printf("%s\n", pre.c_str());
	addSpaces(start);
	pre = "";
      }
      while (pre.length()<len) pre += " ";
      printf("%s ", pre.c_str());

      string desc = o.desc;
      if (o.is_default) {
	desc = string("[default] ") + desc;
      }
      wrapText(desc, w, tot);
      if (o.long_name=="format") {
	showOptions(opt,OPTION_PATCH_FORMAT);
      }
    }
  }
}

void OptionRenderCmdLine::render(const Options& opt) {
  printf("%s version %s\n", opt.getName().c_str(), opt.getVersion().c_str());
  printf("Usage\n");
  printf("\n");
  const vector<string>& usages = opt.getUsages();
  for (int i=0; i<(int)usages.size(); i++) {
    printf("  %s\n", usages[i].c_str());
  }
  printf("\n");
  const string& desc = opt.getDescription();
  wrapText(desc,w,0);
  //printf("%s\n", desc.c_str());
  printf("\n");
  printf("Options\n");
  int f = opt.getOptionFilter();
  showOptions(opt,f);

  const vector<Example>& examples = opt.getExamples();
  if (examples.size()==0) return;
  printf("\n");
  printf("Examples\n");

  const vector<string> reqs = opt.getExampleReqs();
  if (reqs.size()>0) {
    printf("  You can generate test file(s) to use with the examples that follow:\n");
    for (int i=0; i<(int)reqs.size(); i++) {
      if (reqs[i][0]!='_') {
	printf("    %s --test-file %s\n", opt.getName().c_str(), reqs[i].c_str());
      }
    }
    const vector<string> recipes = opt.getExampleRecipes(reqs);
    for (int i=0; i<(int)recipes.size(); i++) {
      printf("    %s\n", recipes[i].c_str());
    }
    printf("\n");
  }

  for (int i=0; i<(int)examples.size(); i++) {
    if (i>0) printf("\n");
    const Example& eg = examples[i];
    //printf("  %s\n", eg.code.c_str());
    wrapCode(eg.code,2);
    printf("    ");
    wrapText(eg.desc,w,4);
  }
}


void OptionRenderDoxygen::showOptions(const Options& opt, int filter,
				      const string& prefix, bool detailed,
				      bool compact) {
  bool flaggy = (filter!=OPTION_PATCH_FORMAT);
  vector<Option> mopts = opt.getOptionList();
  sort(mopts.begin(),mopts.end(),OptionCompare());

  
  for (int i=0; i<(int)mopts.size(); i++) {
    Option& o = mopts[i];
    if (o.coverage&filter) {
      string pre = o.long_name;
      if (flaggy) {
	pre = string("--")+pre;
      }
      if (o.arg!="") {
	pre += "=";
	pre += o.arg;
      }
      
      string anchor = prefix + "_" + o.long_name;
      string desc = o.desc;
      if (o.is_default) {
	desc = string("<i>[default]</i> ") + desc;
      }
      if (!detailed) {
	  printf(" \\li \\ref %s \"%s\"\n", anchor.c_str(), pre.c_str());
      } else if (compact) {
	printf("  \\li <b>%s</b>: ", pre.c_str());
	printf("%s\n", desc.c_str());
      } else {
	printf("\\anchor %s <b>%s</b> <br />", anchor.c_str(), pre.c_str());	
	printf("%s\n\n\n", desc.c_str());
      }
    }
  }
}

void OptionRenderDoxygen::render(const Options& opt) {
  printf("/**\n");
  printf(" *\n\n");

  printf("@page %s %s\n\n", opt.getName().c_str(),
	 opt.getName().c_str());

  printf("%s\n\n", opt.getDescription().c_str());

  printf("\n\n\\section %s_usage Usage\n", opt.getName().c_str());
  const vector<string>& usages = opt.getUsages();
  for (int i=0; i<(int)usages.size(); i++) {
    printf(" \\li %s\n", usages[i].c_str());
  }
  const vector<Example>& examples = opt.getExamples();
  printf("\\if MAN_PAGE_COND\n");
  printf("\\else\n");
  printf("\n\n\\section %s_index Index\n", opt.getName().c_str());
  printf("  \\li \\ref %s_options\n", opt.getName().c_str());
  printf("  \\li \\ref %s_options_detail\n", opt.getName().c_str());
  if (examples.size()>0) {
    printf("  \\li \\ref %s_examples\n", opt.getName().c_str());
  }
  bool show_patch = opt.checkBool("show-patch",true);
  bool show_input_format = opt.checkBool("show-input-format",true);
  if (show_patch) {
    printf("  \\li \\ref %s_patch\n", opt.getName().c_str());
  }
  if (show_input_format) {
    printf("  \\li \\ref %s_table\n", opt.getName().c_str());
  }
  printf("  \\li \\ref %s_version\n", opt.getName().c_str());
  printf("\\endif\n");

  printf("\n\n\\section %s_options Option summary\n", opt.getName().c_str());

  int f = opt.getOptionFilter();
  string anchor = opt.getName() + "_main";
  showOptions(opt,f,anchor);

  printf("\n\n\\section %s_options_detail Option details\n", opt.getName().c_str());
  showOptions(opt,f,anchor,true);

  if (examples.size()>0) {
    printf("\n\n\\section %s_examples Examples\n", opt.getName().c_str());
  }

  const vector<string> reqs = opt.getExampleReqs();
  if (reqs.size()>0) {
    printf("You can generate test file(s) for the examples that follow:\n");
    printf("\\verbatim\n");
    for (int i=0; i<(int)reqs.size(); i++) {
      if (reqs[i][0]!='_') {
	printf("%s --test-file %s\n", opt.getName().c_str(), reqs[i].c_str());
      }
    }
    printf("\\endverbatim\n");
    printf("\n\n");
  }

  for (int i=0; i<(int)examples.size(); i++) {
    if (i>0) printf("\n\n");
    const Example& eg = examples[i];
    printf("\n\n\\subsection %s_examples_%d Example %d\n", 
	   opt.getName().c_str(),
	   i+1, i+1);
    printf("\\verbatim\n");
    wrapCode(eg.code,0);
    printf("\\endverbatim\n");
    printf("%s\n\n",eg.desc.c_str());
  }

  if (show_patch) {
    printf("\n\n\\section %s_patch Patch formats\n", opt.getName().c_str());
    showOptions(opt,OPTION_PATCH_FORMAT,anchor,true,true);
  }

  if (show_input_format) {
    printf("\n\n\\section %s_table Database/spreadsheet file formats\n", opt.getName().c_str());
    vector<FormatDesc> descs = PolyBook::getFormatList();
    for (int i=0; i<(int)descs.size(); i++) {
      const FormatDesc& fd = descs[i];
      printf("%s<br />\n", fd.name.c_str());
      for (int i=0; i<(int)fd.exts.size(); i++) {
	printf("  \\li<b>%s</b>: %s\n",
	       fd.exts[i].ext.c_str(),
	       fd.exts[i].notes.c_str());
      }
      const vector<FormatDesc::Option>& opts = fd.opts;
      if (opts.size()>0) {
	string result;
	printf("  \\li<b>.json</b>: {<br />\n");
	for (int i=0; i<(int)opts.size(); i++) {
	  const FormatDesc::Option& o = opts[i];
	  result += "      \"";
	  result += o.tag;
	  result += "\": ";
	  PolyValue v = o.val;
	  if (o.val.isString()) {
	    result += "\"";
	  }
	  result += o.val.asString();
	  if (o.val.isString()) {
	    result += "\"";
	  }
	  if (i<(int)opts.size()-1) {
	    result += ",";
	  }
	  if (o.notes!="") {
	    result += "  <i>// ";
	    result += o.notes;
	    result += "</i>";
	  }
	  result += "<br />\n";
	}
	printf("%s}\n", result.c_str());
      }
      const vector<FormatDesc::Dbi>& dbis = fd.dbis;
      for (int i=0; i<(int)dbis.size(); i++) {
	printf("  \\li <b>%s</b>", dbis[i].ex.c_str());
	if (dbis[i].notes!="") {
	  printf(" (%s) ", dbis[i].notes.c_str());
	}
	printf("\n");
      }
      printf("\n\n");
    }
  }

  printf("\n\n\\section %s_version Version\n", opt.getName().c_str());

  printf("%s version %s\n", opt.getName().c_str(), opt.getVersion().c_str());

  printf("\n\n");
  printf(" *\n");
  printf(" */\n");
}





void Options::add(int cov, const char *name, const char *desc) {
  Option o;
  o.coverage = cov;
  o.desc = desc;
  string n(name);
  if (n[0]=='*') {
    n = n.substr(1,n.length());
    o.is_default = true;
  }
  string::size_type idx = n.find("=");
  if (idx!=string::npos) {
    o.long_name = n.substr(0,idx);
    o.arg = n.substr(idx+1,n.length());
  } else {
    o.long_name = n;
  }
  opts.push_back(o);
}


Options::Options(const char *name) : name(name) {
  addAll("help",
	 "show how to use this program");
  addTransform("output=OUTPUTFILE",
	       "direct output to this file (default is standard output)");
  add(OPTION_FOR_DIFF|OPTION_FOR_REDIFF,
      "format=FORMAT",
      "set difference format for output");
  add(OPTION_FOR_DIFF|OPTION_FOR_REDIFF,
      "variant=VARIANT",
      "set the desired dialect when using a poorly defined output format (currently for SQL, available variants are: sqlite, access)");
  add(OPTION_FOR_DIFF|OPTION_FOR_MERGE|OPTION_FOR_PATCH|OPTION_FOR_FORMAT,
      "input-formats",
      "list supported input database formats");
  add(OPTION_FOR_DIFF|OPTION_FOR_MERGE,
      "headerless",
      "treat any embedded column names as regular parts of the table (for formats like CSV)");
  addTransform("patch-formats",
	       "list supported patch formats");
  addCompare("id=COLUMN",
	     "set primary key (repeat option for multi-column key)");
  addCompare("bid=COLUMN",
	     "boost a column (repeat option for multiple columns)");
  addCompare("named",
	     "trust names of columns, omitting checks for column renames");
  addCompare("unordered",
	     "treat order of rows as unimportant");
  addCompare("fixed-columns",
	     "ignore new or removed columns");
  addCompare("head-trimmed",
	     "ignore rows removed at the beginning of a table (such as a log file)");
  addCompare("tail-trimmed",
	     "ignore rows removed at the end of a table (such as a log file)");
  add(OPTION_FOR_DIFF|OPTION_FOR_MERGE|OPTION_FOR_FORMAT|OPTION_FOR_PATCH|OPTION_FOR_RESOLVE|OPTION_FOR_REDIFF,
      "default-table=TABLE",
      "name to use when a table name is needed and not supplied");
  add(OPTION_FOR_FORMAT,
      "table=TABLE",
      "operate on a single named table of a workbook/database");
  add(OPTION_FOR_FORMAT,
      "paint",
      "add color highlighting appropriate for highlighter diffs");
  add(OPTION_FOR_DIFF|OPTION_FOR_MERGE|OPTION_FOR_PATCH|OPTION_FOR_REDIFF,
      "table=TABLE",
      "filter for a named table of a workbook/database (repeat option for multiple tables)");
  add(OPTION_FOR_DIFF,
      "apply",
      "apply difference between FILE1 and FILE2 immediately to FILE1");
  add(OPTION_FOR_DIFF,
      "parent=PARENT",
      "use named workbook/database as common ancestor in difference calculations");
  add(OPTION_FOR_MERGE|OPTION_FOR_PATCH,
      "inplace",
      "if modifications are made, make them in place without a copy");

  add(OPTION_FOR_DIFF|OPTION_FOR_REDIFF,
      "omit-format-name",
      "omit any version-dependent header from diff");

  add(OPTION_FOR_DIFF|OPTION_FOR_REDIFF,
      "omit-sheet-name",
      "omit any sheet/table name from diff");

  add(OPTION_FOR_DIFF|OPTION_FOR_REDIFF|OPTION_FOR_PATCH,
      "act=ACT",
      "filter for an action of a particular type (update, insert, delete, none, schema)");

  add(OPTION_FOR_PATCH,
      "cmd=CMD",
      "specify an action inline in tdiff format");

  add(OPTION_FOR_FORMAT,
      "header",
      "extract column names only");

  add(OPTION_FOR_FORMAT,
      "index",
      "extract content of key columns only");

  add(OPTION_FOR_RESOLVE|OPTION_FOR_REDIFF|OPTION_FOR_PATCH,
      "theirs",
      "in case of conflict use cell value that wasn't the local choice");

  add(OPTION_FOR_RESOLVE|OPTION_FOR_REDIFF|OPTION_FOR_PATCH,
      "ours",
      "in case of conflict use cell value that was the local choice");

  add(OPTION_FOR_RESOLVE|OPTION_FOR_REDIFF|OPTION_FOR_PATCH,
      "neither",
      "in case of conflict use cell value from common ancestor");

  add(OPTION_FOR_RESOLVE,
      "dry-run",
      "make no changes, just describe what would happen");

  add(OPTION_FOR_COOPY,
      "gui",
      "force GUI to be shown");

  add(OPTION_FOR_COOPY,
      "silent",
      "keep output to a minimum");

  add(OPTION_FOR_COOPY,
      "pull",
      "pull in data from remote repository to local clone");

  add(OPTION_FOR_COOPY,
      "push",
      "push out data to remote repository from local clone");

  add(OPTION_FOR_COOPY,
      "key=KEY",
      "use specified key when adding or exporting a spreadsheet/database");

  add(OPTION_FOR_COOPY,
      "add=FILE",
      "attach the given spreadsheet/database to the repository");

  add(OPTION_FOR_COOPY,
      "export=FILE",
      "export the given spreadsheet/database from the repository");

  add(OPTION_FOR_COOPY,
      "clone=URL",
      "clone the given repository");

  add(OPTION_FOR_COOPY,
      "new",
      "create a new, empty repository");

  add(OPTION_FOR_COOPY,
      "message=MESSAGE",
      "use the specified message as a log entry");

  add(OPTION_PATCH_FORMAT,
      "*tdiff",
      "reminiscent of the standard unix diff format for text");
  add(OPTION_PATCH_FORMAT,
      "raw",
      "verbose diff format for debugging");
  add(OPTION_PATCH_FORMAT,
      "sql",
      "SQL format (data diffs only)");
  add(OPTION_PATCH_FORMAT,
      "hilite",
      "colorful spreadsheet format");
  add(OPTION_PATCH_FORMAT,
      "review",
      "spreadsheet diff format suitable for quickly accepting or rejecting changes");
  add(OPTION_PATCH_FORMAT,
      "index",
      "tabular output showing relationship between rows and columns");
  add(OPTION_PATCH_FORMAT,
      "csv",
      "csv-compatible diff format");
  add(OPTION_PATCH_FORMAT,
      "ops",
      "summarize modified rows in a table");
  add(OPTION_PATCH_FORMAT,
      "stats",
      "produce statistics on table changes");
  add(OPTION_PATCH_FORMAT,
      "novel",
      "mark all shared rows - remaining rows are unmatched");

  add(OPTION_FOR_DIFF,
      "scan-for-patch",
      "check if FILE2 looks like a patch, and if so, apply it");

}

std::string Options::getVersion() const {
  return QUOTED_VERSION(COOPY_VERSION);
}

static void generateNumbers(CsvSheet& csv, bool buggy, bool addy, 
			    bool conflict) {
  csv.addField("NAME",false);
  csv.addField("DIGIT",false);
  csv.addRecord();
  csv.addField("one",false);
  csv.addField("1",false);
  csv.addRecord();
  csv.addField("two",false);
  csv.addField("2",false);
  csv.addRecord();
  csv.addField("three",false);
  csv.addField("3",false);
  csv.addRecord();
  csv.addField("four",false);
  csv.addField("4",false);
  csv.addRecord();
  csv.addField("five",false);
  csv.addField("5",false);
  csv.addRecord();
  
  if (buggy) {
    csv.cellString(1,4,"999");
    csv.deleteRow(RowRef(3));
  }

  if (addy) {
    RowRef r = csv.insertRow(RowRef(-1));
    csv.cellString(0,r.getIndex(),"six");
    csv.cellString(1,r.getIndex(),"6");
    r = csv.insertRow(RowRef(-1));
    csv.cellString(0,r.getIndex(),"seven");
    csv.cellString(1,r.getIndex(),"7");
    csv.insertRow(RowRef(1));
    csv.cellString(0,1,"zero");
    csv.cellString(1,1,"0");
  }

  if (conflict) {
    if (buggy||addy) {
      fprintf(stderr,"* cannot make what you want\n");
      exit(1);
    }
    csv.deleteRow(RowRef(3));
    csv.cellString(1,3,"444");
  }
}

static bool generateExample(const string& name) {
  if (name=="numbers.csv"||name=="numbers_buggy.csv"||
      name=="numbers_add.csv"||name=="numbers_add.sqlite"||
      name=="numbers_buggy_add.csv"||name=="numbers_buggy_add.sqlite"||
      name=="numbers_conflict.csv"||name=="numbers_conflict.sqlite"||
      name=="numbers.sqlite"||name=="numbers_buggy.sqlite") {
    CsvSheet csv;
    generateNumbers(csv,name.find("_buggy")!=string::npos,
		    name.find("_add")!=string::npos,
		    name.find("_conflict")!=string::npos);

    WrapBook book1(csv,false);
    book1.addReference();
    PolyBook book2;
    book2.take(&book1);
    if (!book2.write(name.c_str())) {
      fprintf(stderr,"* failed to generate %s\n", name.c_str());
      return false;
    }
    fprintf(stderr,"* generated %s\n", name.c_str());
    return true;
  }
  if (name=="directory.sqlite") {
    PolyBook book;
    if (book.attach("directory.sqlite")) {
      string dir = " \n\
@@@ locations \n\
x location |id=| \n\
@ |id|street|city| \n\
+ |1|305 Memorial Drive|Cambridge| \n\
+ |2|Big Crater|Moon| \n\
\n\
@@@ org2loc \n\
x organization |org_id| \n\
x location |loc_id| \n\
@ |org_id|loc_id| \n\
+ |1|2| \n\
+ |2|1| \n\
+ |3|1| \n\
+ |3|2| \n\
\n\
@@@ organizations \n\
x organization |id=| \n\
@ |id|name| \n\
+ |1|Dracula Inc| \n\
+ |2|The Firm| \n\
+ |3|Omni Cooperative| \n\
+ |4|Nonexistence Unlimited| \n\
";

      PoolImpl pool;
      CompareFlags flags;
      flags.create_unknown_sheets = true;
      flags.clean_sheets = true;
      flags.pool = &pool;
      vector<string> cmd;
      cmd.push_back(dir);
      SheetPatcher sink;
      sink.attachBook(book);
      sink.setFlags(flags);
      MergeOutputFilter filter(&sink);
      filter.attachBook(book);
      filter.setFlags(flags);
      PatchParser parser(&filter,cmd,flags);
      parser.apply();
      book.flush();
      return true;
    }
    fprintf(stderr,"* failed to generate %s\n", name.c_str());
    return false;
  }
  if (name=="numbers_patch.tdiff") {
    CsvSheet csv1, csv2;
    generateNumbers(csv1,true,false,false);
    generateNumbers(csv2,false,false,false);
    Coopy coopy;
    coopy.setOutput("numbers_patch.tdiff");
    if (coopy.compare(csv1,csv2)) {
      fprintf(stderr,"* generated %s\n", name.c_str());
      return true;
    }
    fprintf(stderr,"* failed to generate %s\n", name.c_str());
  }
  return false;
}

int Options::apply(int argc, char *argv[]) {
  //printf("Hello good evening and welcome\n");
  //for (int i=0; i<argc; i++) {
  //printf("[%d] [%s]\n", i, argv[i]);
  //}

  core.clear();
  flags = CompareFlags();
  option_bool.clear();
  mapping.clear();

  option_string["output"] = "-";
  option_string["parent"] = "";
  option_string["version"] = "";

  while (true) {
    int option_index = 0;
    static struct option long_options[] = {
      {"format", 1, 0, 'f'},

      {"format-csv", 0, 0, 'c'},
      {"format-sql", 0, 0, 's'},
      {"format-raw", 0, 0, 'r'},
      {"format-tdiff", 0, 0, 't'},
      {"format-index", 0, 0, 'i'},

      {"input-format", 1, 0, 0},
      {"output-format", 1, 0, 0},

      {"apply", 0, 0, 'a'},
      {"dry-run", 0, 0, 0},

      {"equals", 0, 0, 'e'},
      {"index", 0, 0, 'i'},
      {"verbose", 0, 0, 'v'},

      {"id", 1, 0, 'k'},
      {"bid", 1, 0, 'b'},
      {"coin", 1, 0, 0},

      {"named", 0, 0, 'd'},

      {"fixed-columns", 0, 0, 'F'},
      {"ignore-case", 0, 0, 'A'},

      {"ordered", 0, 0, '1'},
      {"unordered", 0, 0, '0'},

      {"map", 1, 0, 'm'},

      {"omit-format-name", 0, 0, 'O'},
      {"omit-sheet-name", 0, 0, 'P'},
      {"header", 0, 0, 0},

      {"table", 1, 0, 'T'},

      {"output", 1, 0, 'o'},
      {"format-version", 1, 0, 'V'},
      {"parent", 1, 0, 'p'},
      {"input-formats", 0, 0, 'l'},
      {"help", 0, 0, 'h'},

      {"head-trimmed", 0, 0, 0},
      {"tail-trimmed", 0, 0, 0},
      {"fake", 0, 0, 0},
      {"inplace", 0, 0, 0},
      {"tmp", 1, 0, 0},
      {"patch", 1, 0, 0},
      {"resolve", 1, 0, 0},
      {"variant", 1, 0, 0},

      {"theirs", 0, 0, 0},
      {"ours", 0, 0, 0},
      {"neither", 0, 0, 0},

      {"cmd", 1, 0, 'x'},

      {"version", 0, 0, 0},
      {"strict", 0, 0, 0},
      {"patch-formats", 0, 0, 0},
      {"default-table", 1, 0, 0},
      {"eol", 1, 0, 0},
      {"headerless", 0, 0, 0},
      {"paint", 0, 0, 0},
      {"scan-for-patch", 0, 0, 0},

      {"act", 1, 0, 0},

      {"help-doxygen", 0, 0, 0},
      {"test-file", 1, 0, 0},

      {"foreign", 0, 0, 0},
      {"native", 0, 0, 0},

      {"meta", 1, 0, 0},
      {"pool", 1, 0, 0},

      {"create", 0, 0, 0},

      {0, 0, 0, 0}
    };

    int c = getopt_long(argc, argv, "", long_options, &option_index);
    if (c==-1) break;
    switch (c) {
    case 0:
      {
	string k = long_options[option_index].name;
	if (k=="head-trimmed") {
	  flags.head_trimmed = true;
	} else if (k=="tail-trimmed") {
	  flags.tail_trimmed = true;
	} else if (k=="fake") {
	  option_string["mode"] = "raw";
	} else if (k=="inplace") {
	  option_bool["inplace"] = true;
	} else if (k=="tmp") {
	  option_string["tmp"] = optarg;
	} else if (k=="patch") {
	  option_string["patch"] = optarg;
	} else if (k=="header") {
	  option_bool["header"] = true;
	} else if (k=="input-format") {
	  option_string["input-format"] = optarg;
	} else if (k=="output-format") {
	  option_string["output-format"] = optarg;
	} else if (k=="default-table") {
	  coopy_set_default_table_name(optarg);
	} else if (k=="patch-formats") {
	  OptionRenderCmdLine render;
	  render.showOptions(*this,OPTION_PATCH_FORMAT);
	  exit(0);
	} else if (k=="help-doxygen") {
	  option_bool["help"] = true;
	  option_bool["help-doxygen"] = true;
	} else if (k=="version") {
	  option_bool["version"] = true;
	  printf("%s\n", getVersion().c_str());
	  exit(0);
	} else if (k=="strict") {
	  coopy_set_strict(true);
	} else if (k=="variant") {
	  flags.variant = optarg;
	} else if (k=="resolve") {
	  option_bool["resolving"] = true;
	  flags.resolve = option_string["resolve"] = optarg;
	} else if (k=="theirs") {
	  option_bool["resolving"] = true;
	  flags.resolve = option_string["resolve"] = "theirs";
	} else if (k=="ours") {
	  option_bool["resolving"] = true;
	  flags.resolve = option_string["resolve"] = "ours";
	} else if (k=="neither") {
	  option_bool["resolving"] = true;
	  flags.resolve = option_string["resolve"] = "neither";
	} else if (k=="dry-run") {
	  option_bool["apply"] = false;
	} else if (k=="headerless") {
	  flags.assume_header = false;
	} else if (k=="foreign") {
	  flags.foreign_pool = true;
	  flags.foreign_pool_set = true;
	} else if (k=="native") {
	  flags.foreign_pool = false;
	  flags.foreign_pool_set = true;
	} else if (k=="paint") {
	  option_bool["paint"] = true;
	} else if (k=="scan-for-patch") {
	  option_bool["scan-for-patch"] = true;
	} else if (k=="test-file") {
	  bool ok = generateExample(optarg);
	  if (!ok) {
	    exit(1);
	  }
	  option_bool["gen"] = true;
	} else if (k=="eol") {
	  string eol = optarg;
	  if (eol!="native"&&eol!="CRLF"&&eol!="LF") {
	    fprintf(stderr,"EOL style not recognized: use native, CRLF, or LF.\n");
	    exit(1);
	  }
	  fprintf(stderr,"Warning: EOL style does not do anything yet. It is a placeholder.\n");
	  coopy_set_default_eol_style(eol.c_str());
	} else if (k=="act") {
	  string act = optarg;
	  if (act=="+") act = "insert";
	  if (act=="-") act = "delete";
	  if (act=="=") act = "update";
	  if (act=="0") act = "none";
	  if (act=="upsert") {
	    flags.acts.insert("update");
	    flags.acts.insert("insert");
	  } else if (act=="update"||act=="insert"||act=="delete"||act=="none"||act=="schema") {
	    flags.acts.insert(act);
	  } else {
	    fprintf(stderr,"Unknown action %s\n", act.c_str());
	    fprintf(stderr,"Try: update, insert, delete\n");
	    return 1;
	  }
	} else if (k == "coin") {
	  flags.coined.push_back(optarg);
	} else if (k == "meta") {
	  option_string["meta"] = optarg;
	} else if (k == "pool") {
	  option_string["pool"] = optarg;
	} else if (k == "create") {
	  flags.create_unknown_sheets = true;
	} else {
	  fprintf(stderr,"Unknown option %s\n", k.c_str());
	  return 1;
	}
      }
      break;
    case 'c':
      option_string["mode"] = "csv";
      break;
    case 's':
      option_string["mode"] = "sql";
      break;
    case 'h':
      option_bool["help"] = true;
      break;
    case 't':
      option_string["mode"] = "tdiff";
      break;
    case 'r':
      option_string["mode"] = "raw";
      break;
    case 'v':
      option_bool["verbose"] = true;
      coopy_set_verbose(true);
      break;
    case 'e':
      option_bool["equals"] = true;
      break;
    case 'i':
      option_string["mode"] = "index";
      option_bool["index"] = true;
      break;
    case 'd':
      flags.trust_column_names = true;
      break;
    case '0':
      flags.use_order = false;
      break;
    case '1':
      flags.use_order = true;
      break;

    case 'k':
      flags.ids.push_back(optarg);
      flags.trust_ids = true;
      flags.bias_ids = false;
      break;

    case 'b':
      flags.ids.push_back(optarg);
      flags.trust_ids = false;
      flags.bias_ids = true;
      break;

    case 'm':
      {
	if (!mapping.read(optarg)) {
	  fprintf(stderr,"Failed to read %s\n", optarg);
	  return 1;
	}
	flags.mapping_book = &mapping;
      }
      break;

    case 'O':
      flags.omit_format_name = true;
      break;
    case 'P':
      flags.omit_sheet_name = true;
      break;

    case 'f':
      option_string["mode"] = optarg;
      break;
    case 'o':
      option_string["output"] = optarg;
      break;
    case 'p':
      option_string["parent"] = optarg;
      break;
    case 'V':
      option_string["version"] = optarg;
      break;
    case 'x':
      option_bool["cmd"] = true;
      addStringToList("cmd",optarg);
      break;

    case 'l':
      PolyBook::showFormats();
      exit(0);
      break;
    case 'a':
      option_bool["apply"] = true;
      break;

    case 'F':
      flags.fixed_columns = true;
      break;

    case 'A':
      flags.ignore_case = true;
      break;

    case 'T':
      flags.tables.insert(optarg);
      flags.ordered_tables.push_back(optarg);
      break;

    default:
      fprintf(stderr, "Unrecognized option\n");
      return 1;
    }
  }

  argc -= optind;
  argv += optind;

  for (int i=0; i<argc; i++) {
    core.push_back(argv[i]);
  }
  if (option_bool.find("gen")!=option_bool.end()) {
    exit(0);
  }
  return 0;
}


void Options::beginHelp() {
}

void Options::addUsage(const char *usage) {
  usages.push_back(usage);
}

void Options::addDescription(const char *desc) {
  description = desc;
}

void Options::endHelp() {
  addRecipe("_numbers_patch.tdiff","ssdiff --output numbers_patch.tdiff numbers_buggy.csv numbers.csv").require("numbers_buggy.csv").require("numbers.csv");
  addRecipe("_numbers_muddle.csv","ssmerge --output numbers_muddle.csv numbers_buggy.csv numbers.csv numbers_conflict.csv").require("numbers_buggy.csv").require("numbers.csv").require("numbers_conflict.csv");
  if (option_bool["help-doxygen"]) {
    OptionRenderDoxygen render;
    render.render(*this);
    return;
  }
  OptionRenderCmdLine render;
  render.render(*this);
}



const std::vector<std::string> Options::getExampleReqs() const {
  map<string,int> reqs;
  for (int i=0; i<(int)examples.size(); i++) {
    const Example& eg = examples[i];
    for (int j=0; j<(int)eg.reqs.size(); j++) {
      string name = eg.reqs[j];
      if (name[0]=='_') {
	const Recipe& recipe = recipes.find(name)->second;
	for (int k=0; k<(int)recipe.reqs.size(); k++) {
	  reqs[recipe.reqs[k]] = 1;
	}
      }
      reqs[eg.reqs[j]] = 1;
    }
  }

  vector<string> reqs_flat;
  for (map<string,int>::iterator it=reqs.begin(); it!=reqs.end(); it++) {
    reqs_flat.push_back(it->first);
  }
  sort(reqs_flat.begin(),reqs_flat.end());

  return reqs_flat;
}


const std::vector<std::string> Options::getExampleRecipes(const std::vector<std::string>& reqs) const {
  std::vector<std::string> result;
  for (int i=0; i<(int)reqs.size(); i++) {
    if (recipes.find(reqs[i])!=recipes.end()) {
      result.push_back(recipes.find(reqs[i])->second.code);
    }
  }
  return result;
}

