#include <stdio.h>
#include <getopt.h>

#include <coopy/CsvFile.h>
#include <coopy/CsvTextBook.h>
#include <coopy/MergeOutputIndex.h>
#include <coopy/MergeOutputPool.h>
#include <coopy/MergeOutputFilter.h>
#include <coopy/BookCompare.h>
#include <coopy/PolyBook.h>
#include <coopy/SheetPatcher.h>
#include <coopy/PatchParser.h>
#include <coopy/PoolImpl.h>
#include <coopy/Options.h>
#include <coopy/Highlighter.h>
#include <coopy/ShortTextBook.h>
#include <coopy/IndexSniffer.h>

#include <coopy/Diff.h>

using namespace std;
using namespace coopy::app;
using namespace coopy::store;
using namespace coopy::cmp;

static Patcher *createTool(string mode, string version="") {
  return Patcher::createByName(mode.c_str(),version.c_str());
}

static bool copyFile(const char *src, const char *dest) {
  return Patcher::copyFile(src,dest);
}


// cloned should, in fact, be always true
static bool copyBook(PolyBook& src_book, const char *src, const char *dest,
		     bool *cloned) {
  PolyBook check;
  Property p1 = check.getType(src);
  Property p2 = check.getType(dest);
  if (p1.get("type").asString()==p2.get("type").asString()) {
    *cloned = true;
    return Patcher::copyFile(src,dest);
  }
  *cloned = false;
  return src_book.write(dest);
}


