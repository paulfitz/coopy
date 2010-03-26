extern void annotation_page(void);
extern void artifact_page(void);
extern void brlist_page(void);
extern void brtimeline_page(void);
extern void ci_page(void);
extern void ci_edit_page(void);
extern void tktedit_page(void);
extern void tktnew_page(void);
extern void page_dir(void);
extern void doc_page(void);
extern void diff_page(void);
extern void finfo_page(void);
extern void hexdump_page(void);
extern void home_page(void);
extern void home_page(void);
extern void info_page(void);
extern void leaves_page(void);
extern void login_page(void);
extern void logo_page(void);
extern void login_page(void);
extern void login_page(void);
extern void home_page(void);
extern void rawartifact_page(void);
extern void rcvfrom_page(void);
extern void rcvfromlist_page(void);
extern void view_list(void);
extern void view_edit(void);
extern void view_edit(void);
extern void view_see_sql(void);
extern void rptview_page(void);
extern void setup_page(void);
extern void setup_access(void);
extern void setup_behavior(void);
extern void setup_config(void);
extern void setup_editcss(void);
extern void setup_footer(void);
extern void setup_header(void);
extern void setup_logo(void);
extern void setup_skin(void);
extern void setup_sync(void);
extern void setup_timeline(void);
extern void user_edit(void);
extern void setup_ulist(void);
extern void shun_page(void);
extern void stat_page(void);
extern void page_style_css(void);
extern void taglist_page(void);
extern void tagtimeline_page(void);
extern void page_test_env(void);
extern void page_timeline(void);
extern void page_timeline_rss(void);
extern void tinfo_page(void);
extern void tktedit_page(void);
extern void tkthistory_page(void);
extern void tktnew_page(void);
extern void tktsetup_page(void);
extern void tktsetup_com_page(void);
extern void tktsetup_editpage_page(void);
extern void tktsetup_keytplt_page(void);
extern void tktsetup_newpage_page(void);
extern void tktsetup_reportlist(void);
extern void tktsetup_rpttplt_page(void);
extern void tktsetup_tab_page(void);
extern void tktsetup_timeline_page(void);
extern void tktsetup_viewpage_page(void);
extern void tkttimeline_page(void);
extern void tktview_page(void);
extern void vdiff_page(void);
extern void ci_page(void);
extern void wcontent_page(void);
extern void wdiff_page(void);
extern void wfind_page(void);
extern void whistory_page(void);
extern void wiki_page(void);
extern void wikirules_page(void);
extern void wikiappend_page(void);
extern void wikiedit_page(void);
extern void wikinew_page(void);
extern void winfo_page(void);
extern void page_xfer(void);
extern void baseline_zip_page(void);
extern void add_cmd(void);
extern void all_cmd(void);
extern void annotate_cmd(void);
extern void artifact_cmd(void);
extern void branch_cmd(void);
extern void cmd_cgi(void);
extern void changes_cmd(void);
extern void checkout_cmd(void);
extern void commit_cmd(void);
extern void clean_cmd(void);
extern void clone_cmd(void);
extern void close_cmd(void);
extern void checkout_cmd(void);
extern void commit_cmd(void);
extern void configuration_cmd(void);
extern void deconstruct_cmd(void);
extern void del_cmd(void);
extern void descendants_cmd(void);
extern void diff_cmd(void);
extern void extra_cmd(void);
extern void finfo_cmd(void);
extern void diff_cmd(void);
extern void help_cmd(void);
extern void cmd_http(void);
extern void info_cmd(void);
extern void leaves_cmd(void);
extern void ls_cmd(void);
extern void merge_cmd(void);
extern void mv_cmd(void);
extern void create_repository_cmd(void);
extern void cmd_open(void);
extern void pull_cmd(void);
extern void push_cmd(void);
extern void rebuild_database(void);
extern void reconstruct_cmd(void);
extern void redo_cmd(void);
extern void remote_url_cmd(void);
extern void mv_cmd(void);
extern void revert_cmd(void);
extern void del_cmd(void);
extern void rstats_cmd(void);
extern void scrub_cmd(void);
extern void search_cmd(void);
extern void cmd_webserver(void);
extern void setting_cmd(void);
extern void sha1sum_test(void);
extern void status_cmd(void);
extern void sync_cmd(void);
extern void tag_cmd(void);
extern void delta_3waymerge_cmd(void);
extern void test_agg_cksum_cmd(void);
extern void test_annotate_step_cmd(void);
extern void cmd_test_canonical_name(void);
extern void test_captcha(void);
extern void cmd_test_cmd_list(void);
extern void test_comment_format(void);
extern void compress_cmd(void);
extern void compress2_cmd(void);
extern void test_content_deltify_cmd(void);
extern void test_content_put_cmd(void);
extern void test_content_rawget_cmd(void);
extern void test_content_undelta_cmd(void);
extern void test_cycle_compress(void);
extern void test_decode64_cmd(void);
extern void cmd_test_delta(void);
extern void delta_apply_cmd(void);
extern void delta_create_cmd(void);
extern void test_detach_cmd(void);
extern void test_encode64_cmd(void);
extern void filezip_cmd(void);
extern void test_find_pivot(void);
extern void user_hash_passwords_cmd(void);
extern void cmd_test_http(void);
extern void import_manifest_cmd(void);
extern void isspace_cmd(void);
extern void md5sum_test(void);
extern void test_name_to_id(void);
extern void manifest_test_parse_cmd(void);
extern void test_rawdiff_cmd(void);
extern void cmd_test_relative_name(void);
extern void testtag_cmd(void);
extern void test_th_render(void);
extern void cmd_test_tree_name(void);
extern void test_udiff_cmd(void);
extern void uncompress_cmd(void);
extern void cmd_test_urlparser(void);
extern void verify_all_cmd(void);
extern void test_wiki_render(void);
extern void cmd_test_xfer(void);
extern void timeline_cmd(void);
extern void cmd_webserver(void);
extern void undo_cmd(void);
extern void setting_cmd(void);
extern void update_cmd(void);
extern void user_cmd(void);
extern void version_cmd(void);
extern void wiki_cmd(void);
extern void baseline_zip_cmd(void);
typedef struct NameMap NameMap;
struct NameMap {
  const char *zName;
  void (*xFunc)(void);
};
static const NameMap aWebpage[] = {
  { "annotate",                  annotation_page },
  { "artifact",                  artifact_page },
  { "brlist",                    brlist_page },
  { "brtimeline",                brtimeline_page },
  { "ci",                        ci_page },
  { "ci_edit",                   ci_edit_page },
  { "debug_tktedit",             tktedit_page },
  { "debug_tktnew",              tktnew_page },
  { "dir",                       page_dir },
  { "doc",                       doc_page },
  { "fdiff",                     diff_page },
  { "finfo",                     finfo_page },
  { "hexdump",                   hexdump_page },
  { "home",                      home_page },
  { "index",                     home_page },
  { "info",                      info_page },
  { "leaves",                    leaves_page },
  { "login",                     login_page },
  { "logo",                      logo_page },
  { "logout",                    login_page },
  { "my",                        login_page },
  { "not_found",                 home_page },
  { "raw",                       rawartifact_page },
  { "rcvfrom",                   rcvfrom_page },
  { "rcvfromlist",               rcvfromlist_page },
  { "reportlist",                view_list },
  { "rptedit",                   view_edit },
  { "rptnew",                    view_edit },
  { "rptsql",                    view_see_sql },
  { "rptview",                   rptview_page },
  { "setup",                     setup_page },
  { "setup_access",              setup_access },
  { "setup_behavior",            setup_behavior },
  { "setup_config",              setup_config },
  { "setup_editcss",             setup_editcss },
  { "setup_footer",              setup_footer },
  { "setup_header",              setup_header },
  { "setup_logo",                setup_logo },
  { "setup_skin",                setup_skin },
  { "setup_sync",                setup_sync },
  { "setup_timeline",            setup_timeline },
  { "setup_uedit",               user_edit },
  { "setup_ulist",               setup_ulist },
  { "shun",                      shun_page },
  { "stat",                      stat_page },
  { "style.css",                 page_style_css },
  { "taglist",                   taglist_page },
  { "tagtimeline",               tagtimeline_page },
  { "test_env",                  page_test_env },
  { "timeline",                  page_timeline },
  { "timeline.rss",              page_timeline_rss },
  { "tinfo",                     tinfo_page },
  { "tktedit",                   tktedit_page },
  { "tkthistory",                tkthistory_page },
  { "tktnew",                    tktnew_page },
  { "tktsetup",                  tktsetup_page },
  { "tktsetup_com",              tktsetup_com_page },
  { "tktsetup_editpage",         tktsetup_editpage_page },
  { "tktsetup_keytplt",          tktsetup_keytplt_page },
  { "tktsetup_newpage",          tktsetup_newpage_page },
  { "tktsetup_reportlist",       tktsetup_reportlist },
  { "tktsetup_rpttplt",          tktsetup_rpttplt_page },
  { "tktsetup_tab",              tktsetup_tab_page },
  { "tktsetup_timeline",         tktsetup_timeline_page },
  { "tktsetup_viewpage",         tktsetup_viewpage_page },
  { "tkttimeline",               tkttimeline_page },
  { "tktview",                   tktview_page },
  { "vdiff",                     vdiff_page },
  { "vinfo",                     ci_page },
  { "wcontent",                  wcontent_page },
  { "wdiff",                     wdiff_page },
  { "wfind",                     wfind_page },
  { "whistory",                  whistory_page },
  { "wiki",                      wiki_page },
  { "wiki_rules",                wikirules_page },
  { "wikiappend",                wikiappend_page },
  { "wikiedit",                  wikiedit_page },
  { "wikinew",                   wikinew_page },
  { "winfo",                     winfo_page },
  { "xfer",                      page_xfer },
  { "zip",                       baseline_zip_page },
};
static const NameMap aCommand[] = {
  { "add",                       add_cmd },
  { "all",                       all_cmd },
  { "annotate",                  annotate_cmd },
  { "artifact",                  artifact_cmd },
  { "branch",                    branch_cmd },
  { "cgi",                       cmd_cgi },
  { "changes",                   changes_cmd },
  { "checkout",                  checkout_cmd },
  { "ci",                        commit_cmd },
  { "clean",                     clean_cmd },
  { "clone",                     clone_cmd },
  { "close",                     close_cmd },
  { "co",                        checkout_cmd },
  { "commit",                    commit_cmd },
  { "configuration",             configuration_cmd },
  { "deconstruct",               deconstruct_cmd },
  { "del",                       del_cmd },
  { "descendants",               descendants_cmd },
  { "diff",                      diff_cmd },
  { "extras",                    extra_cmd },
  { "finfo",                     finfo_cmd },
  { "gdiff",                     diff_cmd },
  { "help",                      help_cmd },
  { "http",                      cmd_http },
  { "info",                      info_cmd },
  { "leaves",                    leaves_cmd },
  { "ls",                        ls_cmd },
  { "merge",                     merge_cmd },
  { "mv",                        mv_cmd },
  { "new",                       create_repository_cmd },
  { "open",                      cmd_open },
  { "pull",                      pull_cmd },
  { "push",                      push_cmd },
  { "rebuild",                   rebuild_database },
  { "reconstruct",               reconstruct_cmd },
  { "redo",                      redo_cmd },
  { "remote-url",                remote_url_cmd },
  { "rename",                    mv_cmd },
  { "revert",                    revert_cmd },
  { "rm",                        del_cmd },
  { "rstats",                    rstats_cmd },
  { "scrub",                     scrub_cmd },
  { "search",                    search_cmd },
  { "server",                    cmd_webserver },
  { "settings",                  setting_cmd },
  { "sha1sum",                   sha1sum_test },
  { "status",                    status_cmd },
  { "sync",                      sync_cmd },
  { "tag",                       tag_cmd },
  { "test-3-way-merge",          delta_3waymerge_cmd },
  { "test-agg-cksum",            test_agg_cksum_cmd },
  { "test-annotate-step",        test_annotate_step_cmd },
  { "test-canonical-name",       cmd_test_canonical_name },
  { "test-captcha",              test_captcha },
  { "test-commands",             cmd_test_cmd_list },
  { "test-comment-format",       test_comment_format },
  { "test-compress",             compress_cmd },
  { "test-compress-2",           compress2_cmd },
  { "test-content-deltify",      test_content_deltify_cmd },
  { "test-content-put",          test_content_put_cmd },
  { "test-content-rawget",       test_content_rawget_cmd },
  { "test-content-undelta",      test_content_undelta_cmd },
  { "test-cycle-compress",       test_cycle_compress },
  { "test-decode64",             test_decode64_cmd },
  { "test-delta",                cmd_test_delta },
  { "test-delta-apply",          delta_apply_cmd },
  { "test-delta-create",         delta_create_cmd },
  { "test-detach",               test_detach_cmd },
  { "test-encode64",             test_encode64_cmd },
  { "test-filezip",              filezip_cmd },
  { "test-find-pivot",           test_find_pivot },
  { "test-hash-passwords",       user_hash_passwords_cmd },
  { "test-http",                 cmd_test_http },
  { "test-import-manifest",      import_manifest_cmd },
  { "test-isspace",              isspace_cmd },
  { "test-md5sum",               md5sum_test },
  { "test-name-to-id",           test_name_to_id },
  { "test-parse-manifest",       manifest_test_parse_cmd },
  { "test-rawdiff",              test_rawdiff_cmd },
  { "test-relative-name",        cmd_test_relative_name },
  { "test-tag",                  testtag_cmd },
  { "test-th-render",            test_th_render },
  { "test-tree-name",            cmd_test_tree_name },
  { "test-udiff",                test_udiff_cmd },
  { "test-uncompress",           uncompress_cmd },
  { "test-urlparser",            cmd_test_urlparser },
  { "test-verify-all",           verify_all_cmd },
  { "test-wiki-render",          test_wiki_render },
  { "test-xfer",                 cmd_test_xfer },
  { "timeline",                  timeline_cmd },
  { "ui",                        cmd_webserver },
  { "undo",                      undo_cmd },
  { "unset",                     setting_cmd },
  { "update",                    update_cmd },
  { "user",                      user_cmd },
  { "version",                   version_cmd },
  { "wiki",                      wiki_cmd },
  { "zip",                       baseline_zip_cmd },
};
static const char zHelp_add_cmd[] = 
  "Usage: %fossil add FILE...\n"
  "\n"
  "Make arrangements to add one or more files to the current checkout\n"
  "at the next commit.\n"
  "\n"
  "When adding files recursively, filenames that begin with \".\" are\n"
  "excluded by default.  To include such files, add the \"--dotfiles\"\n"
  "option to the command-line.";
