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
  std::string version = opt.checkString("version");
  std::string tmp = opt.checkString("tmp","");
  std::string resolve = opt.checkString("resolve","");
  std::string defMode = "tdiff";
  bool apply = opt.checkBool("apply",opt.isResolveLike()?true:false);
  if (opt.isMergeLike()) defMode = "merge";
  if (opt.isPatchLike()&&!opt.isRediffLike()) defMode = "apply";
  std::string mode = opt.checkString("mode",defMode.c_str());
  bool inplace = opt.checkBool("inplace",false);
  bool showPatch = opt.isPatchLike() && mode=="apply" && !apply;
  bool patchy = opt.isPatchLike()||opt.isRediffLike()||opt.isMergeLike();
  bool scan_for_patch = opt.checkBool("scan-for-patch",false);

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
  if (opt.isPatchLike()) {
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
  if (core.size()>=2) {
    remote_file = core[1];
  }

  if (local_file!="") {
    if (!_local.read(local_file.c_str())) {
      fprintf(stderr,"Failed to read %s\n", local_file.c_str());
      return 1;
    }
    flags.local_uri = local_file;
  }
  if (inplace) {
    output = local_file;
  }

  bool patch_is_remote = false;
  if (remote_file!="") {
    if (!_remote.read(remote_file.c_str())) {
      fprintf(stderr,"Failed to read %s\n", remote_file.c_str());
      return 1;
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

  bool cloned = false;
  if ((mode=="apply"||mode=="merge"||mode=="novel")&&!apply) {
    if (local_file!=""&&local->inplace()&&local->getNames().size()>0&&!inplace&&output!=local_file) {
      if (output=="-"&&tmp=="") {
	fprintf(stderr,"This operation would modify an input. Not sure if you want this.\n");
	fprintf(stderr,"Please specify:\n  --inplace, to confirm you want the input changed.\n  --output OUTPUT_FILE, to redirect the output.\n  --tmp TMP_FILE, use temporary storage to avoid changing the input.\n");
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
    if (*local == *remote) {
      return 0;
    }
    return 1;
  }

  Patcher *diff = createTool(mode,version);
  MergeOutputPool pooler;
  if (diff==NULL) {
    fprintf(stderr,"Failed to create handler for patch mode: '%s'\n",
	    mode.c_str());
    return 1;
  }
  PolyBook obook;
  if (diff->needOutputBook()) {
    // output touched
    if (!obook.attach(output.c_str())) {
      delete diff; diff = NULL;
      return 1;
    }
    diff->attachOutputBook(obook);
  }

  PolyBook tbook;
  if (diff->outputStartsFromInput()&&!cloned) {
    char ch = 'x';
    if (output.length()>0) {
      ch = output[output.length()-1];
    }
    if (ch!='-') { // crude test for stdin/out
      // output_touched
      if (!_local.write(output.c_str())) {
	delete diff; diff = NULL;
	return 1;
      }
      if (!_local.read(local_file.c_str())) {
	fprintf(stderr,"Failed to read %s\n", local_file.c_str());
	return 1;
      }
      // output touched
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
    diff->attachBook(*local);
    pooler.attachBook(*local);
  }

  if (apply) {
    SheetPatcher *apply_diff = SheetPatcher::createForApply();
    COOPY_ASSERT(apply_diff!=NULL);
    apply_diff->showSummary(diff);
    diff = apply_diff;
    diff->attachBook(*local);
    pooler.attachBook(*local);
  }

  if (!diff->startOutput(output,flags)) {
    fprintf(stderr,"Patch output failed\n");
    delete diff;
    diff = NULL;
    return 1;
  }
  if (meta_file!="") {
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
    if (patch_file==""&&(!have_cmd)&&(!patch_is_remote)) {
      cmp.compare(*pivot,*local,*remote,*active_diff,flags);
    } else {
      bool ok = false;
      if (have_cmd) {
	PatchParser parser(active_diff,opt.getStringList("cmd"),flags);
	ok = parser.apply();
      } else if (patch_is_remote) {
	PatchParser parser(active_diff,remote,flags);
	ok = parser.apply();
      } else {
	PatchParser parser(active_diff,patch_file,flags);
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
    if (!tbook.write(output.c_str())) {
      fprintf(stderr,"Failed to write %s\n", output.c_str());
      return 1;
    }
  }
  if (mode=="apply"&&!showPatch) {
    if (diff->getChangeCount()>0) {
      if (!local->inplace()) {
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
	if (!local->write(output.c_str())) {
	  fprintf(stderr,"Failed to write %s\n", output.c_str());
	  delete diff; diff = NULL;
	  return 1;
	}
      }
    }
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

