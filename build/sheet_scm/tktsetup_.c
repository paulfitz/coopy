/*
** Copyright (c) 2007 D. Richard Hipp
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public
** License version 2 as published by the Free Software Foundation.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** General Public License for more details.
** 
** You should have received a copy of the GNU General Public
** License along with this library; if not, write to the
** Free Software Foundation, Inc., 59 Temple Place - Suite 330,
** Boston, MA  02111-1307, USA.
**
** Author contact information:
**   drh@hwaci.com
**   http://www.hwaci.com/drh/
**
*******************************************************************************
**
** This file contains code to implement the ticket configuration
** setup screens.
*/
#include "config.h"
#include "tktsetup.h"
#include <assert.h>

/*
** Main sub-menu for configuring the ticketing system.
** WEBPAGE: tktsetup
*/
void tktsetup_page(void){
  login_check_credentials();
  if( !g.okSetup ){
    login_needed();
  }

  style_header("Ticket Setup");
  cgi_printf("<table border=\"0\" cellspacing=\"20\">\n");
  setup_menu_entry("Table", "tktsetup_tab",
    "Specify the schema of the  \"ticket\" table in the database.");
  setup_menu_entry("Timeline", "tktsetup_timeline",
    "How to display ticket status in the timeline");
  setup_menu_entry("Common", "tktsetup_com",
    "Common TH1 code run before all ticket processing.");
  setup_menu_entry("New Ticket Page", "tktsetup_newpage",
    "HTML with embedded TH1 code for the \"new ticket\" webpage.");
  setup_menu_entry("View Ticket Page", "tktsetup_viewpage",
    "HTML with embedded TH1 code for the \"view ticket\" webpage.");
  setup_menu_entry("Edit Ticket Page", "tktsetup_editpage",
    "HTML with embedded TH1 code for the \"edit ticket\" webpage.");
  setup_menu_entry("Report List Page", "tktsetup_reportlist",
    "HTML with embedded TH1 code for the \"report list\" webpage.");
  setup_menu_entry("Report Template", "tktsetup_rpttplt",
    "The default ticket report format.");
  setup_menu_entry("Key Template", "tktsetup_keytplt",
    "The default color key for reports.");
  cgi_printf("</table>\n");
  style_footer();
}

/*
** NOTE:  When changing the table definition below, also change the
** equivalent definition found in schema.c.
*/
/* @-comment: ** */
static const char zDefaultTicketTable[] =
"CREATE TABLE ticket(\n"
"  -- Do not change any column that begins with tkt_\n"
"  tkt_id INTEGER PRIMARY KEY,\n"
"  tkt_uuid TEXT UNIQUE,\n"
"  tkt_mtime DATE,\n"
"  -- Add as many field as required below this line\n"
"  type TEXT,\n"
"  status TEXT,\n"
"  subsystem TEXT,\n"
"  priority TEXT,\n"
"  severity TEXT,\n"
"  foundin TEXT,\n"
"  private_contact TEXT,\n"
"  resolution TEXT,\n"
"  title TEXT,\n"
"  comment TEXT\n"
");\n"
;

/*
** Return the ticket table definition
*/
const char *ticket_table_schema(void){
  return db_get("ticket-table", (char*)zDefaultTicketTable);
}

