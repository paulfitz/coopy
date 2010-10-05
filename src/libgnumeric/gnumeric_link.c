
#include <gnumeric-config.h>
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
#include "gnumeric-paths.h"
#include "gnm-plugin.h"
#include "command-context.h"
#include "command-context-stderr.h"
#include "workbook-view.h"
#include "cell.h"
#include "value.h"
#include <dialogs/dialogs.h>
#include <goffice/goffice.h>
#include <goffice/utils/go-file.h>
#include <goffice/app/io-context.h>
#include <gsf/gsf-utils.h>
#include <string.h>
#include "stf-parse.h"

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
#endif

static GOErrorInfo	*plugin_errs = NULL;
static GOCmdContext	*cc = NULL;
static int gnumeric_init_ct = 0;

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
  return 0;
}

//static WorkbookView *wbv = NULL;


int gnumeric_fini() {
  gnumeric_init_ct--;
  if (gnumeric_init_ct>0) return 0;
  g_object_unref (cc);
  gnm_shutdown ();
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
  printf("Have uri %s\n", uri);
  WorkbookView *wbv = wb_view_new_from_uri (uri, NULL,
					    io_context, NULL);
  g_free (uri);
  printf("Have workbook view\n");
  g_object_unref (io_context);
  return wbv;
}

int gnumeric_save(GnumericWorkbookPtr workbook, const char *fname) {
  WorkbookView *wbv = (WorkbookView *)workbook;
  int res = 0;
  GOFileSaver *fs = NULL;
  fs = go_file_saver_for_file_name (fname);
  if (fs == NULL) {
    res = 2;
    g_printerr (_("Unable to guess exporter to use for '%s'.\n"
		  "Try --list-exporters to see a list of possibilities.\n"),
		fname);
    return 1;
  }
  g_print ("Using exporter %s\n",
	   go_file_saver_get_id (fs));
  if (go_file_saver_get_save_scope (fs) !=
      GO_FILE_SAVE_WORKBOOK) {
    g_printerr (_("Selected exporter (%s) does not support saving multiple sheets in one file.\n"
		  "Only the current sheet will be saved.\n"),
		go_file_saver_get_id (fs));
  }
  char *outfile = go_filename_to_uri (fname);
  res = !wb_view_save_as (wbv, fs, outfile, cc);
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

/*
GnumericSheetPtr gnumeric_sheet(GnumericWorkbookPtr workbook, int index) {
  WorkbookView *wbv = (WorkbookView *)workbook;
  Sheet *sheet = workbook_sheet_by_index (wb_view_get_workbook (wbv), 0);
  return sheet;
}
*/