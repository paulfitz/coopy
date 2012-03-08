#include <stdio.h>
//#include <gnumeric-config.h>
#include <glib/gi18n.h>
#include "gnumeric.h"
#include "position.h"
#include "parse-util.h"
#include "workbook.h"
#include "workbook-priv.h"
#include "workbook-control.h"
#include "sheet.h"
#include "dependent.h"
#include "expr-name.h"
#include "libgnumeric.h"
#include "gutils.h"
//#include "gnumeric-paths.h"
#include "gnm-plugin.h"
#include "command-context.h"
#include "command-context-stderr.h"
#include "workbook-view.h"
#include "cell.h"
#include "value.h"
//#include <dialogs/dialogs.h>
#include <goffice/goffice.h>
#include <goffice/utils/go-file.h>
#include <goffice/app/io-context.h>
#include <gsf/gsf-utils.h>
#include <string.h>
#include "stf-parse.h"
#include "clipboard.h"

#include "mstyle.h"
#include "style.h"
#include "sheet-style.h"
#include "style-color.h"

#include "coopy/gnumeric_link.h"

#ifndef GNM_VERSION_FULL
#define OLD_GNUMERIC
#endif

#ifdef OLD_GNUMERIC
#define GOIOContext IOContext
#define GO_FILE_SAVE_WORKBOOK FILE_SAVE_WORKBOOK
#define GOErrorInfo ErrorInfo
#define GNM_VERSION_FULL GNUMERIC_VERSION
#define go_get_file_savers get_file_savers
#define go_get_file_openers get_file_openers
#define go_io_context_new gnumeric_io_context_new
#define wb_view_get_workbook wb_view_workbook
#define sheet_cell_create sheet_cell_new
#define PASTE_CONTENTS PASTE_CONTENT
#define DO_UNDO NULL, &tmp
#define BEGIN_UNDO GnmRelocUndo tmp;
#define END_UNDO dependents_unrelocate_free (tmp.exprs);
#else
#define DO_UNDO NULL
#define BEGIN_UNDO
#define END_UNDO
#endif

static GOErrorInfo	*plugin_errs = NULL;
static GOCmdContext	*cc = NULL;
static int gnumeric_init_ct = 0;
static int gnumeric_inited = 0;

static void gnumeric_atexit(void) {
  if (gnumeric_inited) {
    g_object_unref (cc);
    gnm_shutdown ();
    gnumeric_inited = 0;
  }
}

int gnumeric_init() {
  gnumeric_init_ct++;
  if (gnumeric_init_ct>1) return 0;
  int		 res = 0;
  int argc = 1;
  char const *argv[] = { "gnumeric", NULL };
#ifdef OLD_GNUMERIC
	gchar const **args = go_shell_argv_to_glib_encoding (argc, argv);
	gnm_pre_parse_init (args[0]);
	gnm_common_init (FALSE);
#else
	gnm_pre_parse_init (argc, argv);
	gnm_init ();
#endif
  
  cc = cmd_context_stderr_new ();
  gnm_plugins_init (GO_CMD_CONTEXT (cc));
  go_plugin_db_activate_plugin_list (
				     go_plugins_get_available_plugins (), &plugin_errs);
  atexit(gnumeric_atexit);
  return 0;
}

//static WorkbookView *wbv = NULL;

int gnumeric_fini() {
  // moved to atexit()

  //gnumeric_init_ct--;
  //if (gnumeric_init_ct>0) return 0;
}

int gnumeric_free(GnumericWorkbookPtr workbook) {
  WorkbookView *wbv = (WorkbookView *)workbook;
  if (wbv!=NULL) {
    g_object_unref (wb_view_get_workbook (wbv));
  }
  return 0;
}