/*
** Common implementation for the ticket setup editor pages.
*/
static void tktsetup_generic(
  const char *zTitle,           /* Page title */
  const char *zDbField,         /* Configuration field being edited */
  const char *zDfltValue,       /* Default text value */
  const char *zDesc,            /* Description of this field */
  char *(*xText)(const char*),  /* Validitity test or NULL */
  void (*xRebuild)(void),       /* Run after successulf update */
  int height                    /* Height of the edit box */
){
  const char *z;
  int isSubmit;
  
  login_check_credentials();
  if( !g.okSetup ){
    login_needed();
  }
  if( P("setup") ){
    cgi_redirect("tktsetup");
  }
  isSubmit = P("submit")!=0;
  z = P("x");
  if( z==0 ){
    z = db_get(zDbField, (char*)zDfltValue);
  }
  style_header("Edit %s", zTitle);
  if( P("clear")!=0 ){
    login_verify_csrf_secret();
    db_unset(zDbField, 0);
    if( xRebuild ) xRebuild();
    z = zDfltValue;
  }else if( isSubmit ){
    char *zErr = 0;
    login_verify_csrf_secret();
    if( xText && (zErr = xText(z))!=0 ){
      cgi_printf("<p><font color=\"red\"><b>ERROR: %h</b></font></p>\n",zErr);
    }else{
      db_set(zDbField, z, 0);
      if( xRebuild ) xRebuild();
      cgi_redirect("tktsetup");
    }
  }
  cgi_printf("<form action=\"%s/%s\" method=\"POST\">\n",g.zBaseURL,g.zPath);
  login_insert_csrf_secret();
  cgi_printf("<p>%s</p>\n"
         "<textarea name=\"x\" rows=\"%d\" cols=\"80\">%h</textarea>\n"
         "<blockquote>\n"
         "<input type=\"submit\" name=\"submit\" value=\"Apply Changes\">\n"
         "<input type=\"submit\" name=\"clear\" value=\"Revert To Default\">\n"
         "<input type=\"submit\" name=\"setup\" value=\"Cancel\">\n"
         "</blockquote>\n"
         "</form>\n"
         "<hr>\n"
         "<h2>Default %s</h2>\n"
         "<blockquote><pre>\n"
         "%h\n"
         "</pre></blockquote>\n",zDesc,height,z,zTitle,zDfltValue);
  style_footer();
}

/*
** WEBPAGE: tktsetup_tab
*/
void tktsetup_tab_page(void){
  static const char zDesc[] =
  "<p>Enter a valid CREATE TABLE statement for the \"ticket\" table.  The\n"
  "table must contain columns named \"tkt_id\", \"tkt_uuid\", and \"tkt_mtime\"\n"
  "with an unique index on \"tkt_uuid\" and \"tkt_mtime\".</p>\n"
  ;
  tktsetup_generic(
    "Ticket Table Schema",
    "ticket-table",
    zDefaultTicketTable,
    zDesc,
    ticket_schema_check,
    ticket_rebuild,
    20
  );
}

static const char zDefaultTicketCommon[] =
"set type_choices {\n"
"   Code_Defect\n"
"   Build_Problem\n"
"   Documentation\n"
"   Feature_Request\n"
"   Incident\n"
"}\n"
"set priority_choices {\n"
"  Immediate\n"
"  High\n"
"  Medium\n"
"  Low\n"
"  Zero\n"
"}\n"
"set severity_choices {\n"
"  Critical\n"
"  Severe\n"
"  Important\n"
"  Minor\n"
"  Cosmetic\n"
"}\n"
"set resolution_choices {\n"
"  Open\n"
"  Fixed\n"
"  Rejected\n"
"  Workaround\n"
"  Unable_To_Reproduce\n"
"  Works_As_Designed\n"
"  External_Bug\n"
"  Not_A_Bug\n"
"  Duplicate\n"
"  Overcome_By_Events\n"
"  Drive_By_Patch\n"
"  Misconfiguration\n"
"}\n"
"set status_choices {\n"
"  Open\n"
"  Verified\n"
"  Review\n"
"  Deferred\n"
"  Fixed\n"
"  Tested\n"
"  Closed\n"
"}\n"
"set subsystem_choices {\n"
"}\n"
;

/*
** Return the ticket common code.
*/
const char *ticket_common_code(void){
  return db_get("ticket-common", (char*)zDefaultTicketCommon);
}

/*
** WEBPAGE: tktsetup_com
*/
void tktsetup_com_page(void){
  static const char zDesc[] =
  "<p>Enter TH1 script that initializes variables prior to generating\n"
  "any of the ticket view, edit, or creation pages.</p>\n"
  ;
  tktsetup_generic(
    "Ticket Common Script",
    "ticket-common",
    zDefaultTicketCommon,
    zDesc,
    0,
    0,
    30
  );
}