static const char zHelp_all_cmd[] = 
  "Usage: %fossil all (list|ls|pull|push|rebuild|sync)\n"
  "\n"
  "The ~/.fossil file records the location of all repositories for a\n"
  "user.  This command performs certain operations on all repositories\n"
  "that can be useful before or after a period of disconnection operation.\n"
  "Available operations are:\n"
  "\n"
  "   list       Display the location of all repositories\n"
  "\n"
  "   ls         An alias for \"list\"\n"
  "\n"
  "   pull       Run a \"pull\" operation on all repositories\n"
  "\n"
  "   push       Run a \"push\" on all repositories\n"
  "\n"
  "   rebuild    Rebuild on all repositories\n"
  "\n"
  "   sync       Run a \"sync\" on all repositories\n"
  "\n"
  "Respositories are automatically added to the set of known repositories\n"
  "when one of the following commands against the repository: clone, info,\n"
  "pull, push, or sync";
static const char zHelp_annotate_cmd[] = 
  "COMMAND: annotate\n"
  "\n"
  "%fossil annotate FILENAME\n"
  "\n"
  "Output the text of a file with markings to show when each line of\n"
  "the file was introduced.";
static const char zHelp_artifact_cmd[] = 
  "Usage: %fossil artifact ARTIFACT-ID  ?OUTPUT-FILENAME?\n"
  "\n"
  "Extract an artifact by its SHA1 hash and write the results on\n"
  "standard output, or if the optional 4th argument is given, in\n"
  "the named output file.";