GnumericWorkbookPtr gnumeric_load(const char *fname) {
  GOIOContext *io_context = go_io_context_new (cc);
  char *uri = go_filename_to_uri (fname);
  //printf("Have uri %s\n", uri);

  WorkbookView *wbv = wb_view_new_from_uri (uri, NULL,
					    io_context, NULL);
  g_free (uri);
  //printf("Have workbook view\n");
  g_object_unref (io_context);
  return wbv;
}

GnumericWorkbookPtr gnumeric_create() {
  return workbook_view_new(NULL);

}


int gnumeric_save(GnumericWorkbookPtr workbook, const char *fname,
		  const char *format) {
  WorkbookView *wbv = (WorkbookView *)workbook;
  int res = 0;
  GOFileSaver *fs = NULL;

  if (fname[0]=='-') {
      format = "Gnumeric_Excel:excel_biff8";
    }
  if (format!=NULL) {
    fs = go_file_saver_for_id(format);
    if (fs == NULL) {
      res = 1;
      g_printerr (_("Unknown exporter '%s'.\n"), format);
      return 1;
    }
  }
  if (fs==NULL) {
    fs = go_file_saver_for_file_name (fname);
    if (fs == NULL) {
      res = 2;
      g_printerr (_("Unable to guess exporter to use for '%s'.\n"
		    "Try --list-exporters to see a list of possibilities.\n"),
		  fname);
      return 1;
    }
  }
  //g_print ("Using exporter %s\n",
  //go_file_saver_get_id (fs));
  if (go_file_saver_get_save_scope (fs) !=
      GO_FILE_SAVE_WORKBOOK) {
    g_printerr (_("Selected exporter (%s) does not support saving multiple sheets in one file.\n"
		  "Only the current sheet will be saved.\n"),
		go_file_saver_get_id (fs));
  }
  char *outfile = go_filename_to_uri (fname);
  if (fname[0]=='-') {
    res = !wb_view_save_as (wbv, fs, "fd://1", cc);
  } else {
    res = !wb_view_save_as (wbv, fs, outfile, cc);
  }
  g_free (outfile);
  return 0;
}

int gnumeric_overlay_csv(GnumericWorkbookPtr workbook,
			 const char *start, const char *stop) {
  WorkbookView *wbv = (WorkbookView *)workbook;
  printf("For testing, try pasting some data into workbook\n");
  //Workbook *wb = wb_view_get_workbook (wbv);
  Sheet *sheet = wb_view_cur_sheet (wbv);
  if (sheet==NULL) { printf("no sheet!\n"); return 1; }
  StfParseOptions_t *options = stf_parse_options_guess(start);
  stf_parse_sheet(options,start,stop,sheet,0,0);
  stf_parse_options_free(options);
  options = NULL;
  return 0;
}

int gnumeric_get_sheet_count(GnumericWorkbookPtr workbook) {
  WorkbookView *wbv = (WorkbookView *)workbook;
  return workbook_sheet_count(wb_view_get_workbook(wbv));
}

GnumericSheetPtr gnumeric_get_sheet(GnumericWorkbookPtr workbook, int index) {
  WorkbookView *wbv = (WorkbookView *)workbook;
  Sheet *sheet = workbook_sheet_by_index (wb_view_get_workbook (wbv), index);
  return (GnumericSheetPtr*)sheet;
}

GnumericSheetPtr gnumeric_get_sheet_by_name(GnumericWorkbookPtr workbook, 
					    const char *name) {
  WorkbookView *wbv = (WorkbookView *)workbook;
  Sheet *sheet = workbook_sheet_by_name (wb_view_get_workbook (wbv), name);
  return (GnumericSheetPtr*)sheet;
}

