
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
#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif

#define OLD_GNUMERIC

#ifdef OLD_GNUMERIC
#define GOIOContext IOContext
#define GO_FILE_SAVE_WORKBOOK FILE_SAVE_WORKBOOK
#define GOErrorInfo ErrorInfo
#define GNM_VERSION_FULL GNUMERIC_VERSION
#define go_get_file_savers get_file_savers
#define go_get_file_openers get_file_openers
//#define go_ IOContext
#endif

static GOErrorInfo	*plugin_errs = NULL;
static GOCmdContext	*cc = NULL;
//GOptionContext *ocontext;
//GError *error = NULL;

int gnumeric_init() {
  int		 res = 0;
  int argc = 1;
  char const *argv[] = { "gnumeric", NULL };
	/* No code before here, we need to init threads */
#ifdef OLD_GNUMERIC
	gchar const **args = go_shell_argv_to_glib_encoding (argc, argv);
	gnm_pre_parse_init (args[0]);
#else
	argv = gnm_pre_parse_init (argc, argv);
#endif
  gnm_common_init (FALSE);
  
  cc = cmd_context_stderr_new ();
  gnm_plugins_init (GO_CMD_CONTEXT (cc));
  go_plugin_db_activate_plugin_list (
				     go_plugins_get_available_plugins (), &plugin_errs);
  return 0;
}

static WorkbookView *wbv = NULL;


int gnumeric_fini() {
  if (wbv!=NULL) {
    g_object_unref (wb_view_workbook (wbv));
  }
  g_object_unref (cc);
  gnm_shutdown ();
}

int gnumeric_load(const char *fname) {
  IOContext *io_context = gnumeric_io_context_new (cc);
  char *uri = go_filename_to_uri (fname);
  printf("Have uri %s\n", uri);
  wbv = wb_view_new_from_uri (uri, NULL,
			      io_context, NULL);
  g_free (uri);
  printf("Have workbook view\n");
  g_object_unref (io_context);
  return 0;
}

int gnumeric_save(const char *fname) {
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
      FILE_SAVE_WORKBOOK) {
    g_printerr (_("Selected exporter (%s) does not support saving multiple sheets in one file.\n"
		  "Only the current sheet will be saved.\n"),
		go_file_saver_get_id (fs));
  }
  char *outfile = go_filename_to_uri (fname);
  res = !wb_view_save_as (wbv, fs, outfile, cc);
  g_free (outfile);
  return 0;
}

int gnumeric_overlay_csv(const char *start, const char *stop) {
    printf("For testing, try pasting some data into workbook\n");
    //Workbook *wb = wb_view_get_workbook (wbv);
    Sheet *sheet = wb_view_cur_sheet (wbv);
    if (sheet==NULL) { printf("no sheet!\n"); return 1; }
    //char data[] = "42,fortytwo\n12,twelve\n";
    //char *start = &data[0];
    //char *stop = start+strlen(data);
    StfParseOptions_t *options = stf_parse_options_guess(start);
    stf_parse_sheet(options,start,stop,sheet,0,0);
    stf_parse_options_free(options);
    options = NULL;
    return 0;
}

int gnumeric_convert (const char *in, const char *out) {
  int res = 0;
  GOFileSaver *fs = NULL;
  GOFileOpener *fo = NULL;
  char *infile = go_filename_to_uri (in);
  char *outfile = go_filename_to_uri (out);

  if (outfile != NULL) {
    fs = go_file_saver_for_file_name (outfile);
    if (fs == NULL) {
      res = 2;
      g_printerr (_("Unable to guess exporter to use for '%s'.\n"
		    "Try --list-exporters to see a list of possibilities.\n"),
		  outfile);
      return 1;
    }
    g_print ("Using exporter %s\n",
	     go_file_saver_get_id (fs));
  }
  fo = go_file_opener_for_id ("Gnumeric_stf:stf_csv");
  if (fo) {
    printf("Unknown opener\n");
    return 1;
  }
  if (fs != NULL) {
    printf("Start conversion...\n");
    IOContext *io_context = gnumeric_io_context_new (cc);
    char *uri = go_filename_to_uri (in);
    printf("Have uri %s\n", uri);
    WorkbookView *wbv = wb_view_new_from_uri (uri, fo,
					      io_context, NULL);
    g_free (uri);
    printf("Have workbook view\n");

    printf("For testing, try pasting some data into workbook\n");
    //Workbook *wb = wb_view_get_workbook (wbv);
    Sheet *sheet = wb_view_cur_sheet (wbv);
    if (sheet==NULL) { printf("no sheet!\n"); return 1; }
    char data[] = "42,fortytwo\n12,twelve\n";
    char *start = &data[0];
    char *stop = start+strlen(data);
    StfParseOptions_t *options = stf_parse_options_guess(data);
    stf_parse_sheet(options,start,stop,sheet,0,0);
    stf_parse_options_free(options);
    options = NULL;

    if (go_file_saver_get_save_scope (fs) !=
	FILE_SAVE_WORKBOOK) {
      g_printerr (_("Selected exporter (%s) does not support saving multiple sheets in one file.\n"
		    "Only the current sheet will be saved.\n"),
		  go_file_saver_get_id (fs));
    }
    res = !wb_view_save_as (wbv, fs, outfile, cc);
    g_object_unref (wb_view_workbook (wbv));
    g_object_unref (io_context);
  }
  return res;
}



int hello() {
  printf("Hello!\n");

  return 0;
}