static const char zHelp_branch_cmd[] = 
  "Usage: %fossil branch SUBCOMMAND ... ?-R|--repository FILE?\n"
  "\n"
  "Run various subcommands on the branches of the open repository or\n"
  "of the repository identified by the -R or --repository option.\n"
  "\n"
  "   %fossil branch new BRANCH-NAME BASIS ?-bgcolor COLOR? \n"
  "\n"
  "       Create a new branch BRANCH-NAME off of check-in BASIS.\n"
  "       You can optionally give the branch a default color.\n"
  "\n"
  "   %fossil branch list\n"
  "\n"
  "       List all branches";
static const char zHelp_cmd_cgi[] = 
  "Usage: %fossil ?cgi? SCRIPT\n"
  "\n"
  "The SCRIPT argument is the name of a file that is the CGI script\n"
  "that is being run.  The command name, \"cgi\", may be omitted if\n"
  "the GATEWAY_INTERFACE environment variable is set to \"CGI\" (which\n"
  "should always be the case for CGI scripts run by a webserver.)  The\n"
  "SCRIPT file should look something like this:\n"
  "\n"
  "     #!/usr/bin/fossil\n"
  "     repository: /home/somebody/project.db\n"
  "\n"
  "The second line defines the name of the repository.  After locating\n"
  "the repository, fossil will generate a webpage on stdout based on\n"
  "the values of standard CGI environment variables.";
static const char zHelp_changes_cmd[] = 
  "Usage: %fossil changes\n"
  "\n"
  "Report on the edit status of all files in the current checkout.\n"
  "See also the \"status\" and \"extra\" commands.";
static const char zHelp_checkout_cmd[] = 
  "Usage: %fossil checkout VERSION ?-f|--force? ?--keep?\n"
  "\n"
  "Check out a version specified on the command-line.  This command\n"
  "will abort if there are edited files in the current checkout unless\n"
  "the --force option appears on the command-line.  The --keep option\n"
  "leaves files on disk unchanged, except the manifest and manifest.uuid\n"
  "files.\n"
  "\n"
  "The --latest flag can be used in place of VERSION to checkout the\n"
  "latest version in the repository.\n"
  "\n"
  "See also the \"update\" command.";
static const char zHelp_commit_cmd[] = 
  "Usage: %fossil commit ?OPTIONS? ?FILE...?\n"
  "\n"
  "Create a new version containing all of the changes in the current\n"
  "checkout.  You will be prompted to enter a check-in comment unless\n"
  "one of the \"-m\" or \"-M\" options are used to specify a comment.\n"
  "\"-m\" takes a single string for the commit message and \"-M\" requires\n"
  "a filename from which to read the commit message. If neither \"-m\"\n"
  "nor \"-M\" are specified then the editor defined in the \"editor\"\n"
  "fossil option (see %fossil help set) will be used, or from the\n"
  "\"VISUAL\" or \"EDITOR\" environment variables (in that order) if no\n"
  "editor is set.\n"
  "\n"
  "You will be prompted for your GPG passphrase in order to sign the\n"
  "new manifest unless the \"--nosign\" options is used.  All files that\n"
  "have changed will be committed unless some subset of files is\n"
  "specified on the command line.\n"
  "\n"
  "The --branch option followed by a branch name cases the new check-in\n"
  "to be placed in the named branch.  The --bgcolor option can be followed\n"
  "by a color name (ex:  '#ffc0c0') to specify the background color of\n"
  "entries in the new branch when shown in the web timeline interface.\n"
  "\n"
  "A check-in is not permitted to fork unless the --force or -f\n"
  "option appears.  A check-in is not allowed against a closed check-in.\n"
  "\n"
  "The --private option creates a private check-in that is never synced.\n"
  "Children of private check-ins are automatically private.\n"
  "\n"
  "Options:\n"
  "\n"
  "   --comment|-m COMMENT-TEXT\n"
  "   --branch NEW-BRANCH-NAME\n"
  "   --bgcolor COLOR\n"
  "   --nosign\n"
  "   --force|-f\n"
  "   --private\n"
  "   --message-file|-M COMMENT-FILE";
static const char zHelp_clean_cmd[] = 
  "Usage: %fossil clean ?--force? ?--dotfiles?\n"
  "\n"
  "Delete all \"extra\" files in the source tree.  \"Extra\" files are\n"
  "files that are not officially part of the checkout.  See also\n"
  "the \"extra\" command. This operation cannot be undone. \n"
  "\n"
  "You will be prompted before removing each file. If you are\n"
  "sure you wish to remove all \"extra\" files you can specify the\n"
  "optional --force flag and no prompts will be issued.\n"
  "\n"
  "Files and subdirectories whose names begin with \".\" are\n"
  "normally ignored.  They are included if the \"--dotfiles\" option\n"
  "is used.";
static const char zHelp_clone_cmd[] = 
  "Usage: %fossil clone ?OPTIONS? URL FILENAME\n"
  "\n"
  "Make a clone of a repository specified by URL in the local\n"
  "file named FILENAME.  \n"
  "\n"
  "By default, your current login name is used to create the default\n"
  "admin user. This can be overridden using the -A|--admin-user\n"
  "parameter.\n"
  "\n"
  "Options:\n"
  "\n"
  "   --admin-user|-A USERNAME";
static const char zHelp_close_cmd[] = 
  "Usage: %fossil close ?-f|--force?\n"
  "\n"
  "The opposite of \"open\".  Close the current database connection.\n"
  "Require a -f or --force flag if there are unsaved changed in the\n"
  "current check-out.";
static const char zHelp_configuration_cmd[] = 
  "Usage: %fossil configure METHOD ...\n"
  "\n"
  "Where METHOD is one of: export import merge pull push reset.  All methods\n"
  "accept the -R or --repository option to specific a repository.\n"
  "\n"
  "   %fossil configuration export AREA FILENAME\n"
  "\n"
  "        Write to FILENAME exported configuraton information for AREA.\n"
  "        AREA can be one of:  all ticket skin project\n"
  "\n"
  "   %fossil configuration import FILENAME\n"
  "\n"
  "        Read a configuration from FILENAME, overwriting the current\n"
  "        configuration.\n"
  "\n"
  "   %fossil configuration merge FILENAME\n"
  "\n"
  "        Read a configuration from FILENAME and merge its values into\n"
  "        the current configuration.  Existing values take priority over\n"
  "        values read from FILENAME.\n"
  "\n"
  "   %fossil configuration pull AREA ?URL?\n"
  "\n"
  "        Pull and install the configuration from a different server\n"
  "        identified by URL.  If no URL is specified, then the default\n"
  "        server is used. \n"
  "\n"
  "   %fossil configuration push AREA ?URL?\n"
  "\n"
  "        Push the local configuration into the remote server identified\n"
  "        by URL.  Admin privilege is required on the remote server for\n"
  "        this to work.\n"
  "\n"
  "   %fossil configuration reset AREA\n"
  "\n"
  "        Restore the configuration to the default.  AREA as above.\n"
  "\n"
  "WARNING: Do not import, merge, or pull configurations from an untrusted\n"
  "source.  The inbound configuration is not checked for safety and can\n"
  "introduce security vulnerabilities.";