GnumericSheetPtr gnumeric_add_sheet(GnumericWorkbookPtr workbook,
				    const char *name) {
  WorkbookView *wbv = (WorkbookView *)workbook;

#ifndef OLD_GNUMERIC
  int cols = gnm_conf_get_core_workbook_n_cols ();
  int rows = gnm_conf_get_core_workbook_n_rows ();
  if (!gnm_sheet_valid_size (cols, rows)) {
    gnm_sheet_suggest_size (&cols, &rows);
  }
  Sheet *sheet = workbook_sheet_add (wb_view_get_workbook (wbv), 
				     -1, cols, rows);
#else
  Sheet *sheet = workbook_sheet_add (wb_view_get_workbook (wbv), 
				     -1, TRUE);
#endif
  if (name!=NULL) {
    int idx = gnumeric_get_sheet_count(workbook)-1;
    GSList *idxs = NULL;
    GSList *names = NULL;
    idxs = g_slist_append(idxs, GINT_TO_POINTER(idx));
    names = g_slist_append(names, (gpointer)name);
    workbook_sheet_rename(wb_view_get_workbook (wbv),
			  idxs,
			  names,
			  cc);
    g_slist_free(idxs);
    g_slist_free(names);
  }
  return (GnumericSheetPtr*)sheet;
}

int gnumeric_sheet_get_size(GnumericSheetPtr sheet, int *w, int *h) {
  Sheet *s = (Sheet *)sheet;
  //GnmSheetSize const *size = gnm_sheet_get_size(sheet);
  GnmRange range = sheet_get_extent(s,FALSE);
  if (w!=NULL) *w = range.end.col+1;
  if (h!=NULL) *h = range.end.row+1;
}

char *gnumeric_sheet_get_cell_as_string(GnumericSheetPtr sheet, int x, int y) {
  GnmValue const *value = sheet_cell_get_value((Sheet*)sheet,x, y);
  if (value==NULL) return NULL;
  if (value->type == VALUE_EMPTY) return NULL;
  return value_get_as_string(value);
}

static void
coopy_add_attr (PangoAttrList *attrs, PangoAttribute *attr)
{
	attr->start_index = 0;
	attr->end_index = 2;
	pango_attr_list_insert (attrs, attr);
}

int gnumeric_sheet_set_cell_as_string(GnumericSheetPtr sheet, int x, int y,
				      const char *str) {
  GnmCell *cell = sheet_cell_get((Sheet*)sheet,x,y);
  if (cell==NULL) {
    cell = sheet_cell_create((Sheet*)sheet,x,y);
  }

  // When we have set up cell types better, we can try this:
  //   sheet_cell_set_text(cell,str,NULL);

  // For now we quote everything as strings:
  GnmValue *val = value_new_string(str);

  sheet_cell_set_value(cell,val);


  /*
  GnmStyle *mstyle;
  
  mstyle = gnm_style_new ();
  //gnm_style_set_format_text (mstyle, format);
  //gnm_style_get_pango_attrs(mstyle,NULL,1.0)
  printf("Setting WEIGHT\n");
  gnm_style_set_font_strike (mstyle, TRUE);
  //gnm_style_set_font_bold (mstyle, TRUE);
  printf("Done.\n");
  
  GnmRange r;
  r.start = r.end = cell->pos;
  sheet_style_apply_range (cell->base.sheet, &r, mstyle);
  */

  return 0;
}

int gnumeric_sheet_remove_cell(GnumericSheetPtr sheet, int x, int y) {
  GnmCell *cell = sheet_cell_get((Sheet*)sheet,x,y);
  if (cell==NULL) {
    return 0;
  }
  sheet_cell_remove((Sheet*)sheet,cell,FALSE,FALSE);
  return 0;
}


const char *gnumeric_sheet_get_name(GnumericSheetPtr sheet) {
  Sheet *s = (Sheet *)sheet;
  return s->name_unquoted;
}

void gnumeric_free_string(char *str) {
  g_free(str);
}


