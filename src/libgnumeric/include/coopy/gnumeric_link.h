
typedef void *GnumericWorkbookPtr;
typedef void *GnumericSheetPtr;
typedef void *GnumericStylePtr;

/* initialization (nesting ok) */
int gnumeric_init();
int gnumeric_fini();

GnumericWorkbookPtr gnumeric_create();
GnumericWorkbookPtr gnumeric_load(const char *fname);
int gnumeric_free(GnumericWorkbookPtr workbook);

int gnumeric_save(GnumericWorkbookPtr workbook, 
		  const char *fname,
		  const char *format);

int gnumeric_overlay_csv(GnumericWorkbookPtr workbook,
			 const char *start, const char *stop);

int gnumeric_get_sheet_count(GnumericWorkbookPtr workbook);

GnumericSheetPtr gnumeric_get_sheet(GnumericWorkbookPtr workbook, int index);

GnumericSheetPtr gnumeric_get_sheet_by_name(GnumericWorkbookPtr workbook, 
					    const char *name);

GnumericSheetPtr gnumeric_add_sheet(GnumericWorkbookPtr workbook,
				    const char *name);

int gnumeric_sheet_get_size(GnumericSheetPtr sheet, int *w, int *h);

/* call gnumeric_free_string on result */
char *gnumeric_sheet_get_cell_as_string(GnumericSheetPtr sheet, int x, int y);

int gnumeric_sheet_set_cell_as_string(GnumericSheetPtr sheet, int x, int y,
				      const char *str);

int gnumeric_sheet_remove_cell(GnumericSheetPtr sheet, int x, int y);

const char *gnumeric_sheet_get_name(GnumericSheetPtr sheet);

void gnumeric_free_string(char *str);

int gnumeric_move_column(GnumericSheetPtr sheet, int src, int dest);

int gnumeric_insert_column(GnumericSheetPtr sheet, int before);

int gnumeric_delete_column(GnumericSheetPtr sheet, int at);

int gnumeric_move_row(GnumericSheetPtr sheet, int src, int dest);

int gnumeric_insert_row(GnumericSheetPtr sheet, int before);

int gnumeric_delete_row(GnumericSheetPtr sheet, int at);

int gnumeric_delete_data(GnumericSheetPtr sheet);

GnumericStylePtr gnumeric_sheet_get_style(GnumericSheetPtr sheet, int x, int y);

int gnumeric_sheet_set_style(GnumericSheetPtr sheet, 
			     GnumericStylePtr style,
			     int x, int y);

int gnumeric_sheet_set_row_style(GnumericSheetPtr sheet, 
				 GnumericStylePtr style,
				 int y);

int gnumeric_sheet_set_col_style(GnumericSheetPtr sheet, 
				 GnumericStylePtr style,
				 int x);

int gnumeric_style_set_font_strike(GnumericStylePtr style, int flag);

int gnumeric_style_set_font_bold(GnumericStylePtr style, int flag);

int gnumeric_style_set_font_color(GnumericStylePtr style, 
				  int r16, int g16, int b16);

int gnumeric_style_set_back_color(GnumericStylePtr style, 
				  int r16, int g16, int b16);

void gnumeric_free_style(GnumericStylePtr style);