static const char zDefaultNew[] =
"<th1>\n"
"  if {[info exists submit]} {\n"
"     set status Open\n"
"     submit_ticket\n"
"  }\n"
"</th1>\n"
"<h1 align=\"center\">Enter A New Ticket</h1>\n"
"<table cellpadding=\"5\">\n"
"<tr>\n"
"<td colspan=\"2\">\n"
"Enter a one-line summary of the ticket:<br>\n"
"<input type=\"text\" name=\"title\" size=\"60\" value=\"$<title>\">\n"
"</td>\n"
"</tr>\n"
"\n"
"<tr>\n"
"<td align=\"right\">Type:\n"
"<th1>combobox type $type_choices 1</th1>\n"
"</td>\n"
"<td>What type of ticket is this?</td>\n"
"</tr>\n"
"\n"
"<tr>\n"
"<td align=\"right\">Version:\n"
"<input type=\"text\" name=\"foundin\" size=\"20\" value=\"$<foundin>\">\n"
"</td>\n"
"<td>In what version or build number do you observe the problem?</td>\n"
"</tr>\n"
"\n"
"<tr>\n"
"<td align=\"right\">Severity:\n"
"<th1>combobox severity $severity_choices 1</th1>\n"
"</td>\n"
"<td>How debilitating is the problem?  How badly does the problem\n"
"affect the operation of the product?</td>\n"
"</tr>\n"
"\n"
"<tr>\n"
"<td align=\"right\">EMail:\n"
"<input type=\"text\" name=\"private_contact\" value=\"$<private_contact>\" size=\"30\">\n"
"</td>\n"
"<td><u>Not publicly visible</u>. Used by developers to contact you with\n"
"questions.</td>\n"
"</tr>\n"
"\n"
"<tr>\n"
"<td colspan=\"2\">\n"
"Enter a detailed description of the problem.\n"
"For code defects, be sure to provide details on exactly how\n"
"the problem can be reproduced.  Provide as much detail as\n"
"possible.\n"
"<br>\n"
"<th1>set nline [linecount $comment 50 10]</th1>\n"
"<textarea name=\"comment\" cols=\"80\" rows=\"$nline\"\n"
" wrap=\"virtual\" class=\"wikiedit\">$<comment></textarea><br>\n"
"<input type=\"submit\" name=\"preview\" value=\"Preview\">\n"
"</tr>\n"
"\n"
"<th1>enable_output [info exists preview]</th1>\n"
"<tr><td colspan=\"2\">\n"
"Description Preview:<br><hr>\n"
"<th1>wiki $comment</th1>\n"
"<hr>\n"
"</td></tr>\n"
"<th1>enable_output 1</th1>\n"
"\n"
"<tr>\n"
"<td align=\"right\">\n"
"<input type=\"submit\" name=\"submit\" value=\"Submit\">\n"
"</td>\n"
"<td>After filling in the information above, press this button to create\n"
"the new ticket</td>\n"
"</tr>\n"
"<tr>\n"
"<td align=\"right\">\n"
"<input type=\"submit\" name=\"cancel\" value=\"Cancel\">\n"
"</td>\n"
"<td>Abandon and forget this ticket</td>\n"
"</tr>\n"
"</table>\n"
;

/*
** Return the code used to generate the new ticket page
*/
const char *ticket_newpage_code(void){
  return db_get("ticket-newpage", (char*)zDefaultNew);
}

/*
** WEBPAGE: tktsetup_newpage
*/
void tktsetup_newpage_page(void){
  static const char zDesc[] =
  "<p>Enter HTML with embedded TH1 script that will render the \"new ticket\"\n"
  "page</p>\n"
  ;
  tktsetup_generic(
    "HTML For New Tickets",
    "ticket-newpage",
    zDefaultNew,
    zDesc,
    0,
    0,
    40
  );
}

