#include <stdio.h>
#include <getopt.h>

#include <coopy/Options.h>

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
  void showOptions(const Options& opt, int filter);
  virtual void render(const Options& opt);
};

class OptionRenderDoxygen : public OptionRender {
public:
  void showOptions(const Options& opt, int filter, const string& prefix,
		   bool detailed = false, bool compact = false);
  virtual void render(const Options& opt);
};

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

  /*
  for (int i=0; i<80; i++) {
    printf("%d", i/10);
  }
  printf("\n");
  for (int i=0; i<80; i++) {
    printf("%d", i%10);
  }
  printf("\n");
  */
  
  for (int i=0; i<(int)mopts.size(); i++) {
    Option& o = mopts[i];
    int tot = start+len+1;
    int w = 78;
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
      if (desc.length()<=w-tot) {
	printf("%s\n", desc.c_str());
      } else {
	string txt = desc;
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
	    printf("% *c", tot, ' ');
	  }
	}
      }
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
  printf("%s\n", desc.c_str());
  printf("\n");
  printf("Options\n");
  int f = opt.getOptionFilter();
  showOptions(opt,f);
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
  printf("\n\n\\section %s_index Index\n", opt.getName().c_str());
  printf("  \\li \\ref %s_options\n", opt.getName().c_str());
  printf("  \\li \\ref %s_options_detail\n", opt.getName().c_str());
  printf("  \\li \\ref %s_patch\n", opt.getName().c_str());
  printf("  \\li \\ref %s_table\n", opt.getName().c_str());
  printf("  \\li \\ref %s_version\n", opt.getName().c_str());

  printf("\n\n\\section %s_options Option summary\n", opt.getName().c_str());

  int f = opt.getOptionFilter();
  string anchor = opt.getName() + "_main";
  showOptions(opt,f,anchor);

  printf("\n\n\\section %s_options_detail Option details\n", opt.getName().c_str());
  showOptions(opt,f,anchor,true);

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
	     "trust names of columns (saves work checking for column renames)");
  addCompare("unordered",
	     "neglect order of rows (saves work)");
  addCompare("fixed-columns",
	     "ignore new or removed columns");
  addCompare("head-trimmed",
	     "ignore rows removed at the beginning of a table (such as a log file)");
  addCompare("tail-trimmed",
	     "ignore rows removed at the end of a table (such as a log file)");
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

  add(OPTION_FOR_FORMAT,
      "header",
      "extract column names only"),

  add(OPTION_FOR_FORMAT,
      "index",
      "extract content of key columns only"),

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

      {"cmd", 1, 0, 'x'},

      {"version", 0, 0, 0},
      {"patch-formats", 0, 0, 0},

      {"help-doxygen", 0, 0, 0},

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


