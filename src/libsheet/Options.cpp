#include <stdio.h>
#include <getopt.h>

#include <coopy/Options.h>
#include <coopy/CsvSheet.h>
#include <coopy/CsvWrite.h>
#include <coopy/Coopy.h>

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
	printf("% *c", tot, ' ');
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
      printf("% *c",start,' ');
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
	printf("% *c",start,' ');
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
      printf("    %s --example %s\n", opt.getName().c_str(), reqs[i].c_str());
    }
    printf("\n");
  }

  for (int i=0; i<(int)examples.size(); i++) {
    if (i>0) printf("\n");
    const Example& eg = examples[i];
    printf("  %s\n", eg.code.c_str());
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
  printf("\n\n\\section %s_index Index\n", opt.getName().c_str());
  printf("  \\li \\ref %s_options\n", opt.getName().c_str());
  printf("  \\li \\ref %s_options_detail\n", opt.getName().c_str());
  if (examples.size()>0) {
    printf("  \\li \\ref %s_examples\n", opt.getName().c_str());
  }
  printf("  \\li \\ref %s_patch\n", opt.getName().c_str());
  printf("  \\li \\ref %s_table\n", opt.getName().c_str());
  printf("  \\li \\ref %s_version\n", opt.getName().c_str());

  printf("\n\n\\section %s_options Option summary\n", opt.getName().c_str());

  int f = opt.getOptionFilter();
  string anchor = opt.getName() + "_main";
  showOptions(opt,f,anchor);

  printf("\n\n\\section %s_options_detail Option details\n", opt.getName().c_str());
  showOptions(opt,f,anchor,true);

  if (examples.size()==0) return;
  printf("\n\n\\section %s_examples Examples\n", opt.getName().c_str());

  const vector<string> reqs = opt.getExampleReqs();
  if (reqs.size()>0) {
    printf("You can generate test file(s) for the examples that follow:\n");
    printf("\\verbatim\n");
    for (int i=0; i<(int)reqs.size(); i++) {
      printf("%s --example %s\n", opt.getName().c_str(), reqs[i].c_str());
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
    printf("\\verbatim\n%s\n\\endverbatim\n", eg.code.c_str());
    printf("%s\n\n",eg.desc.c_str());
  }

  printf("\n\n\\section %s_patch Patch formats\n", opt.getName().c_str());
  showOptions(opt,OPTION_PATCH_FORMAT,anchor,true,true);

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
  addTransform("output=OUTPUTFILE",
	       "direct output to this file (default is standard output)");
  add(OPTION_FOR_DIFF|OPTION_FOR_REDIFF,
      "format=FORMAT",
      "set difference format for output");
  add(OPTION_FOR_DIFF|OPTION_FOR_MERGE|OPTION_FOR_PATCH|OPTION_FOR_FORMAT,
      "input-formats",
      "list supported input database formats");
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
  addAll("default-table=TABLE",
	 "name to use when a table name is needed and not supplied");
  add(OPTION_FOR_DIFF|OPTION_FOR_MERGE|OPTION_FOR_FORMAT,
      "table=TABLE",
      "operate on a single named table of a workbook/database");
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
      "omit any version-dependent header from diff"),

  add(OPTION_FOR_DIFF|OPTION_FOR_REDIFF,
      "omit-sheet-name",
      "omit any sheet/table name from diff"),

  add(OPTION_FOR_PATCH,
      "cmd=CMD",
      "specify a patch (in tdiff format) with a string rather than as a file, useful to make a quick change to a table that does not merit a full patch file"),

  add(OPTION_FOR_FORMAT,
      "header",
      "extract column names only"),

  add(OPTION_FOR_FORMAT,
      "index",
      "extract content of key columns only"),

  add(OPTION_FOR_RESOLVE,
      "theirs",
      "in case of conflict use cell value that wasn't the local choice"),

  add(OPTION_FOR_RESOLVE,
      "ours",
      "in case of conflict use cell value that was the local choice"),

  add(OPTION_FOR_RESOLVE,
      "neither",
      "in case of conflict use cell value from common ancestor"),

  add(OPTION_FOR_RESOLVE,
      "dry-run",
      "make no changes, just describe what would happen"),

  add(OPTION_PATCH_FORMAT,
      "*tdiff",
      "vaguely similar to a standard unix diff");
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
    csv.cellString(1,4,"444");
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
      {"format-csv", 0, 0, 'c'},
      {"format-sql", 0, 0, 's'},
      {"format-raw", 0, 0, 'r'},
      {"format-tdiff", 0, 0, 't'},
      {"format-index", 0, 0, 'i'},

      {"format", 1, 0, 'f'},
      {"input-format", 1, 0, 0},
      {"output-format", 1, 0, 0},

      {"apply", 0, 0, 'a'},
      {"dry-run", 0, 0, 0},

      {"equals", 0, 0, 'e'},
      {"index", 0, 0, 'i'},
      {"verbose", 0, 0, 'v'},

      {"id", 1, 0, 'k'},
      {"bid", 1, 0, 'b'},

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
      {"patch-formats", 0, 0, 0},
      {"default-table", 1, 0, 0},

      {"help-doxygen", 0, 0, 0},
      {"example", 1, 0, 0},

      {0, 0, 0, 0}
    };

    int c = getopt_long(argc, argv, "",
			long_options, &option_index);
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
	} else if (k=="example") {
	  bool ok = generateExample(optarg);
	  if (!ok) {
	    exit(1);
	  }
	  option_bool["gen"] = true;
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
      option_string["cmd"] = optarg;
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