static const char zHelp_deconstruct_cmd[] = 
  "COMMAND: deconstruct \n"
  "Usage %fossil deconstruct ?-R|--repository REPOSITORY? DESTINATION\n"
  "\n"
  "Populates the indicated DESTINATION directory with copies of all\n"
  "artifcats contained within the repository.  Artifacts are named AA/bbbbb\n"
  "where AA is the first 2 characters of the artifact ID and bbbbb is the\n"
  "remaining 38 characters.";
static const char zHelp_del_cmd[] = 
  "Usage: %fossil rm FILE...\n"
  "   or: %fossil del FILE...\n"
  "\n"
  "Remove one or more files from the tree.\n"
  "\n"
  "This command does not remove the files from disk.  It just marks the\n"
  "files as no longer being part of the project.  In other words, future\n"
  "changes to the named files will not be versioned.";
static const char zHelp_descendants_cmd[] = 
  "Usage: %fossil descendants ?BASELINE-ID?\n"
  "\n"
  "Find all leaf descendants of the baseline specified or if the argument\n"
  "is omitted, of the baseline currently checked out.";
static const char zHelp_diff_cmd[] = 
  "Usage: %fossil diff|gdiff ?options? ?FILE?\n"
  "\n"
  "Show the difference between the current version of FILE (as it\n"
  "exists on disk) and that same file as it was checked out.  Or\n"
  "if the FILE argument is omitted, show the unsaved changed currently\n"
  "in the working check-out.\n"
  "\n"
  "If the \"--from VERSION\" or \"-r VERSION\" option is used it specifies\n"
  "the source check-in for the diff operation.  If not specified, the \n"
  "source check-in is the base check-in for the current check-out.\n"
  "\n"
  "If the \"--to VERSION\" option appears, it specifies the check-in from\n"
  "which the second version of the file or files is taken.  If there is\n"
  "no \"--to\" option then the (possibly edited) files in the current check-out\n"
  "are used.\n"
  "\n"
  "The \"-i\" command-line option forces the use of the internal diff logic\n"
  "rather than any external diff program that might be configured using\n"
  "the \"setting\" command.  If no external diff program is configured, then\n"
  "the \"-i\" option is a no-op.  The \"-i\" option converts \"gdiff\" into \"diff\".";
static const char zHelp_extra_cmd[] = 
  "Usage: %fossil extras ?--dotfiles? ?--ignore GLOBPATTERN?\n"
  "\n"
  "Print a list of all files in the source tree that are not part of\n"
  "the current checkout.  See also the \"clean\" command.\n"
  "\n"
  "Files and subdirectories whose names begin with \".\" are normally\n"
  "ignored but can be included by adding the --dotfiles option.";
static const char zHelp_finfo_cmd[] = 
  "Usage: %fossil finfo FILENAME\n"
  "\n"
  "Print the change history for a single file.\n"
  "\n"
  "The \"--limit N\" and \"--offset P\" options limits the output to the first\n"
  "N changes after skipping P changes.";
static const char zHelp_help_cmd[] = 
  "Usage: %fossil help COMMAND\n"
  "\n"
  "Display information on how to use COMMAND";
static const char zHelp_cmd_http[] = 
  "Usage: %fossil http REPOSITORY [--notfound URL]\n"
  "\n"
  "Handle a single HTTP request appearing on stdin.  The resulting webpage\n"
  "is delivered on stdout.  This method is used to launch an HTTP request\n"
  "handler from inetd, for example.  The argument is the name of the \n"
  "repository.\n"
  "\n"
  "If REPOSITORY is a directory that contains one or more respositories\n"
  "with names of the form \"*.fossil\" then the first element of the URL\n"
  "pathname selects among the various repositories.  If the pathname does\n"
  "not select a valid repository and the --notfound option is available,\n"
  "then the server redirects (HTTP code 302) to the URL of --notfound.";
static const char zHelp_info_cmd[] = 
  "Usage: %fossil info ?ARTIFACT-ID|FILENAME?\n"
  "\n"
  "With no arguments, provide information about the current tree.\n"
  "If an argument is specified, provide information about the object\n"
  "in the respository of the current tree that the argument refers\n"
  "to.  Or if the argument is the name of a repository, show\n"
  "information about that repository.";
static const char zHelp_leaves_cmd[] = 
  "Usage: %fossil leaves ?--all? ?--closed?\n"
  "\n"
  "Find leaves of all branches.  By default show only open leaves.\n"
  "The --all flag causes all leaves (closed and open) to be shown.\n"
  "The --closed flag shows only closed leaves.";
static const char zHelp_ls_cmd[] = 
  "Usage: %fossil ls [-l]\n"
  "\n"
  "Show the names of all files in the current checkout.  The -l provides\n"
  "extra information about each file.";
static const char zHelp_merge_cmd[] = 
  "Usage: %fossil merge [--cherrypick] [--backout] VERSION\n"
  "\n"
  "The argument is a version that should be merged into the current\n"
  "checkout.  All changes from VERSION back to the nearest common\n"
  "ancestor are merged.  Except, if either of the --cherrypick or\n"
  "--backout options are used only the changes associated with the\n"
  "single check-in VERSION are merged.  The --backout option causes\n"
  "the changes associated with VERSION to be removed from the current\n"
  "checkout rather than added.\n"
  "\n"
  "Only file content is merged.  The result continues to use the\n"
  "file and directory names from the current checkout even if those\n"
  "names might have been changed in the branch being merged in.\n"
  "\n"
  "Other options:\n"
  "\n"
  "  --detail                Show additional details of the merge\n"
  "\n"
  "  --binary GLOBPATTERN    Treat files that match GLOBPATTERN as binary\n"
  "                          and do not try to merge parallel changes.  This\n"
  "                          option overrides the \"binary-glob\" setting.";
static const char zHelp_mv_cmd[] = 
  "Usage: %fossil mv|rename OLDNAME NEWNAME\n"
  "   or: %fossil mv|rename OLDNAME... DIR\n"
  "\n"
  "Move or rename one or more files within the tree\n"
  "\n"
  "This command does not rename the files on disk.  All this command does is\n"
  "record the fact that filenames have changed so that appropriate notations\n"
  "can be made at the next commit/checkin.";
static const char zHelp_create_repository_cmd[] = 
  "Usage: %fossil new ?OPTIONS? FILENAME\n"
  "\n"
  "Create a repository for a new project in the file named FILENAME.\n"
  "This command is distinct from \"clone\".  The \"clone\" command makes\n"
  "a copy of an existing project.  This command starts a new project.\n"
  "\n"
  "By default, your current login name is used to create the default\n"
  "admin user. This can be overridden using the -A|--admin-user\n"
  "parameter.\n"
  "\n"
  "Options:\n"
  "\n"
  "   --admin-user|-A USERNAME\n"
  "   --date-override DATETIME";