static const char zDefaultView[] =
"<table cellpadding=\"5\">\n"
"<tr><td align=\"right\">Ticket&nbsp;UUID:</td><td bgcolor=\"#d0d0d0\" colspan=\"3\">\n"
"$<tkt_uuid>\n"
"</td></tr>\n"
"<tr><td align=\"right\">Title:</td>\n"
"<td bgcolor=\"#d0d0d0\" colspan=\"3\" valign=\"top\">\n"
"<th1>wiki $title</th1>\n"
"</td></tr>\n"
"<tr><td align=\"right\">Status:</td><td bgcolor=\"#d0d0d0\">\n"
"$<status>\n"
"</td>\n"
"<td align=\"right\">Type:</td><td bgcolor=\"#d0d0d0\">\n"
"$<type>\n"
"</td></tr>\n"
"<tr><td align=\"right\">Severity:</td><td bgcolor=\"#d0d0d0\">\n"
"$<severity>\n"
"</td>\n"
"<td align=\"right\">Priority:</td><td bgcolor=\"#d0d0d0\">\n"
"$<priority>\n"
"</td></tr>\n"
"<tr><td align=\"right\">Subsystem:</td><td bgcolor=\"#d0d0d0\">\n"
"$<subsystem>\n"
"</td>\n"
"<td align=\"right\">Resolution:</td><td bgcolor=\"#d0d0d0\">\n"
"$<resolution>\n"
"</td></tr>\n"
"<tr><td align=\"right\">Last&nbsp;Modified:</td><td bgcolor=\"#d0d0d0\">\n"
"$<tkt_datetime>\n"
"</td>\n"
"<th1>enable_output [hascap e]</th1>\n"
"  <td align=\"right\">Contact:</td><td bgcolor=\"#d0d0d0\">\n"
"  $<private_contact>\n"
"  </td>\n"
"<th1>enable_output 1</th1>\n"
"</tr>\n"
"<tr><td align=\"right\">Version&nbsp;Found&nbsp;In:</td>\n"
"<td colspan=\"3\" valign=\"top\" bgcolor=\"#d0d0d0\">\n"
"$<foundin>\n"
"</td></tr>\n"
"<tr><td>Description &amp; Comments:</td></tr>\n"
"<tr><td colspan=\"4\" bgcolor=\"#d0d0d0\">\n"
"<span  bgcolor=\"#d0d0d0\"><th1>wiki $comment</th1></span>\n"
"</td></tr>\n"
"</table>\n"
;


/*
** Return the code used to generate the view ticket page
*/
const char *ticket_viewpage_code(void){
  return db_get("ticket-viewpage", (char*)zDefaultView);
}

/*
** WEBPAGE: tktsetup_viewpage
*/
void tktsetup_viewpage_page(void){
  static const char zDesc[] =
  "<p>Enter HTML with embedded TH1 script that will render the \"view ticket\"\n"
  "page</p>\n"
  ;
  tktsetup_generic(
    "HTML For Viewing Tickets",
    "ticket-viewpage",
    zDefaultView,
    zDesc,
    0,
    0,
    40
  );
}

