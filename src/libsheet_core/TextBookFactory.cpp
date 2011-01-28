#include <coopy/TextBookFactory.h>

#include <stdio.h>

using namespace coopy::store;

TextBookFactory::TextBookFactory() {
}

#define OK(x) ((x)?"yes":"no")

void AttachConfig::show() const {
  printf("cancreate? %s shouldcreate? %s overwrite? %s read? %s write? %s prev? %s\n",
	 OK(canCreate), OK(shouldCreate),
	 OK(canOverwrite), 
	 OK(shouldRead), OK(shouldWrite),
	 OK(prevBook!=NULL));
	 
}
