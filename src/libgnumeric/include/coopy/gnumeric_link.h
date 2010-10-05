
typedef void *GnumericWorkbookPtr;

/* initialization (nesting ok) */
int gnumeric_init();
int gnumeric_fini();

GnumericWorkbookPtr gnumeric_load(const char *fname);
int gnumeric_free(GnumericWorkbookPtr workbook);

int gnumeric_save(GnumericWorkbookPtr workbook, const char *fname);
int gnumeric_overlay_csv(GnumericWorkbookPtr workbook,
			 const char *start, const char *stop);

