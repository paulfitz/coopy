#include <stdio.h>

#include <gcj/cni.h>
#include <java/lang/System.h>
#include <java/io/PrintStream.h>
#include <java/lang/Throwable.h>
#include <jackcess.h>
     
int main(int argc, char *argv[]) {
  using namespace java::lang;
     
  try
    {
      JvCreateJavaVM(NULL);
      JvAttachCurrentThread(NULL, NULL);
      
      String *message = JvNewStringLatin1("Hello from C++");
      JvInitClass(&System::class$);
      System::out->println(message);
      jackcess *jack = new jackcess();
      if (jack) {
	int x = jack->hello();
	printf("I have %d\n", x);
	jack->go();
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


