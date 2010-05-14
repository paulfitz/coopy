#ifndef COOPY_DBG
#define COOPY_DBG

extern bool _csv_verbose;
#define dbg_printf if (_csv_verbose) printf

#endif