int gnumeric_move_column(GnumericSheetPtr sheet, int src, int dest) {
  int w, h;
  gnumeric_sheet_get_size(sheet,&w,&h);

  /*
    If dest<src:
       copy the src column
       copy from dest to src-1
         paste that offset to right by 1
       paste to dest column

    If dest>src:
       copy the src column
       copy from src+1 to dest
         paste that offset to left by 1
       paste to dest column
   */

  GnmPasteTarget pt;
  pt.sheet = sheet;
  pt.paste_flags = PASTE_CONTENTS | PASTE_COMMENTS | PASTE_NO_RECALC;
  pt.paste_flags = pt.paste_flags | PASTE_FORMATS;

  GnmRange range1, range2;
  GnmCellRegion *rcopy1, *rcopy2 = NULL;
  if (src==dest) return 0;

  // copy src column
  range1.start.row = 0;
  range1.end.row = h-1;
  range1.start.col = src;
  range1.end.col = src;
  rcopy1 = clipboard_copy_range(sheet,&range1);

  // copy src-dest range
  range2.start.row = 0;
  range2.end.row = h-1;
  if (dest<src) {
    range2.start.col = dest;
    range2.end.col = src-1;
  } else {
    range2.start.col = src+1;
    range2.end.col = dest;
  }
  rcopy2 = clipboard_copy_range(sheet,&range2);
  if (dest<src) {
    range2.start.col++;
    range2.end.col++;
  } else {
    range2.start.col--;
    range2.end.col--;
  }
  pt.range = range2;
  clipboard_paste_region(rcopy2, &pt, cc);
  cellregion_unref(rcopy2);

  range1.start.col = dest;
  range1.end.col = dest;
  pt.range = range1;
  clipboard_paste_region(rcopy1, &pt, cc);
  cellregion_unref(rcopy1);
  return 0;
}


int gnumeric_move_row(GnumericSheetPtr sheet, int src, int dest) {
  int w, h;
  gnumeric_sheet_get_size(sheet,&w,&h);

  GnmPasteTarget pt;
  pt.sheet = sheet;
  pt.paste_flags = PASTE_CONTENTS | PASTE_COMMENTS | PASTE_NO_RECALC;
  pt.paste_flags = pt.paste_flags | PASTE_FORMATS;

  GnmRange range1, range2;
  GnmCellRegion *rcopy1, *rcopy2 = NULL;
  if (src==dest) return 0;

  // copy src column
  range1.start.col = 0;
  range1.end.col = w-1;
  range1.start.row = src;
  range1.end.row = src;
  rcopy1 = clipboard_copy_range(sheet,&range1);

  // copy src-dest range
  range2.start.col = 0;
  range2.end.col = w-1;
  if (dest<src) {
    range2.start.row = dest;
    range2.end.row = src-1;
  } else {
    range2.start.row = src+1;
    range2.end.row = dest;
  }
  rcopy2 = clipboard_copy_range(sheet,&range2);
  if (dest<src) {
    range2.start.row++;
    range2.end.row++;
  } else {
    range2.start.row--;
    range2.end.row--;
  }
  pt.range = range2;
  clipboard_paste_region(rcopy2, &pt, cc);
  cellregion_unref(rcopy2);

  range1.start.row = dest;
  range1.end.row = dest;
  pt.range = range1;
  clipboard_paste_region(rcopy1, &pt, cc);
  cellregion_unref(rcopy1);
  return 0;
}



int gnumeric_insert_column(GnumericSheetPtr sheet, int before) {
  BEGIN_UNDO;
  sheet_insert_cols(sheet,before,1,DO_UNDO,cc);
  //gnumeric_sheet_get_size(sheet, &w, &h);
  END_UNDO;
  return 0;
}

int gnumeric_delete_column(GnumericSheetPtr sheet, int at) {
  BEGIN_UNDO;
  sheet_delete_cols(sheet,at,1,DO_UNDO,cc);
  END_UNDO;
  return 0;
}

int gnumeric_insert_row(GnumericSheetPtr sheet, int before) {
  BEGIN_UNDO;
  sheet_insert_rows(sheet,before,1,DO_UNDO,cc);
  END_UNDO;
  return 0;
}

