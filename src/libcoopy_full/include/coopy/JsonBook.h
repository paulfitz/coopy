#ifndef COOPY_JSONBOOK
#define COOPY_JSONBOOK

#include <coopy/FoldedSheet.h>
#include <coopy/TextBook.h>
#include <coopy/TextBookFactory.h>

namespace coopy {
  namespace store {
    /**
     *
     * Json table representation.
     *
     */
    namespace json {
      class JsonBook;
      class JsonBookFactory;
    }
  }
}

class coopy::store::json::JsonBook : public TextBook {
public:
  std::vector<PolySheet> sheets;
  std::vector<std::string> names;
  std::map<std::string,int> name2index;

  virtual std::vector<std::string> getNames() {
    return names;
  }

  virtual PolySheet readSheet(const std::string& name) {
    if (name2index.find(name)!=name2index.end()) {
      return sheets[name2index[name]];
    }
    return PolySheet();
  }

  bool clear() {
    sheets.clear();
    names.clear();
    name2index.clear();
    return true;
  }
  
  bool read(const char *fname);

  bool write(const char *fname) {
    Property p;
    return write(fname,this,p);
  }

  static bool write(const char *fname, TextBook *book, const Property& options);

  static std::string render(TextBook *book, const Property& options);

  virtual bool open(const Property& config);

  bool addSheet(const SheetSchema& schema);

  virtual bool namedSheets() const {
    return true;
  }
};



class coopy::store::json::JsonBookFactory : public TextBookFactory {
public:
  virtual std::string getName() {
    return "jsonbook";
  }

  virtual TextBook *open(AttachConfig& config, AttachReport& report) {
    //static int ct = 0;
    //printf("hallo! write %d read %d   :  '%s'\n", config.shouldWrite, config.shouldRead, config.options.get("file").asString().c_str());
    //ct++;
    //COOPY_ASSERT(ct<5);
    if (config.shouldWrite) {
      if (config.prevBook!=NULL) {
	dbg_printf("writing book file %s\n", config.options.get("file").asString().c_str());
	if (JsonBook::write(config.options.get("file").asString().c_str(),
			    config.prevBook,
			    config.options)) {
	  report.success = true;
	  return config.prevBook;
	}
      }
      return NULL;
    }

    JsonBook *book = new JsonBook();
    if (book==NULL) return NULL;

    if (config.shouldRead) {
      if (!config.options.check("should_attach")) {
	dbg_printf("reading jsonbook file %s\n", config.options.get("file").asString().c_str());
	bool r = book->read(config.fname.c_str());
	if (!r) {
	  delete book;
	  book = NULL;
	}
      }
    }
    
    if (book!=NULL) {
      report.success = true;
    }

    return book;
  }

  static JsonBookFactory *makeFactory() {
    return new JsonBookFactory();
  }
};




#endif

