#ifndef FORMAT_DESC_INC
#define FORMAT_DESC_INC

#include <coopy/Property.h>

#include <vector>
#include <string>

namespace coopy {
  namespace store {
    class FormatDesc;
  }
}

class coopy::store::FormatDesc {
public:
  std::string name;

  FormatDesc(const char *name) : name(name) {
  }

  class Extension {
  public:
    std::string ext;
    std::string notes;
    Extension(const char *ext, const char *notes) : ext(ext), notes(notes) {}
  };

  class Dbi {
  public:
    std::string ex;
    std::string notes;
    Dbi(const char *ex, const char *notes) : ex(ex), notes(notes) {}
  };

  class Option {
  public:
    std::string tag;
    PolyValue val;
    std::string notes;
    bool opt;

    Option(const char *tag, PolyValue val, const char *notes, bool opt) :
      tag(tag), val(val), notes(notes), opt(opt) {}
  };

  std::vector<Extension> exts;
  std::vector<Option> opts;
  std::vector<Dbi> dbis;

  void addExtension(const char *ext, const char *notes) {
    exts.push_back(Extension(ext,notes));
  }
  void addDbi(const char *ext, const char *notes) {
    dbis.push_back(Dbi(ext,notes));
  }
  void addOption(const char *tag, PolyValue val, const char *notes, bool opt) {
    opts.push_back(Option(tag,val,notes,opt));
  }

  std::string toString() {
    std::string result = "";
    if (exts.size()>0) {
      result += "  This format is activated for files with the following extensions:\n";
      for (int i=0; i<(int)exts.size(); i++) {
	result += "    extension: ";
	result += exts[i].ext;
	if (exts[i].notes!="") {
	  result += " (";
	  result += exts[i].notes;
	  result += ")";
	}
	result += "\n";
      }
    }
    if (opts.size()>0) {
      result += "  This format is activated for a .json file like this example:\n";
      result += "    {\n";
      for (int i=0; i<(int)opts.size(); i++) {
	Option& o = opts[i];
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
	result += "\n";
      }
      result += "    }\n";
    }
    if (dbis.size()>0) {
      result += "  This format is activated for references starting with 'dbi:' such as:\n";
      for (int i=0; i<(int)dbis.size(); i++) {
	result += "    ";
	result += dbis[i].ex;
	if (dbis[i].notes!="") {
	  result += " (";
	  result += dbis[i].notes;
	  result += ")";
	}
	result += "\n";
      }
    }
    return result;
  }

  void show() {
    printf("%s\n%s\n", name.c_str(),toString().c_str());
  }
};

#endif