static const char zHelp_cmd_open[] = 
  "Usage: %fossil open FILENAME ?VERSION? ?--keep?\n"
  "\n"
  "Open a connection to the local repository in FILENAME.  A checkout\n"
  "for the repository is created with its root at the working directory.\n"
  "If VERSION is specified then that version is checked out.  Otherwise\n"
  "the latest version is checked out.  No files other than \"manifest\"\n"
  "and \"manifest.uuid\" are modified if the --keep option is present.\n"
  "\n"
  "See also the \"close\" command.";
static const char zHelp_pull_cmd[] = 
  "Usage: %fossil pull ?URL? ?options?\n"
  "\n"
  "Pull changes from a remote repository into the local repository.\n"
  "Use the \"-R REPO\" or \"--repository REPO\" command-line options\n"
  "to specify an alternative repository file.\n"
  "\n"
  "If the URL is not specified, then the URL from the most recent\n"
  "clone, push, pull, remote-url, or sync command is used.\n"
  "\n"
  "The URL specified normally becomes the new \"remote-url\" used for\n"
  "subsequent push, pull, and sync operations.  However, the \"--once\"\n"
  "command-line option makes the URL a one-time-use URL that is not\n"
  "saved.\n"
  "\n"
  "See also: clone, push, sync, remote-url";
static const char zHelp_push_cmd[] = 
  "Usage: %fossil push ?URL? ?options?\n"
  "\n"
  "Push changes in the local repository over into a remote repository.\n"
  "Use the \"-R REPO\" or \"--repository REPO\" command-line options\n"
  "to specify an alternative repository file.\n"
  "\n"
  "If the URL is not specified, then the URL from the most recent\n"
  "clone, push, pull, remote-url, or sync command is used.\n"
  "\n"
  "The URL specified normally becomes the new \"remote-url\" used for\n"
  "subsequent push, pull, and sync operations.  However, the \"--once\"\n"
  "command-line option makes the URL a one-time-use URL that is not\n"
  "saved.\n"
  "\n"
  "See also: clone, pull, sync, remote-url";
static const char zHelp_rebuild_database[] = 
  "Usage: %fossil rebuild ?REPOSITORY?\n"
  "\n"
  "Reconstruct the named repository database from the core\n"
  "records.  Run this command after updating the fossil\n"
  "executable in a way that changes the database schema.";
static const char zHelp_reconstruct_cmd[] = 
  "COMMAND: reconstruct \n"
  "Usage %fossil reconstruct REPOSITORY ORIGIN\n"
  "\n"
  "Creates the REPOSITORY and populates it with the artifacts in the\n"
  "indicated ORIGIN directory.";
static const char zHelp_redo_cmd[] = 
  "Usage: %fossil redo ?FILENAME...?\n"
  "\n"
  "Redo the an update or merge or revert operation that has been undone\n"
  "by the undo command.  If FILENAME is specified then restore the changes\n"
  "associated with the named file(s) but otherwise leave the update\n"
  "or merge undone.\n"
  "\n"
  "A single level of undo/redo is supported.  The undo/redo stack\n"
  "is cleared by the commit and checkout commands.";
static const char zHelp_remote_url_cmd[] = 
  "Usage: %fossil remote-url ?URL|off?\n"
  "\n"
  "Query and/or change the default server URL used by the \"pull\", \"push\",\n"
  "and \"sync\" commands.\n"
  "\n"
  "The remote-url is set automatically by a \"clone\" command or by any\n"
  "\"sync\", \"push\", or \"pull\" command that specifies an explicit URL.\n"
  "The default remote-url is used by auto-syncing and by \"sync\", \"push\",\n"
  "\"pull\" that omit the server URL.\n"
  "\n"
  "See also: clone, push, pull, sync";
static const char zHelp_revert_cmd[] = 
  "Usage: %fossil revert ?-r REVISION? FILE ...\n"
  "\n"
  "Revert to the current repository version of FILE, or to\n"
  "the version associated with baseline REVISION if the -r flag\n"
  "appears.\n"
  "\n"
  "If a file is reverted accidently, it can be restored using\n"
  "the \"fossil undo\" command.";
static const char zHelp_rstats_cmd[] = 
  "Usage: %fossil rstats\n"
  "\n"
  "Deliver a report of the repository statistics for the\n"
  "current checkout.";
static const char zHelp_scrub_cmd[] = 
  "COMMAND: scrub\n"
  "%fossil scrub [--verily] [--force] [REPOSITORY]\n"
  "\n"
  "The command removes sensitive information (such as passwords) from a\n"
  "repository so that the respository can be sent to an untrusted reader.\n"
  "\n"
  "By default, only passwords are removed.  However, if the --verily option\n"
  "is added, then private branches, concealed email addresses, IP\n"
  "addresses of correspondents, and similar privacy-sensitive fields\n"
  "are also purged.\n"
  "\n"
  "This command permanently deletes the scrubbed information.  The effects\n"
  "of this command are irreversible.  Use with caution.\n"
  "\n"
  "The user is prompted to confirm the scrub unless the --force option\n"
  "is used.";
static const char zHelp_search_cmd[] = 
  "COMMAND: search\n"
  "%fossil search pattern...\n"
  "\n"
  "Search for timeline entries matching the pattern.";
static const char zHelp_cmd_webserver[] = 
  "Usage: %fossil server ?-P|--port TCPPORT? ?REPOSITORY?\n"
  "   Or: %fossil ui ?-P|--port TCPPORT? ?REPOSITORY?\n"
  "\n"
  "Open a socket and begin listening and responding to HTTP requests on\n"
  "TCP port 8080, or on any other TCP port defined by the -P or\n"
  "--port option.  The optional argument is the name of the repository.\n"
  "The repository argument may be omitted if the working directory is\n"
  "within an open checkout.\n"
  "\n"
  "The \"ui\" command automatically starts a web browser after initializing\n"
  "the web server.\n"
  "\n"
  "In the \"server\" command, the REPOSITORY can be a directory (aka folder)\n"
  "that contains one or more respositories with names ending in \".fossil\".\n"
  "In that case, the first element of the URL is used to select among the\n"
  "various repositories.";
