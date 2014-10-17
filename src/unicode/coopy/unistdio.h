#ifdef NEED_UNIMAIN
FILE *uni_fopen(const char *path, const char *mode);
#else
#define uni_fopen fopen
#endif
