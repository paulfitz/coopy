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
  return true;
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

  CompareFlags flags = opt.getCompareFlags();
  PoolImpl pool;
  flags.pool = &pool;

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

  if (remote_file!="") {
    if (!_remote.read(remote_file.c_str())) {
      fprintf(stderr,"Failed to read %s\n", remote_file.c_str());
      return 1;
    }
    flags.remote_uri = remote_file;
  }

  bool cloned = false;
  if ((mode=="apply"||mode=="merge")&&!apply) {
    if (local_file!=""&&local->inplace()&&!inplace&&output!=local_file) {
      if (output=="-"&&tmp=="") {
	fprintf(stderr,"Please specify --inplace, or given an --output, or specify a 'tmp' location to avoid modifying database.\n");
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
      if (cloned) {
	if (!_local.read(tmp.c_str())) {
	  fprintf(stderr,"Failed to switch to %s\n", tmp.c_str());
	  return 1;
	}
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
    if (!obook.attach(output.c_str())) {
      delete diff; diff = NULL;
      return 1;
    }
    diff->attachOutputBook(obook);
  }

  PolyBook tbook;
  if (diff->outputStartsFromInput()&&!cloned) {
    if (output!="-") {
      if (!_local.write(output.c_str())) {
	delete diff; diff = NULL;
	return 1;
      }
      if (!_local.read(local_file.c_str())) {
	fprintf(stderr,"Failed to read %s\n", local_file.c_str());
	return 1;
      }
      if (!tbook.read(output.c_str())) {
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
    if (patch_file==""&&(!have_cmd)) {
      cmp.compare(*pivot,*local,*remote,*diff,flags);
    } else {
      diff->setFlags(flags);
      bool filter = false;
      if (flags.ordered_tables.size()>0) {
	filter = true;
      }
      MergeOutputFilter filter_diff(diff);
      Patcher *active_diff = diff;
      if (filter) {
	active_diff = &filter_diff;
	filter_diff.startOutput(output,flags);
	filter_diff.setFlags(flags);
      }
      bool ok = false;
      if (have_cmd) {
	PatchParser parser(active_diff,opt.getStringList("cmd"),flags);
	ok = parser.apply();
      } else {
	PatchParser parser(active_diff,patch_file,flags);
	ok = parser.apply();
      }
      if (filter) {
	filter_diff.stopOutput(output,flags);
      }
      if (!ok) {
	fprintf(stderr,"Patch failed\n");
      }
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

  diff->stopOutput(output,flags);
  if (diff->needOutputBook()) {
    if (obook.isValid()) {
      obook.flush();
    } else {
      local->flush();
    }
  }
  if (diff->outputStartsFromInput()) {
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