static const char zDefaultEdit[] =
"<th1>\n"
"  if {![info exists username]} {set username $login}\n"
"  if {[info exists submit]} {\n"
"    if {[info exists cmappnd]} {\n"
"      if {[string length $cmappnd]>0} {\n"
"        set ctxt \"\\n\\n<hr><i>[htmlize $login]\"\n"
"        if {$username ne $login} {\n"
"          set ctxt \"$ctxt claiming to be [htmlize $username]\"\n"
"        }\n"
"        set ctxt \"$ctxt added on [date]:</i><br>\\n$cmappnd\"\n"
"        append_field comment $ctxt\n"
"      }\n"
"    }\n"
"    submit_ticket\n"
"  }\n"
"</th1>\n"
"<table cellpadding=\"5\">\n"
"<tr><td align=\"right\">Title:</td><td>\n"
"<input type=\"text\" name=\"title\" value=\"$<title>\" size=\"60\">\n"
"</td></tr>\n"
"<tr><td align=\"right\">Status:</td><td>\n"
"<th1>combobox status $status_choices 1</th1>\n"
"</td></tr>\n"
"<tr><td align=\"right\">Type:</td><td>\n"
"<th1>combobox type $type_choices 1</th1>\n"
"</td></tr>\n"
"<tr><td align=\"right\">Severity:</td><td>\n"
"<th1>combobox severity $severity_choices 1</th1>\n"
"</td></tr>\n"
"<tr><td align=\"right\">Priority:</td><td>\n"
"<th1>combobox priority $priority_choices 1</th1>\n"
"</td></tr>\n"
"<tr><td align=\"right\">Resolution:</td><td>\n"
"<th1>combobox resolution $resolution_choices 1</th1>\n"
"</td></tr>\n"
"<tr><td align=\"right\">Subsystem:</td><td>\n"
"<th1>combobox subsystem $subsystem_choices 1</th1>\n"
"</td></tr>\n"
"<th1>enable_output [hascap e]</th1>\n"
"  <tr><td align=\"right\">Contact:</td><td>\n"
"  <input type=\"text\" name=\"private_contact\" size=\"40\"\n"
"   value=\"$<private_contact>\">\n"
"  </td></tr>\n"
"<th1>enable_output 1</th1>\n"
"<tr><td align=\"right\">Version&nbsp;Found&nbsp;In:</td><td>\n"
"<input type=\"text\" name=\"foundin\" size=\"50\" value=\"$<foundin>\">\n"
"</td></tr>\n"
"<tr><td colspan=\"2\">\n"
"<th1>\n"
"  if {![info exists eall]} {set eall 0}\n"
"  if {[info exists aonlybtn]} {set eall 0}\n"
"  if {[info exists eallbtn]} {set eall 1}\n"
"  if {![hascap w]} {set eall 0}\n"
"  if {![info exists cmappnd]} {set cmappnd {}}\n"
"  set nline [linecount $comment 15 10]\n"
"  enable_output $eall\n"
"</th1>\n"
"  Description And Comments:<br>\n"
"  <textarea name=\"comment\" cols=\"80\" rows=\"$nline\"\n"
"   wrap=\"virtual\" class=\"wikiedit\">$<comment></textarea><br>\n"
"  <input type=\"hidden\" name=\"eall\" value=\"1\">\n"
"  <input type=\"submit\" name=\"aonlybtn\" value=\"Append Remark\">\n"
"  <input type=\"submit\" name=\"preview1btn\" value=\"Preview\">\n"
"<th1>enable_output [expr {!$eall}]</th1>\n"
"  Append Remark from\n"
"  <input type=\"text\" name=\"username\" value=\"$<username>\" size=\"30\">:<br>\n"
"  <textarea name=\"cmappnd\" cols=\"80\" rows=\"15\"\n"
"   wrap=\"virtual\" class=\"wikiedit\">$<cmappnd></textarea><br>\n"
"<th1>enable_output [expr {[hascap w] && !$eall}]</th1>\n"
"  <input type=\"submit\" name=\"eallbtn\" value=\"Edit All\">\n"
"<th1>enable_output [expr {!$eall}]</th1>\n"
"  <input type=\"submit\" name=\"preview2btn\" value=\"Preview\">\n"
"<th1>enable_output 1</th1>\n"
"</td></tr>\n"
"\n"
"<th1>enable_output [info exists preview1btn]</th1>\n"
"<tr><td colspan=\"2\">\n"
"Description Preview:<br><hr>\n"
"<th1>wiki $comment</th1>\n"
"<hr>\n"
"</td></tr>\n"
"<th1>enable_output [info exists preview2btn]</th1>\n"
"<tr><td colspan=\"2\">\n"
"Description Preview:<br><hr>\n"
"<th1>wiki $cmappnd</th1>\n"
"<hr>\n"
"</td></tr>\n"
"<th1>enable_output 1</th1>\n"
"\n"
"<tr><td align=\"right\"></td><td>\n"
"<input type=\"submit\" name=\"submit\" value=\"Submit Changes\">\n"
"<input type=\"submit\" name=\"cancel\" value=\"Cancel\">\n"
"</td></tr>\n"
"</table>\n"
;

/*
** Return the code used to generate the edit ticket page
*/
const char *ticket_editpage_code(void){
  return db_get("ticket-editpage", (char*)zDefaultEdit);
}

/*
** WEBPAGE: tktsetup_editpage
*/
void tktsetup_editpage_page(void){
  static const char zDesc[] =
  "<p>Enter HTML with embedded TH1 script that will render the \"edit ticket\"\n"
  "page</p>\n"
  ;
  tktsetup_generic(
    "HTML For Editing Tickets",
    "ticket-editpage",
    zDefaultEdit,
    zDesc,
    0,
    0,
    40
  );
}

/*
** The default report list page
*/
static const char zDefaultReportList[] =
"<th1>\n"
"if {[hascap n]} {\n"
"  html \"<p>Enter a new ticket:</p>\"\n"
"  html \"<ul><li><a href='tktnew'>New ticket</a></li></ul>\"\n"
"}\n"
"</th1>\n"
"\n"
"<p>Choose a report format from the following list:</p>\n"
"<ol>\n"
"<th1>html $report_items</th1>\n"
"</ol>\n"
"\n"
"<th1>\n"
"if {[hascap t]} {\n"
"  html \"<p>Create a new ticket display format:</p>\"\n"
"  html \"<ul><li><a href='rptnew'>New report format</a></li></ul>\"\n"
"}\n"
"</th1>\n"
;

/*
** Return the code used to generate the report list
*/
const char *ticket_reportlist_code(void){
  return db_get("ticket-reportlist", (char*)zDefaultReportList);
}