static const char zHelp_setting_cmd[] = 
  "COMMAND: settings\n"
  "COMMAND: unset\n"
  "%fossil setting ?PROPERTY? ?VALUE? ?-global?\n"
  "%fossil unset PROPERTY ?-global?\n"
  "\n"
  "The \"setting\" command with no arguments lists all properties and their\n"
  "values.  With just a property name it shows the value of that property.\n"
  "With a value argument it changes the property for the current repository.\n"
  "\n"
  "The \"unset\" command clears a property setting.\n"
  "\n"
  "\n"
  "   auto-captcha     If enabled, the Login page will provide a button\n"
  "                    which uses JavaScript to fill out the captcha for\n"
  "                    the \"anonymous\" user. (Most bots cannot use JavaScript.)\n"
  "\n"
  "   autosync         If enabled, automatically pull prior to commit\n"
  "                    or update and automatically push after commit or\n"
  "                    tag or branch creation.  If the the value is \"pullonly\"\n"
  "                    then only pull operations occur automatically.\n"
  "\n"
  "   binary-glob      The VALUE is a comma-separated list of GLOB patterns\n"
  "                    that should be treated as binary files for merging\n"
  "                    purposes.  Example:   *.xml\n"
  "\n"
  "   clearsign        When enabled, fossil will attempt to sign all commits\n"
  "                    with gpg.  When disabled (the default), commits will\n"
  "                    be unsigned.\n"
  "\n"
  "   diff-command     External command to run when performing a diff.\n"
  "                    If undefined, the internal text diff will be used.\n"
  "\n"
  "   dont-push        Prevent this repository from pushing from client to\n"
  "                    server.  Useful when setting up a private branch.\n"
  "\n"
  "   editor           Text editor command used for check-in comments.\n"
  "\n"
  "   gdiff-command    External command to run when performing a graphical\n"
  "                    diff. If undefined, text diff will be used.\n"
  "\n"
  "   http-port        The TCP/IP port number to use by the \"server\"\n"
  "                    and \"ui\" commands.  Default: 8080\n"
  "\n"
  "   ignore-glob      The VALUE is a comma-separated list of GLOB patterns\n"
  "                    specifying files that the \"extra\" command will ignore.\n"
  "                    Example:  *.o,*.obj,*.exe\n"
  "\n"
  "   localauth        If enabled, require that HTTP connections from\n"
  "                    127.0.0.1 be authenticated by password.  If\n"
  "                    false, all HTTP requests from localhost have\n"
  "                    unrestricted access to the repository.\n"
  "\n"
  "   mtime-changes    Use file modification times (mtimes) to detect when\n"
  "                    files have been modified.  (Default \"on\".)\n"
  "\n"
  "   pgp-command      Command used to clear-sign manifests at check-in.\n"
  "                    The default is \"gpg --clearsign -o \".\n"
  "\n"
  "   proxy            URL of the HTTP proxy.  If undefined or \"off\" then\n"
  "                    the \"http_proxy\" environment variable is consulted.\n"
  "                    If the http_proxy environment variable is undefined\n"
  "                    then a direct HTTP connection is used.\n"
  "\n"
  "   web-browser      A shell command used to launch your preferred\n"
  "                    web browser when given a URL as an argument.\n"
  "                    Defaults to \"start\" on windows, \"open\" on Mac,\n"
  "                    and \"firefox\" on Unix.";
static const char zHelp_sha1sum_test[] = 
  "COMMAND: sha1sum\n"
  "%fossil sha1sum FILE...\n"
  "\n"
  "Compute an SHA1 checksum of all files named on the command-line.\n"
  "If an file is named \"-\" then take its content from standard input.";
static const char zHelp_status_cmd[] = 
  "Usage: %fossil status\n"
  "\n"
  "Report on the status of the current checkout.";
static const char zHelp_sync_cmd[] = 
  "Usage: %fossil sync ?URL? ?options?\n"
  "\n"
  "Synchronize the local repository with a remote repository.  This is\n"
  "the equivalent of running both \"push\" and \"pull\" at the same time.\n"
  "Use the \"-R REPO\" or \"--repository REPO\" command-line options\n"
  "to specify an alternative repository file.\n"
  "\n"
  "If a user-id and password are required, specify them as follows:\n"
  "\n"
  "    http://userid:password@www.domain.com:1234/path\n"
  "\n"
  "If the URL is not specified, then the URL from the most recent successful\n"
  "clone, push, pull, remote-url, or sync command is used.\n"
  "\n"
  "The URL specified normally becomes the new \"remote-url\" used for\n"
  "subsequent push, pull, and sync operations.  However, the \"--once\"\n"
  "command-line option makes the URL a one-time-use URL that is not\n"
  "saved.\n"
  "\n"
  "See also:  clone, push, pull, remote-url";
static const char zHelp_tag_cmd[] = 
  "Usage: %fossil tag SUBCOMMAND ...\n"
  "\n"
  "Run various subcommands to control tags and properties\n"
  "\n"
  "    %fossil tag add ?--raw? ?--propagate? TAGNAME CHECK-IN ?VALUE?\n"
  "\n"
  "        Add a new tag or property to CHECK-IN. The tag will\n"
  "        be usable instead of a CHECK-IN in commands such as\n"
  "        update and merge.  If the --propagate flag is present,\n"
  "        the tag value propages to all descendants of CHECK-IN\n"
  "\n"
  "    %fossil tag cancel ?--raw? TAGNAME CHECK-IN\n"
  "\n"
  "        Remove the tag TAGNAME from CHECK-IN, and also remove\n"
  "        the propagation of the tag to any descendants.\n"
  "\n"
  "    %fossil tag find ?--raw? TAGNAME\n"
  "\n"
  "        List all check-ins that use TAGNAME\n"
  "\n"
  "    %fossil tag list ?--raw? ?CHECK-IN?\n"
  "\n"
  "        List all tags, or if CHECK-IN is supplied, list\n"
  "        all tags and their values for CHECK-IN.\n"
  "\n"
  "The option --raw allows the manipulation of all types of tags\n"
  "used for various internal purposes in fossil. It also shows\n"
  "\"cancel\" tags for the \"find\" and \"list\" subcommands. You should\n"
  "not use this option to make changes unless you are sure what\n"
  "you are doing.\n"
  "\n"
  "If you need to use a tagname that might be confused with\n"
  "a hexadecimal baseline or artifact ID, you can explicitly\n"
  "disambiguate it by prefixing it with \"tag:\". For instance:\n"
  "\n"
  "  fossil update decaf\n"
  "\n"
  "will be taken as an artifact or baseline ID and fossil will\n"
  "probably complain that no such revision was found. However\n"
  "\n"
  "  fossil update tag:decaf\n"
  "\n"
  "will assume that \"decaf\" is a tag/branch name.";
static const char zHelp_delta_3waymerge_cmd[] = 
  "COMMAND:  test-3-way-merge\n"
  "\n"
  "Combine change in going from PIVOT->VERSION1 with the change going\n"
  "from PIVOT->VERSION2 and write the combined changes into MERGED.";
static const char zHelp_test_agg_cksum_cmd[] = 
  "COMMAND: test-agg-cksum";
static const char zHelp_test_annotate_step_cmd[] = 
  "COMMAND: test-annotate-step";
static const char zHelp_cmd_test_canonical_name[] = 
  "COMMAND:  test-canonical-name\n"
  "\n"
  "Test the operation of the canonical name generator.";
static const char zHelp_test_captcha[] = 
  "COMMAND: test-captcha";
static const char zHelp_cmd_test_cmd_list[] = 
  "Usage: %fossil test-commands\n"
  "\n"
  "List all commands used for testing and debugging.";
static const char zHelp_test_comment_format[] = 
  "COMMAND: test-comment-format";
static const char zHelp_compress_cmd[] = 
  "COMMAND: test-compress";
static const char zHelp_compress2_cmd[] = 
  "COMMAND: test-compress-2";
static const char zHelp_test_content_deltify_cmd[] = 
  "COMMAND:  test-content-deltify\n"
  "\n"
  "Convert the content at RID into a delta from SRCID.";
static const char zHelp_test_content_put_cmd[] = 
  "COMMAND:  test-content-put\n"
  "\n"
  "Extract a blob from the database and write it into a file.";
static const char zHelp_test_content_rawget_cmd[] = 
  "COMMAND:  test-content-rawget\n"
  "\n"
  "Extract a blob from the database and write it into a file.  This\n"
  "version does not expand the delta.";
static const char zHelp_test_content_undelta_cmd[] = 
  "COMMAND:  test-content-undelta\n"
  "\n"
  "Make sure the content at RECORDID is not a delta";
static const char zHelp_test_cycle_compress[] = 
  "COMMAND: test-cycle-compress\n"
  "\n"
  "Compress and uncompress each file named on the command line.\n"
  "Verify that the original content is recovered.";
static const char zHelp_test_decode64_cmd[] = 
  "Usage: %fossil test-decode64 STRING";
