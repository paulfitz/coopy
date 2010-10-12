
typedef void *GnumericWorkbookPtr;
typedef void *GnumericSheetPtr;

/* initialization (nesting ok) */
int gnumeric_init();
int gnumeric_fini();

GnumericWorkbookPtr gnumeric_load(const char *fname);
int gnumeric_free(GnumericWorkbookPtr workbook);

int gnumeric_save(GnumericWorkbookPtr workbook, 
		  const char *fname,
		  const char *format);

int gnumeric_overlay_csv(GnumericWorkbookPtr workbook,
			 const char *start, const char *stop);

GnumericSheetPtr gnumeric_get_sheet(GnumericWorkbookPtr workbook, int index);

int gnumeric_sheet_get_size(GnumericSheetPtr sheet, int *w, int *h);

/* call gnumeric_free_string on result */
char *gnumeric_sheet_get_cell_as_string(GnumericSheetPtr sheet, int x, int y);

int gnumeric_sheet_set_cell_as_string(GnumericSheetPtr sheet, int x, int y,
				      const char *str);

void gnumeric_free_string(char *str);

int gnumeric_move_column(GnumericSheetPtr sheet, int src, int dest);
