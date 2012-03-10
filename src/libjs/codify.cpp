
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc!=4) {
    return 1;
  }
  char *src = argv[1];
  char *dest = argv[2];
  char *tag = argv[3];

  FILE *fin = fopen(src,"r");
  FILE *fout = fopen(dest,"w");
  if (fin==NULL) return 1;
  if (fout==NULL) return 1;
  fprintf(fout, "#include <string>\n");
  fprintf(fout, "std::string %s() { \n", tag);
  fprintf(fout, "std::string result = \"");
  while (!feof(fin)) {
    char buf[1000];
    char *result = fgets(buf,sizeof(buf),fin);
    if (result!=NULL) {
      for (int i=0; i<strlen(result); i++) {
	char ch = result[i];
	switch (ch) {
	case '\r':
	case '\n':
	  break;
	case '\?':
	case '\"':
	case '\'':
	case '\\':
	  fprintf(fout, "\\%c", ch);	  
	  break;
	default:
	  fprintf(fout, "%c", ch);
	  break;
	}
      }
      fprintf(fout, "\\n\\\n");
    }
  }
  fprintf(fout, "\";\n");
  fprintf(fout, "return result;\n");
  fprintf(fout, "}\n");
  fclose(fout);
  fclose(fin);
}

