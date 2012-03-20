#include <stdio.h>

#include <gcj/cni.h>
#include <java/lang/System.h>
#include <java/io/PrintStream.h>
#include <java/lang/Throwable.h>
#include <JackBox.h>
#include <coopy/JackTextBook.h>
     
#define JS(x) JvNewStringLatin1(x)

using namespace java::lang;
using namespace com::healthmarketscience::jackcess;
using namespace coopy::store;
using namespace coopy::store::mdb;

int main(int argc, char *argv[]) {
     
  try
    {
      JvCreateJavaVM(NULL);
      JvAttachCurrentThread(NULL, NULL);
      
      String *message = JvNewStringLatin1("Hello from C++");
      JvInitClass(&System::class$);
      System::out->println(message);
      JackBox *jack = new JackBox();
      if (jack) {
	int x = jack->hello();
	printf("I have %d\n", x);
	jack->go();

	Database *db = jack->create(JS("new2.mdb"));
	if (!db) exit(1);
	{
	  TableBuilder *tbl = jack->makeTable(JS("foo"));
	  jack->addColumn(tbl,JS("col1"),JS("string"));
	  jack->addColumn(tbl,JS("col2"),JS("string"));
	  jack->addTable(db,tbl);
	}
	{
	  TableBuilder *tbl = jack->makeTable(JS("foo2"));
	  jack->addColumn(tbl,JS("col1"),JS("string"));
	  jack->addColumn(tbl,JS("col2"),JS("string"));
	  jack->addTable(db,tbl);
	}
	
	//delete jack;
	jack = NULL;
      }

      JvDetachCurrentThread();
    }
  catch (Throwable *t)
    {
      System::err->println(JvNewStringLatin1("Unhandled Java exception:"));
      t->printStackTrace();
    }

  JackTextBook book;
  Property p;
  p.put("file","new2.mdb");
  book.open(p);

  return 0;
}