/*
** WEBPAGE: tktsetup_reportlist
*/
void tktsetup_reportlist(void){
  static const char zDesc[] =
  "<p>Enter HTML with embedded TH1 script that will render the \"report list\"\n"
  "page</p>\n"
  ;
  tktsetup_generic(
    "HTML For Report List",
    "ticket-reportlist",
    zDefaultReportList,
    zDesc,
    0,
    0,
    40
  );
}

/*
** The default template ticket report format:
*/
static char zDefaultReport[] = 
"SELECT\n"
"  CASE WHEN status IN ('Open','Verified') THEN '#f2dcdc'\n"
"       WHEN status='Review' THEN '#e8e8e8'\n"
"       WHEN status='Fixed' THEN '#cfe8bd'\n"
"       WHEN status='Tested' THEN '#bde5d6'\n"
"       WHEN status='Deferred' THEN '#cacae5'\n"
"       ELSE '#c8c8c8' END AS 'bgcolor',\n"
"  substr(tkt_uuid,1,10) AS '#',\n"
"  datetime(tkt_mtime) AS 'mtime',\n"
"  type,\n"
"  status,\n"
"  subsystem,\n"
"  title,\n"
"  comment AS '_comments'\n"
"FROM ticket\n"
;


/*
** Return the template ticket report format:
*/
char *ticket_report_template(void){
  return db_get("ticket-report-template", zDefaultReport);
}

/*
** WEBPAGE: tktsetup_rpttplt
*/
void tktsetup_rpttplt_page(void){
  static const char zDesc[] =
  "<p>Enter the default ticket report format template.  This is the\n"
  "the template report format that initially appears when creating a\n"
  "new ticket summary report.</p>\n"
  ;
  tktsetup_generic(
    "Default Report Template",
    "ticket-report-template",
    zDefaultReport,
    zDesc,
    0,
    0,
    20
  );
}

/*
** The default template ticket key:
*/
static const char zDefaultKey[] = 
"#ffffff Key:\n"
"#f2dcdc Active\n"
"#e8e8e8 Review\n"
"#cfe8bd Fixed\n"
"#bde5d6 Tested\n"
"#cacae5 Deferred\n"
"#c8c8c8 Closed\n"
;


/*
** Return the template ticket report format:
*/
const char *ticket_key_template(void){
  return db_get("ticket-key-template", (char*)zDefaultKey);
}

/*
** WEBPAGE: tktsetup_keytplt
*/
void tktsetup_keytplt_page(void){
  static const char zDesc[] =
  "<p>Enter the default ticket report color-key template.  This is the\n"
  "the color-key that initially appears when creating a\n"
  "new ticket summary report.</p>\n"
  ;
  tktsetup_generic(
    "Default Report Color-Key Template",
    "ticket-key-template",
    zDefaultKey,
    zDesc,
    0,
    0,
    10
  );
}

/*
** WEBPAGE: tktsetup_timeline
*/
void tktsetup_timeline_page(void){
  login_check_credentials();
  if( !g.okSetup ){
    login_needed();
  }

  if( P("setup") ){
    cgi_redirect("tktsetup");
  }
  style_header("Ticket Display On Timelines");
  db_begin_transaction();
  cgi_printf("<form action=\"%s/tktsetup_timeline\" method=\"POST\">\n",g.zBaseURL);
  login_insert_csrf_secret();

  cgi_printf("<hr>\n");
  entry_attribute("Ticket Title", 40, "ticket-title-expr", "t", "title");
  cgi_printf("<p>An SQL expression in a query against the TICKET table that will\n"
         "return the title of the ticket for display purposes.</p>\n");

  cgi_printf("<hr>\n");
  entry_attribute("Ticket Status", 40, "ticket-status-column", "s", "status");
  cgi_printf("<p>The name of the column in the TICKET table that contains the ticket\n"
         "status in human-readable form.  Case sensitive.</p>\n");

  cgi_printf("<hr>\n");
  entry_attribute("Ticket Closed", 40, "ticket-closed-expr", "c",
                  "status='Closed'");
  cgi_printf("<p>An SQL expression that evaluates to true in a TICKET table query if\n"
         "the ticket is closed.</p>\n");

  cgi_printf("<hr>\n"
         "<p>\n"
         "<input type=\"submit\"  name=\"submit\" value=\"Apply Changes\">\n"
         "<input type=\"submit\" name=\"setup\" value=\"Cancel\">\n"
         "</p>\n"
         "</form>\n");
  db_end_transaction(0);
  style_footer();
  
}
