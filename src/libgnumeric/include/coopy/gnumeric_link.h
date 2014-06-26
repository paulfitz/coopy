/* vim: set sw=8: -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
#ifndef _GNUMERIC_LINK_H_
#define _GNUMERIC_LINK_H_

/**
 *
 * This is a stand-alone header file for interfacing with gnumeric's
 * spreadsheet library.  It allows basic manipulation of a workbook.
 * It includes no other header files.
 *
 */

/**
 *
 * An opaque handle for a workbook.
 *
 */
typedef void *GnumericWorkbookPtr;

/**
 *
 * An opaque handle for a sheet.
 *
 */
typedef void *GnumericSheetPtr;

/**
 *
 * An opaque handle for style information.
 *
 */
typedef void *GnumericStylePtr;

/**
 *
 * Cell content.  Representation is currently hacky, needs cleaning up.
 *
 */
typedef struct GSheetCellStruct {
	char *all;
	char *url;
	char *txt;
	int is_url;
} GSheetCell;

/**
 *
 * Handle to cell content.
 *
 */
typedef GSheetCell *GSheetCellPtr;

/**
 *
 * Set all cell content fields blank.
 *
 */
void gsheetcell_zero(GSheetCellPtr cell);

/**
 *
 * Free cell content.
 *
 */
void gsheetcell_free(GSheetCellPtr cell);

/**
 *
 * Free a string.
 *
 */
void gnumeric_free_string(char *str);

/**
 *
 * Initialize the library.
 *
 */
int gnumeric_init();

/**
 *
 * Shut down the library.
 *
 */
int gnumeric_fini();

/**
 *
 * Create a workbook.
 *
 */
GnumericWorkbookPtr gnumeric_create();

/**
 *
 * Load a workbook from a file.
 *
 */
GnumericWorkbookPtr gnumeric_load(const char *fname);

/**
 *
 * Free a workbook.
 *
 */
int gnumeric_free(GnumericWorkbookPtr workbook);

/**
 *
 * Save a workbook to a file in a given format (leave format NULL to
 * determine format from filename).
 *
 */
int gnumeric_save(GnumericWorkbookPtr workbook, 
		  const char *fname,
		  const char *format);
/**
 *
 * Get number of sheets in workbook.
 *
 */
int gnumeric_get_sheet_count(GnumericWorkbookPtr workbook);

/**
 *
 * Get a sheet from the workbook by index.
 *
 */
GnumericSheetPtr gnumeric_get_sheet(GnumericWorkbookPtr workbook, int index);

/**
 *
 * Get a sheet from the workbook by name.
 *
 */
GnumericSheetPtr gnumeric_get_sheet_by_name(GnumericWorkbookPtr workbook, 
					    const char *name);

/**
 *
 * Add a sheet to the workbook.
 *
 */
GnumericSheetPtr gnumeric_add_sheet(GnumericWorkbookPtr workbook,
				    const char *name);

/**
 *
 * Get the name of a sheet.
 *
 */
const char *gnumeric_sheet_get_name(GnumericSheetPtr sheet);

/**
 *
 * Get dimensions of a bounding box around cells with data, with
 * top-left fixed at origin.
 *
 */
int gnumeric_sheet_get_size(GnumericSheetPtr sheet, int *w, int *h);

/**
 *
 * Read a cell as a string. Call gnumeric_free_string on result.
 *
 */
char *gnumeric_sheet_get_cell_as_string(GnumericSheetPtr sheet, int x, int y);

/**
 *
 * Set a cell as a string.
 *
 */
int gnumeric_sheet_set_cell_as_string(GnumericSheetPtr sheet, int x, int y,
				      const char *str);

/**
 *
 * Read a cell.
 *
 */
int gnumeric_sheet_get_cell(GnumericSheetPtr sheet, int x, int y,
			    GSheetCellPtr cell);

/**
 *
 * Write a cell.
 *
 */
int gnumeric_sheet_set_cell(GnumericSheetPtr sheet, int x, int y,
			    GSheetCellPtr cell);

/**
 *
 * Clear a cell.
 *
 */
int gnumeric_sheet_remove_cell(GnumericSheetPtr sheet, int x, int y);

/**
 *
 * Move a column to the left of the specified destination.
 *
 */
int gnumeric_move_column(GnumericSheetPtr sheet, int src, int dest);

/**
 *
 * Insert a column to the left of the specified destination.
 *
 */
int gnumeric_insert_column(GnumericSheetPtr sheet, int before);

/**
 *
 * Delete a column at the specified destination.
 *
 */
int gnumeric_delete_column(GnumericSheetPtr sheet, int at);

/**
 *
 * Move a row before the specified destination.
 *
 */
int gnumeric_move_row(GnumericSheetPtr sheet, int src, int dest);

/**
 *
 * Insert a row before the specified destination.
 *
 */
int gnumeric_insert_row(GnumericSheetPtr sheet, int before);

/**
 *
 * Delete a row at the specified destination.
 *
 */
int gnumeric_delete_row(GnumericSheetPtr sheet, int at);

/**
 *
 * Delete rows in the given range.
 *
 */
int gnumeric_delete_rows(GnumericSheetPtr sheet, int first, int last);

/**
 *
 * Clear an entire sheet.
 *
 */
int gnumeric_delete_data(GnumericSheetPtr sheet);


/**
 *
 * Get the style of a cell.
 *
 */
GnumericStylePtr gnumeric_sheet_get_style(GnumericSheetPtr sheet, int x, int y);

/**
 *
 * Set the style of a cell.
 *
 */
int gnumeric_sheet_set_style(GnumericSheetPtr sheet, 
			     GnumericStylePtr style,
			     int x, int y);

/**
 *
 * Set the style of a row.
 *
 */
int gnumeric_sheet_set_row_style(GnumericSheetPtr sheet, 
				 GnumericStylePtr style,
				 int y);

/**
 *
 * Set the style of a column.
 *
 */
int gnumeric_sheet_set_col_style(GnumericSheetPtr sheet, 
				 GnumericStylePtr style,
				 int x);

/**
 *
 * Set font strike style.
 *
 */
int gnumeric_style_set_font_strike(GnumericStylePtr style, int flag);

/**
 *
 * Set bold style.
 *
 */
int gnumeric_style_set_font_bold(GnumericStylePtr style, int flag);

/**
 *
 * Set text color.
 *
 */
int gnumeric_style_set_font_color(GnumericStylePtr style, 
				  int r16, int g16, int b16);

/**
 *
 * Set background color.
 *
 */
int gnumeric_style_set_back_color(GnumericStylePtr style, 
				  int r16, int g16, int b16);

/**
 *
 * Free a style.
 *
 */
void gnumeric_free_style(GnumericStylePtr style);

/**
 *
 * Get the text color a cell.
 *
 */
int gnumeric_sheet_get_cell_font_color(GnumericSheetPtr sheet, int x, int y,
				       int *r, int *g, int *b);

/**
 *
 * Test function, do not use.
 *
 */
int gnumeric_overlay_csv(GnumericWorkbookPtr workbook,
			 const char *start, const char *stop);


#endif
