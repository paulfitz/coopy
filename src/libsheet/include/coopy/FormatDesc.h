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

  void addExtension(const char *ext, const char *notes) {
    exts.push_back(Extension(ext,notes));
  }
  void addOption(const char *tag, PolyValue val, const char *notes, bool opt) {
    opts.push_back(Option(tag,val,notes,opt));
  }

  std::string toString() {
    std::string result = "";
    for (int i=0; i<(int)exts.size(); i++) {
      result += "  extension: ";
      result += exts[i].ext;
      if (exts[i].notes!="") {
	result += " (";
	result += exts[i].notes;
	result += ")";
      }
      result += "\n";
    }
    if (opts.size()>0) {
      result += "  extension: .json\n";
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
    return result;
  }

  void show() {
    printf("%s\n%s", name.c_str(),toString().c_str());
  }
};

#endif