static const char zHelp_cmd_test_delta[] = 
  "COMMAND:  test-delta\n"
  "\n"
  "Read two files named on the command-line.  Create and apply deltas\n"
  "going in both directions.  Verify that the original files are\n"
  "correctly recovered.";
static const char zHelp_delta_apply_cmd[] = 
  "COMMAND:  test-delta-apply\n"
  "\n"
  "Given an input files and a delta, apply the delta to the input file\n"
  "and write the result.";
static const char zHelp_delta_create_cmd[] = 
  "COMMAND:  test-delta-create\n"
  "\n"
  "Given two input files, create and output a delta that carries\n"
  "the first file into the second.";
static const char zHelp_test_detach_cmd[] = 
  "COMMAND:  test-detach\n"
  "\n"
  "Change the project-code and make other changes in order to prevent\n"
  "the repository from ever again pushing or pulling to other\n"
  "repositories.  Used to create a \"test\" repository for development\n"
  "testing by cloning a working project repository.";
static const char zHelp_test_encode64_cmd[] = 
  "Usage: %fossil test-encode64 STRING";
static const char zHelp_filezip_cmd[] = 
  "COMMAND: test-filezip\n"
  "\n"
  "Generate a ZIP archive specified by the first argument that\n"
  "contains files given in the second and subsequent arguments.";
static const char zHelp_test_find_pivot[] = 
  "COMMAND:  test-find-pivot\n"
  "\n"
  "Test the pivot_find() procedure.";
static const char zHelp_user_hash_passwords_cmd[] = 
  "Usage: %fossil test-hash-passwords REPOSITORY\n"
  "\n"
  "Convert all local password storage to use a SHA1 hash of the password\n"
  "rather than cleartext.  Passwords that are already stored as the SHA1\n"
  "has are unchanged.";
static const char zHelp_cmd_test_http[] = 
  "COMMAND: test-http\n"
  "Works like the http command but gives setup permission to all users.";
static const char zHelp_import_manifest_cmd[] = 
  "Usage: %fossil test-import-manifest DATE COMMENT ?-p PARENT_RECORDID?... ?-f (FILE_RECORDID PATH)?...\n"
  "\n"
  "Create a new version containing the specified file\n"
  "revisions (if any), and child of the given PARENT version.";
static const char zHelp_isspace_cmd[] = 
  "COMMAND: test-isspace";
static const char zHelp_md5sum_test[] = 
  "COMMAND: test-md5sum\n"
  "\n"
  "Compute an MD5 checksum of all files named on the command-line.\n"
  "If an file is named \"-\" then take its content from standard input.";
static const char zHelp_test_name_to_id[] = 
  "COMMAND:  test-name-to-id\n"
  "\n"
  "Convert a name to a full artifact ID.";
static const char zHelp_manifest_test_parse_cmd[] = 
  "Usage: %fossil test-parse-manifest FILENAME\n"
  "\n"
  "Parse the manifest and discarded.  Use for testing only.";
static const char zHelp_test_rawdiff_cmd[] = 
  "COMMAND: test-rawdiff";
static const char zHelp_cmd_test_relative_name[] = 
  "COMMAND:  test-relative-name\n"
  "\n"
  "Test the operation of the relative name generator.";
static const char zHelp_testtag_cmd[] = 
  "COMMAND: test-tag\n"
  "%fossil test-tag (+|*|-)TAGNAME ARTIFACT-ID ?VALUE?\n"
  "\n"
  "Add a tag or anti-tag to the rebuildable tables of the local repository.\n"
  "No tag artifact is created so the new tag is erased the next\n"
  "time the repository is rebuilt.  This routine is for testing\n"
  "use only.";
static const char zHelp_test_th_render[] = 
  "COMMAND: test-th-render";
static const char zHelp_cmd_test_tree_name[] = 
  "COMMAND:  test-tree-name\n"
  "\n"
  "Test the operation of the tree name generator.";
static const char zHelp_test_udiff_cmd[] = 
  "COMMAND: test-udiff";
static const char zHelp_uncompress_cmd[] = 
  "COMMAND: test-uncompress";
static const char zHelp_cmd_test_urlparser[] = 
  "COMMAND: test-urlparser";
static const char zHelp_verify_all_cmd[] = 
  "COMMAND: test-verify-all\n"
  "\n"
  "Verify all records in the repository.";
static const char zHelp_test_wiki_render[] = 
  "COMMAND: test-wiki-render";
static const char zHelp_cmd_test_xfer[] = 
  "COMMAND: test-xfer\n"
  "\n"
  "This command is used for debugging the server.  There is a single\n"
  "argument which is the uncompressed content of an \"xfer\" message\n"
  "from client to server.  This command interprets that message as\n"
  "if had been received by the server.\n"
  "\n"
  "On the client side, run:\n"
  "\n"
  "     fossil push http://bogus/ --httptrace\n"
  "\n"
  "Or a similar command to provide the output.  The content of the\n"
  "message will appear on standard output.  Capture this message\n"
  "into a file named (for example) out.txt.  Then run the\n"
  "server in gdb:\n"
  "\n"
  "    gdb fossil\n"
  "    r test-xfer out.txt";
static const char zHelp_timeline_cmd[] = 
  "Usage: %fossil timeline ?WHEN? ?BASELINE|DATETIME? ?-n N? ?-t TYPE?\n"
  "\n"
  "Print a summary of activity going backwards in date and time\n"
  "specified or from the current date and time if no arguments\n"
  "are given.  Show as many as N (default 20) check-ins.  The\n"
  "WHEN argument can be any unique abbreviation of one of these\n"
  "keywords:\n"
  "\n"
  "    before\n"
  "    after\n"
  "    descendants | children\n"
  "    ancestors | parents\n"
  "\n"
  "The BASELINE can be any unique prefix of 4 characters or more.\n"
  "The DATETIME should be in the ISO8601 format.  For\n"
  "examples: \"2007-08-18 07:21:21\".  You can also say \"current\"\n"
  "for the current version or \"now\" for the current time.\n"
  "\n"
  "The optional TYPE argument may any types supported by the /timeline\n"
  "page. For example:\n"
  "\n"
  "    w  = wiki commits only\n"
  "    ci = file commits only\n"
  "    t  = tickets only";
static const char zHelp_undo_cmd[] = 
  "Usage: %fossil undo ?FILENAME...?\n"
  "\n"
  "Undo the most recent update or merge or revert operation.  If FILENAME is\n"
  "specified then restore the content of the named file(s) but otherwise\n"
  "leave the update or merge or revert in effect.\n"
  "\n"
  "A single level of undo/redo is supported.  The undo/redo stack\n"
  "is cleared by the commit and checkout commands.";
static const char zHelp_update_cmd[] = 
  "Usage: %fossil update ?VERSION? ?FILES...?\n"
  "\n"
  "Change the version of the current checkout to VERSION.  Any uncommitted\n"
  "changes are retained and applied to the new checkout.\n"
  "\n"
  "The VERSION argument can be a specific version or tag or branch name.\n"
  "If the VERSION argument is omitted, then the leaf of the the subtree\n"
  "that begins at the current version is used, if there is only a single\n"
  "leaf.  VERSION can also be \"current\" to select the leaf of the current\n"
  "version or \"latest\" to select the most recent check-in.\n"
  "\n"
  "If one or more FILES are listed after the VERSION then only the\n"
  "named files are candidates to be updated.  If FILES is omitted, all\n"
  "files in the current checkout are subject to be updated.\n"
  "\n"
  "The -n or --nochange option causes this command to do a \"dry run\".  It\n"
  "prints out what would have happened but does not actually make any\n"
  "changes to the current checkout or the repository.\n"
  "\n"
  "The -v or --verbose option prints status information about unchanged\n"
  "files in addition to those file that actually do change.";
