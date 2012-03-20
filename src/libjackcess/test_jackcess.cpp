#include <stdio.h>

#include <gcj/cni.h>
#include <java/lang/System.h>
#include <java/io/PrintStream.h>
#include <java/lang/Throwable.h>
#include <JackBox.h>
     
#define JS(x) JvNewStringLatin1(x)

int main(int argc, char *argv[]) {
  using namespace java::lang;
  using namespace com::healthmarketscience::jackcess;
     
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

	TableBuilder *tbl = jack->makeTable(JS("foo"));
	jack->addColumn(tbl,JS("col1"),JS("string"));
	jack->addColumn(tbl,JS("col2"),JS("string"));
	jack->save(tbl,JS("new2.mdb"));
	
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
}


