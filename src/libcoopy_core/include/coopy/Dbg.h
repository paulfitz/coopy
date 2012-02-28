#ifndef COOPY_DBG
#define COOPY_DBG

#include <stdio.h>
#include <stdlib.h>

extern bool _csv_verbose;
#define dbg_printf if (_csv_verbose) printf

void coopy_set_verbose(bool verbose);
bool coopy_is_verbose();

void coopy_set_strict(bool strict);
bool coopy_is_strict();

void coopy_set_default_table_name(const char *name);
const char *coopy_get_default_table_name();
 
void coopy_print_trace(FILE *out, const char *file, int line);

#define COOPY_ASSERT(x) if (!(x)) { printf("Assertion failure %s:%d  !(%s)\n",__FILE__,__LINE__, #x ); coopy_print_trace(stdout,__FILE__,__LINE__); exit(1); }

#define COOPY_MEMORY(x) if (!(x)) { printf("Memory allocation failure %s:%d  !(%s)\n",__FILE__,__LINE__, #x ); coopy_print_trace(stdout,__FILE__,__LINE__); exit(1); }

#endif