int Diff::apply(const Options& opt) {
  dbg_printf("{} Diff::apply begins\n");
  bool resolved = true;
  bool verbose = opt.checkBool("verbose");
  bool equality = opt.checkBool("equals");
  bool resolving = opt.isResolveLike(); //opt.checkBool("resolving")
  std::string output = opt.checkString("output","-");
  std::string parent_file = opt.checkString("parent");
  std::string patch_file = opt.checkString("patch");
  std::string meta_file = opt.checkString("meta");
  std::string pool_file = opt.checkString("pool");
  bool have_cmd = opt.isStringList("cmd");
  std::vector<std::string> cmds;
  if (have_cmd) cmds = opt.getStringList("cmd");
  std::string version = opt.checkString("version");
  std::string tmp = opt.checkString("tmp","");
  std::string resolve = opt.checkString("resolve","");
  std::string defMode = "default";
  bool apply = opt.checkBool("apply",opt.isResolveLike()?true:false);
  if (opt.isMergeLike()) defMode = "merge";
  if (opt.isPatchLike()&&!opt.isRediffLike()) defMode = "apply";
  std::string mode = opt.checkString("mode",defMode.c_str());
  bool inplace = opt.checkBool("inplace",false);
  bool showPatch = opt.isPatchLike() && mode=="apply" && (!apply);
  bool patchy = opt.isPatchLike()||opt.isRediffLike()||opt.isMergeLike();
  bool scan_for_patch = opt.checkBool("scan-for-patch",false);
  bool couldChangeInput = false;
  bool extractHeader = opt.checkBool("header");
  bool extractIndex = opt.checkBool("index");
  bool omitHeader = opt.checkBool("omit-header");
  const vector<string>& include_columns = opt.getCompareFlags().include_columns;
  const vector<string>& exclude_columns = opt.getCompareFlags().exclude_columns;
  bool have_includes = (include_columns.size() > 0);
  bool have_excludes = (exclude_columns.size() > 0);

  CompareFlags flags = opt.getCompareFlags();
  PoolImpl pool;
  flags.pool = &pool;
  PolyBook pool_book;
  if (pool_file!="") {
    if (!pool_book.attachReadWrite(pool_file.c_str())) {
      fprintf(stderr,"Failed to attach %s\n", pool_file.c_str());
      return 1;
    }
    pool.attachBook(pool_book);
  }

  vector<string> core = opt.getCore();
  if (opt.isMergeLike()) {
    if (core.size()>0) {
      parent_file = core[0];
      core.erase(core.begin());
    }
  }
  if (opt.isPatchLike()&&!opt.isFormatLike()) {
    couldChangeInput = true;
    if (core.size()>0) {
      if ((!have_cmd)||core.size()>1) {
	patch_file = core.back();
	core.erase(core.begin()+core.size()-1);
      }
    }
  }

  BookCompare cmp;
  cmp.setVerbose(verbose);

  PolyBook _pivot;
  PolyBook *pivot;
  PolyBook _local;
  PolyBook *local = &_local;
  PolyBook _remote;
  PolyBook *remote = &_remote;

  string local_file;
  if (core.size()>=1) {
    local_file = core[0];
  }
  
  string remote_file;
  bool read_and_will_write = false;
  if (core.size()>=2) {
    if (opt.isFormatLike()) {
      output = core[1];
      read_and_will_write = true;
    } else {
      remote_file = core[1];
    }
  }

  dbg_printf("\n{} Diff::apply checking local file if any\n");

  if (local_file!="") {
    if (read_and_will_write) {
      if (!_local.readAndWillWrite(local_file.c_str(),
				   "",
				   output.c_str(),
				   "")) {
	fprintf(stderr,"Failed to read %s\n", local_file.c_str());
	return 1;
      }
    } else {
      if (!_local.read(local_file.c_str())) {
	fprintf(stderr,"Failed to read %s\n", local_file.c_str());
	return 1;
      }
    }
    flags.local_uri = local_file;
  }
  if (inplace) {
    output = local_file;
  }

  // an option to extract header - mostly of interest for ssformat
  if (extractHeader) {
    PolySheet sheet = _local.readSheetByIndex(0);
    CompareFlags flags;
    NameSniffer sniff(sheet,flags);
    ShortTextBook *book = new ShortTextBook();
    if (book==NULL) {
      fprintf(stderr,"Failed to allocate output\n");
      return 1;
    }
    for (int i=0; i<sheet.width(); i++) {
      book->sheet.addField(sniff.suggestColumnName(i).c_str(),false);
    }
    book->sheet.addRecord();
    _local.take(book);
  }

  // an option to extract index - mostly of interest for ssformat
  if (extractIndex) {
    PolySheet sheet = _local.readSheetByIndex(0);
    CompareFlags flags;
    NameSniffer nsniff(sheet,flags);
    IndexSniffer sniff(sheet,flags,nsniff);
    vector<int> indexes = sniff.suggestIndexes();
    dbg_printf("Index count %d\n", (int)indexes.size());
    ShortTextBook *book = new ShortTextBook();
    if (book==NULL) {
      fprintf(stderr,"Failed to allocate output\n");
      return 1;
    }
    book->sheet.copy(sheet);
    int at = 0;
    for (int i=0; i<sheet.width(); i++) {
      if (indexes[i]==0) {
	book->sheet.deleteColumn(at);
      } else {
	at++;
      }
    }
    _local.take(book);
  }

  // options to omit/include columns
  if (have_excludes) {
    for (int i=0; i<(int)exclude_columns.size(); i++) { 
      have_cmd = true;
      string cmd = "@- |";
      cmd += exclude_columns[i];
      cmd += "|";
      cmds.push_back(cmd);
    }
    couldChangeInput = true;
  }
  if (have_includes) {
    string cmd = "@@= |";
    for (int i=0; i<(int)include_columns.size(); i++) { 
      cmd += include_columns[i];
      cmd += "|";
    }
    have_cmd = true;
    cmds.push_back(cmd);
    couldChangeInput = true;
  }


  dbg_printf("\n{} Diff::apply checking remote file if any\n");

  bool patch_is_remote = false;
  if (remote_file!="") {
    //if (!_remote.read(remote_file.c_str())) {
    if (flags.offload_to_sql_when_possible) {
      if (!_remote.readForReference(remote_file.c_str(),_local)) {
	fprintf(stderr,"Failed to read %s\n", remote_file.c_str());
	return 1;
      }
    } else {
      if (!_remote.read(remote_file.c_str())) {
	fprintf(stderr,"Failed to read %s\n", remote_file.c_str());
	return 1;
      }
    }
    flags.remote_uri = remote_file;
    if (scan_for_patch) {
      if (_remote.getSheetCount()>=1) {
	PolySheet sheet = _remote.readSheetByIndex(0);
	if (sheet.isValid()) {
	  if (sheet.width()>=1) {
	    for (int i=0; i<3 && i<sheet.height(); i++) {
	      if (sheet.cellString(0,i)=="@@") {
		patch_is_remote = true;
		mode = "apply";
		patchy = true;
		showPatch = !apply;
	      }
	    }
	  }
	}
      }
    }
  }

  dbg_printf("\n{} Diff::apply doing inplace tweaks if needed\n");

  bool cloned = false;
  if (couldChangeInput&&(mode=="apply"||mode=="merge"||mode=="novel")&&!apply) {
    if (local_file!=""&&local->inplace()&&local->getNames().size()>0&&!inplace&&output!=local_file) {
      if (output=="-"&&tmp=="") {
	fprintf(stderr,"This operation could modify an input.  Please confirm by specifying one of:\n");
	fprintf(stderr,"  --inplace, to confirm you want the input changed.\n  --output OUTPUT_FILE, to redirect the output.\n  --tmp TMP_FILE, use temporary storage to avoid changing the input.\n");
	return 1;
      }
      if (tmp=="") {
	tmp = output;
      }
      dbg_printf("Copy %s -> %s\n", local_file.c_str(), tmp.c_str());
      if (!copyBook(*local,local_file.c_str(),tmp.c_str(),&cloned)) {
	fprintf(stderr,"Failed to write %s\n", tmp.c_str());
	return 1;
      }
      dbg_printf("Cloned copy? %d\n", cloned);
      if (!_local.read(tmp.c_str())) {
	fprintf(stderr,"Failed to switch to %s\n", tmp.c_str());
	return 1;
      }
    }
  }

  // an option to omit header - mostly of interest for ssformat
  if (omitHeader) {
    for (int i=0; i<_local.getSheetCount(); i++) {
      PolySheet sheet = _local.readSheetByIndex(i);
      CompareFlags flags;
      NameSniffer sniff(sheet,flags);
      if (sniff.isEmbedded()) {
	RowRef row0(0);
	RowRef rowh(sniff.getHeaderHeight()-1);
	sheet.deleteRows(row0,rowh);
      }
    }
  }

  dbg_printf("\n{} Diff::apply dealing with pivot if any\n");

  if (parent_file!="") {
    if (!_pivot.read(parent_file.c_str())) {
      fprintf(stderr,"Failed to read %s\n", parent_file.c_str());
      return 1;
    }
    flags.pivot_uri = parent_file;
    pivot = &_pivot;
  } else {
    pivot = &_local;
    flags.pivot_sides_with_local = true;
    flags.has_pivot = false;
  }

  if (equality) {
    if (local->equals(*remote,flags)) {
      return 0;
    }
    return 1;
  }

  dbg_printf("\n{} Diff::apply creating tool: %s %s\n", mode.c_str(), version.c_str());

  Patcher *diff = createTool(mode,version);
  MergeOutputPool pooler;
  if (diff==NULL) {
    fprintf(stderr,"Failed to create handler for patch mode: '%s'\n",
	    mode.c_str());
    return 1;
  }
  PolyBook obook;
  if (diff->needOutputBook()) {
    dbg_printf("\n{} Diff::apply preparing output book\n");
    // output touched
    if (!obook.attach(output.c_str())) {
      delete diff; diff = NULL;
      return 1;
    }
    diff->attachOutputBook(obook);
  }

  PolyBook tbook;
  if (diff->outputStartsFromInput()&&!cloned) {
    dbg_printf("\n{} Diff::apply fiddling with output book #1\n");
    char ch = 'x';
    if (output.length()>0) {
      ch = output[output.length()-1];
    }
    if (ch!='-') { // crude test for stdin/out
      // output_touched
      dbg_printf("{} Diff::apply writing local to output\n");
      if (!_local.write(output.c_str())) {
	delete diff; diff = NULL;
	return 1;
      }
      dbg_printf("{} Diff::apply re-reading local\n");
      if (!_local.read(local_file.c_str())) {
	fprintf(stderr,"Failed to read %s\n", local_file.c_str());
	return 1;
      }
      // output touched
      dbg_printf("{} Diff::apply reading output if exists\n");
      if (!tbook.readIfExists(output.c_str())) {
	fprintf(stderr,"Failed to read %s\n", output.c_str());
	return 1;
      }
    } else {
      tbook.take(new CsvTextBook(true));
      Property p;
      tbook.copy(_local,p);
    }
    diff->attachBook(tbook);
    pooler.attachBook(tbook);
  } else {
    dbg_printf("\n{} Diff::apply fiddling with output book #2\n");
    diff->attachBook(*local);
    pooler.attachBook(*local);
  }

  if (apply) {
    dbg_printf("\n{} Diff::apply prepare for application\n");
    SheetPatcher *apply_diff = SheetPatcher::createForApply();
    COOPY_ASSERT(apply_diff!=NULL);
    apply_diff->showSummary(diff);
    diff = apply_diff;
    diff->attachBook(*local);
    pooler.attachBook(*local);
  }

  dbg_printf("\n{} Diff::apply start output\n");
  if (!diff->startOutput(output,flags)) {
    fprintf(stderr,"Patch output failed\n");
    delete diff;
    diff = NULL;
    return 1;
  }

  if (meta_file!="") {
    dbg_printf("{} Diff::apply add meta file\n");
    pooler.startOutput(output,flags);
    pooler.setFlags(flags);
    PatchParser parser(&pooler,meta_file,flags);
    bool ok = parser.apply();
    pooler.stopOutput(output,flags);
    if (!ok) {
      fprintf(stderr,"Failed to read %s\n", meta_file.c_str());
      delete diff;
      diff = NULL;
      return 1;
    }
  }

  if (!resolving) {
    dbg_printf("\n{} Diff::apply diff/patch/merge\n");
    diff->setFlags(flags);
    bool filter = flags.ordered_tables.size()>0 || flags.acts.size()>0 || 
      flags.create_unknown_sheets || patchy || flags.resolve!="";
    MergeOutputFilter filter_diff(diff);
    if (filter) {
      filter = true;
      //if (flags.create_unknown_sheets) {
      if (diff->getBook()) {
	filter_diff.attachBook(*(diff->getBook()));
      }
    }
    Patcher *active_diff = diff;
    if (filter) {
      active_diff = &filter_diff;
      filter_diff.startOutput("-",flags);
      filter_diff.setFlags(flags);
    }
    if (patch_file==""&&(!have_cmd)&&(!patch_is_remote)&&(!opt.isFormatLike())) {
      cmp.compare(*pivot,*local,*remote,*active_diff,flags);
    } else {
      bool ok = false;
      if (have_cmd) {
	dbg_printf("\n{} Diff::apply diff/patch/merge with command\n");
	PatchParser parser(active_diff,cmds,flags);
	ok = parser.apply();
      } else if (patch_is_remote) {
	dbg_printf("\n{} Diff::apply diff/patch/merge from remote\n");
	PatchParser parser(active_diff,remote,flags);
	ok = parser.apply();
      } else if (patch_file!="") {
	dbg_printf("\n{} Diff::apply diff/patch/merge from file\n");
	PatchParser parser(active_diff,patch_file,flags);
	ok = parser.apply();
      } else {
	dbg_printf("\n{} Diff::apply diff/patch/merge from other\n");
	vector<string> acts;
	PatchParser parser(active_diff,acts,flags);
	ok = parser.apply();
      }
      if (!ok) {
	fprintf(stderr,"Patch failed\n");
      }
    }
    if (filter) {
      filter_diff.stopOutput("-",flags);
    }
  } else {
    dbg_printf("\n{} Diff::apply resolve\n");
    flags.resolving = true;
    if (flags.remote_uri == "") {
      flags.remote_uri = flags.local_uri;
    }
    if (!opt.checkBool("resolving")) {
      vector<string> names = local->getNames();
      for (int j=0; j<names.size(); j++) {
	PolySheet t = local->readSheetByIndex(j);
	t.mustHaveSchema();
	SheetSchema *ss = t.getSchema();
	if (ss) {
	  for (int i=0; i<ss->getColumnCount(); i++) {
	    string s = ss->getColumnInfo(i).getName();
	    if (s=="_MERGE_") {
	      resolved = false;
	    }
	  }
	}
      }
    } else {
      cmp.resolve(*pivot,*local,*remote,*diff,flags);
    }
  }

  dbg_printf("\n{} Diff::apply finish up\n");

  bool will_write_output = false;
  if (showPatch) {
    if ((!local->inplace())||(tmp!=output)) {
      if (output!=local_file || !local->inplace()) {
	will_write_output = true;
      }
    }
  }

  diff->stopOutput(output,flags);
  if (diff->needOutputBook()) {
    if (obook.isValid()) {
      if (!diff->outputStartsFromInput()) {
	if (!will_write_output) {
	  obook.flush();
	}
      }
    } else {
      local->flush();
    }
  }

  if (pool_file!="") {
    pool.save();
    pool_book.flush();
  }

  if (diff->outputStartsFromInput()) {
    // output touched
    if (!tbook.inplace()) {
      dbg_printf("{} Diff::apply write tbook\n");
      if (!tbook.write(output.c_str())) {
	fprintf(stderr,"Failed to write %s\n", output.c_str());
	return 1;
      }
    }
  }
  if (mode=="apply"&&!showPatch) {
    if (diff->getChangeCount()>0) {
      if (!local->inplace()) {
	dbg_printf("{} Diff::apply write local #1\n");
	if (!local->write(local_file.c_str())) {
	  fprintf(stderr,"Failed to write %s\n", local_file.c_str());
	  return 1;
	}
      }
    }
  }
  if (showPatch) {
    if ((!local->inplace())||(tmp!=output)) {
      if (output!=local_file || !local->inplace()) {
	dbg_printf("{} Diff::apply write local #2\n");
	if (!local->write(output.c_str())) {
	  fprintf(stderr,"Failed to write %s\n", output.c_str());
	  delete diff; diff = NULL;
	  return 1;
	}
      }
    }
  }


  TextBook *output_book = diff->getBook();
  if (opt.checkBool("paint")) {
    // Reopen file for painting - not optimal, but adequate
    PolyBook src;
    if (!src.attach(output.c_str())) {
      fprintf(stderr,"Failed to attach %s\n", output.c_str());
      return 1;
    }
    Highlighter h;
    h.apply(src);
    src.flush();
  }

  bool conflicted = diff->isConflicted();
  delete diff;
  diff = NULL;

  if (opt.isMergeLike()) {
    if (conflicted) {
      fprintf(stderr,"Conflict detected.\n");
      return 1;
    }
  }
  if (!resolved) {
      fprintf(stderr,"Conflict unresolved.\n");
      return 1;
  }

  return 0;
}