int gnumeric_delete_row(GnumericSheetPtr sheet, int at) {
  BEGIN_UNDO;
  sheet_delete_rows(sheet,at,1,DO_UNDO,cc);
  END_UNDO;
  return 0;
}

int gnumeric_delete_rows(GnumericSheetPtr sheet, int first, int last) {
  printf("delete from %d to %d\n", first, last);
  BEGIN_UNDO;
  sheet_delete_rows(sheet,first,last-first+1,DO_UNDO,cc);
  END_UNDO;
  return last-first+1;
}


int gnumeric_delete_data(GnumericSheetPtr sheet) {
  BEGIN_UNDO;
  int w, h;
  gnumeric_sheet_get_size(sheet, &w, &h);
  sheet_delete_rows(sheet,0,h+1,DO_UNDO,cc);
  END_UNDO;
  return 0;
}


GnumericStylePtr gnumeric_sheet_get_style(GnumericSheetPtr sheet, 
					  int x, int y) {
  GnmStyle const *r = sheet_style_get((Sheet*)sheet,x,y);
  if (r==NULL) return NULL;
  return gnm_style_dup(r);
}

int gnumeric_sheet_set_style(GnumericSheetPtr sheet, 
			     GnumericStylePtr style,
			     int x, int y) {
  GnmRange r;
  r.start.col = x;
  r.start.row = y;
  r.end.col = x;
  r.end.row = y;
  sheet_style_apply_range ((Sheet*)sheet, &r, (GnmStyle*)style);
  return 0;
}

int gnumeric_sheet_set_row_style(GnumericSheetPtr sheet, 
				 GnumericStylePtr style,
				 int y) {
  sheet_style_apply_row ((Sheet*)sheet, y, (GnmStyle*)style);
  return 0;
}

int gnumeric_sheet_set_col_style(GnumericSheetPtr sheet, 
				 GnumericStylePtr style,
				 int x) {
  sheet_style_apply_col ((Sheet*)sheet, x, (GnmStyle*)style);
  return 0;
}

//gnumeric_style_set_font_strike(GnumericStylePtr style, int flag);

int gnumeric_style_set_font_strike(GnumericStylePtr style, int flag) {
  gnm_style_set_font_strike ((GnmStyle*)style, flag?TRUE:FALSE);
  return 0;
}

int gnumeric_style_set_font_bold(GnumericStylePtr style, int flag) {
  gnm_style_set_font_bold ((GnmStyle*)style, flag?TRUE:FALSE);
  return 0;
}

int gnumeric_style_set_font_color(GnumericStylePtr style, 
				  int r16, int g16, int b16) {
  GnmColor *color = style_color_new_i16(r16,g16,b16);
  gnm_style_set_font_color((GnmStyle*)style,color);
  return 0;
}

int gnumeric_style_set_back_color(GnumericStylePtr style, 
				  int r16, int g16, int b16) {
  GnmColor *color = style_color_new_i16(r16,g16,b16);
  gnm_style_set_back_color((GnmStyle*)style,color);
  gnm_style_set_pattern((GnmStyle*)style,1);
  return 0;
}


void gnumeric_free_style(GnumericStylePtr style) {
  gnm_style_unref((GnmStyle*)style);
}

int gnumeric_sheet_get_cell_font_color(GnumericSheetPtr sheet, int x, int y,
				       int *r16, int *g16, int *b16) {
  GnmCell *cell = sheet_cell_get((Sheet*)sheet,x,y);
  if (cell==NULL) {
    return 1;
  }
  GOColor c = gnm_cell_get_render_color(cell);
  if (!c) {
    *r16 = *g16 = *b16 = 0;
  } else {
      *r16 = GO_COLOR_UINT_R(c);
      *g16 = GO_COLOR_UINT_G(c);
      *b16 = GO_COLOR_UINT_B(c);
  }
  return 0;
}