static const char zHelp_user_cmd[] = 
  "Usage: %fossil user SUBCOMMAND ...  ?-R|--repository FILE?\n"
  "\n"
  "Run various subcommands on users of the open repository or of\n"
  "the repository identified by the -R or --repository option.\n"
  "\n"
  "   %fossil user capabilities USERNAME ?STRING?\n"
  "\n"
  "       Query or set the capabilities for user USERNAME\n"
  "\n"
  "   %fossil user default ?USERNAME?\n"
  "\n"
  "       Query or set the default user.  The default user is the\n"
  "       user for command-line interaction.\n"
  "\n"
  "   %fossil user list\n"
  "\n"
  "       List all users known to the repository\n"
  "\n"
  "   %fossil user new ?USERNAME? ?CONTACT-INFO? ?PASSWORD?\n"
  "\n"
  "       Create a new user in the repository.  Users can never be\n"
  "       deleted.  They can be denied all access but they must continue\n"
  "       to exist in the database.\n"
  "\n"
  "   %fossil user password USERNAME ?PASSWORD?\n"
  "\n"
  "       Change the web access password for a user.";
static const char zHelp_version_cmd[] = 
  "Usage: %fossil version\n"
  "\n"
  "Print the source code version number for the fossil executable.";
static const char zHelp_wiki_cmd[] = 
  "Usage: %fossil wiki (export|create|commit|list) WikiName\n"
  "\n"
  "Run various subcommands to work with wiki entries.\n"
  "\n"
  "    %fossil wiki export PAGENAME ?FILE?\n"
  "\n"
  "       Sends the latest version of the PAGENAME wiki\n"
  "       entry to the given file or standard output.\n"
  "\n"
  "    %fossil wiki commit PAGENAME ?FILE?\n"
  "\n"
  "       Commit changes to a wiki page from FILE or from standard\n"
  "       input.\n"
  "\n"
  "    %fossil wiki create PAGENAME ?FILE?\n"
  "\n"
  "       Create a new wiki page with initial content taken from\n"
  "       FILE or from standard input.\n"
  "\n"
  "    %fossil wiki list\n"
  "\n"
  "       Lists all wiki entries, one per line, ordered\n"
  "       case-insentively by name.\n"
  "\n"
  "TODOs:\n"
  "\n"
  "    %fossil wiki export ?-u ARTIFACT? WikiName ?FILE?\n"
  "\n"
  "       Outputs the selected version of WikiName.\n"
  "\n"
  "    %fossil wiki delete ?-m MESSAGE? WikiName\n"
  "\n"
  "       The same as deleting a file entry, but i don't know if fossil\n"
  "       supports a commit message for Wiki entries.\n"
  "\n"
  "    %fossil wiki ?-u? ?-d? ?-s=[|]? list\n"
  "\n"
  "       Lists the artifact ID and/or Date of last change along with\n"
  "       each entry name, delimited by the -s char.\n"
  "\n"
  "    %fossil wiki diff ?ARTIFACT? ?-f infile[=stdin]? EntryName\n"
  "\n"
  "       Diffs the local copy of a page with a given version (defaulting\n"
  "       to the head version).";
static const char zHelp_baseline_zip_cmd[] = 
  "Usage: %fossil zip VERSION OUTPUTFILE [--name DIRECTORYNAME]\n"
  "\n"
  "Generate a ZIP archive for a specified version.  If the --name option is\n"
  "used, it argument becomes the name of the top-level directory in the\n"
  "resulting ZIP archive.  If --name is omitted, the top-level directory\n"
  "named is derived from the project name, the check-in date and time, and\n"
  "the artifact ID of the check-in.";
static const char * const aCmdHelp[] = {
  zHelp_add_cmd,
  zHelp_all_cmd,
  zHelp_annotate_cmd,
  zHelp_artifact_cmd,
  zHelp_branch_cmd,
  zHelp_cmd_cgi,
  zHelp_changes_cmd,
  zHelp_checkout_cmd,
  zHelp_commit_cmd,
  zHelp_clean_cmd,
  zHelp_clone_cmd,
  zHelp_close_cmd,
  zHelp_checkout_cmd,
  zHelp_commit_cmd,
  zHelp_configuration_cmd,
  zHelp_deconstruct_cmd,
  zHelp_del_cmd,
  zHelp_descendants_cmd,
  zHelp_diff_cmd,
  zHelp_extra_cmd,
  zHelp_finfo_cmd,
  zHelp_diff_cmd,
  zHelp_help_cmd,
  zHelp_cmd_http,
  zHelp_info_cmd,
  zHelp_leaves_cmd,
  zHelp_ls_cmd,
  zHelp_merge_cmd,
  zHelp_mv_cmd,
  zHelp_create_repository_cmd,
  zHelp_cmd_open,
  zHelp_pull_cmd,
  zHelp_push_cmd,
  zHelp_rebuild_database,
  zHelp_reconstruct_cmd,
  zHelp_redo_cmd,
  zHelp_remote_url_cmd,
  zHelp_mv_cmd,
  zHelp_revert_cmd,
  zHelp_del_cmd,
  zHelp_rstats_cmd,
  zHelp_scrub_cmd,
  zHelp_search_cmd,
  zHelp_cmd_webserver,
  zHelp_setting_cmd,
  zHelp_sha1sum_test,
  zHelp_status_cmd,
  zHelp_sync_cmd,
  zHelp_tag_cmd,
  zHelp_delta_3waymerge_cmd,
  zHelp_test_agg_cksum_cmd,
  zHelp_test_annotate_step_cmd,
  zHelp_cmd_test_canonical_name,
  zHelp_test_captcha,
  zHelp_cmd_test_cmd_list,
  zHelp_test_comment_format,
  zHelp_compress_cmd,
  zHelp_compress2_cmd,
  zHelp_test_content_deltify_cmd,
  zHelp_test_content_put_cmd,
  zHelp_test_content_rawget_cmd,
  zHelp_test_content_undelta_cmd,
  zHelp_test_cycle_compress,
  zHelp_test_decode64_cmd,
  zHelp_cmd_test_delta,
  zHelp_delta_apply_cmd,
  zHelp_delta_create_cmd,
  zHelp_test_detach_cmd,
  zHelp_test_encode64_cmd,
  zHelp_filezip_cmd,
  zHelp_test_find_pivot,
  zHelp_user_hash_passwords_cmd,
  zHelp_cmd_test_http,
  zHelp_import_manifest_cmd,
  zHelp_isspace_cmd,
  zHelp_md5sum_test,
  zHelp_test_name_to_id,
  zHelp_manifest_test_parse_cmd,
  zHelp_test_rawdiff_cmd,
  zHelp_cmd_test_relative_name,
  zHelp_testtag_cmd,
  zHelp_test_th_render,
  zHelp_cmd_test_tree_name,
  zHelp_test_udiff_cmd,
  zHelp_uncompress_cmd,
  zHelp_cmd_test_urlparser,
  zHelp_verify_all_cmd,
  zHelp_test_wiki_render,
  zHelp_cmd_test_xfer,
  zHelp_timeline_cmd,
  zHelp_cmd_webserver,
  zHelp_undo_cmd,
  zHelp_setting_cmd,
  zHelp_update_cmd,
  zHelp_user_cmd,
  zHelp_version_cmd,
  zHelp_wiki_cmd,
  zHelp_baseline_zip_cmd,
};
#define main fossil_main
