/*
** Copyright (c) 2006,2007 D. Richard Hipp
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
** This file contains code to implement the basic web page look and feel.
**
*/
#include "config.h"
#include "style.h"


/*
** Elements of the submenu are collected into the following
** structure and displayed below the main menu by style_header().
**
** Populate this structure with calls to style_submenu_element()
** prior to calling style_header().
*/
static struct Submenu {
  const char *zLabel;
  const char *zTitle;
  const char *zLink;
} aSubmenu[30];
static int nSubmenu = 0;

/*
** Remember that the header has been generated.  The footer is omitted
** if an error occurs before the header.
*/
static int headerHasBeenGenerated = 0;

/*
** Add a new element to the submenu
*/
void style_submenu_element(
  const char *zLabel,
  const char *zTitle,
  const char *zLink,
  ...
){
  va_list ap;
  assert( nSubmenu < sizeof(aSubmenu)/sizeof(aSubmenu[0]) );
  aSubmenu[nSubmenu].zLabel = zLabel;
  aSubmenu[nSubmenu].zTitle = zTitle;
  va_start(ap, zLink);
  aSubmenu[nSubmenu].zLink = vmprintf(zLink, ap);
  va_end(ap);
  nSubmenu++;
}

/*
** Compare two submenu items for sorting purposes
*/
static int submenuCompare(const void *a, const void *b){
  const struct Submenu *A = (const struct Submenu*)a;
  const struct Submenu *B = (const struct Submenu*)b;
  return strcmp(A->zLabel, B->zLabel);
}

/*
** Draw the header.
*/
void style_header(const char *zTitleFormat, ...){
  va_list ap;
  char *zTitle;
  const char *zHeader = db_get("header", (char*)zDefaultHeader);  
  login_check_credentials();

  va_start(ap, zTitleFormat);
  zTitle = vmprintf(zTitleFormat, ap);
  va_end(ap);
  
  cgi_destination(CGI_HEADER);
  cgi_printf("%s",
     "<!DOCTYPE html PUBLIC \"-//W3C/DTD XHTML 1.0 Strict//EN\""
     " \"http://www.x3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">");
  
  if( g.thTrace ) Th_Trace("BEGIN_HEADER<br />\n", -1);

  /* Generate the header up through the main menu */
  Th_Store("project_name", db_get("project-name","Unnamed Fossil Project"));
  Th_Store("title", zTitle);
  Th_Store("baseurl", g.zBaseURL);
  Th_Store("index_page", db_get("index-page","/home"));
  Th_Store("current_page", g.zPath);
  Th_Store("manifest_version", MANIFEST_VERSION);
  Th_Store("manifest_date", MANIFEST_DATE);
  if( g.zLogin ){
    Th_Store("login", g.zLogin);
  }
  if( g.thTrace ) Th_Trace("BEGIN_HEADER_SCRIPT<br />\n", -1);
  Th_Render(zHeader);
  if( g.thTrace ) Th_Trace("END_HEADER<br />\n", -1);
  Th_Unstore("title");   /* Avoid collisions with ticket field names */
  cgi_destination(CGI_BODY);
  g.cgiPanic = 1;
  headerHasBeenGenerated = 1;
}

/*
** Draw the footer at the bottom of the page.
*/
void style_footer(void){
  const char *zFooter;

  if( !headerHasBeenGenerated ) return;
  
  /* Go back and put the submenu at the top of the page.  We delay the
  ** creation of the submenu until the end so that we can add elements
  ** to the submenu while generating page text.
  */
  cgi_destination(CGI_HEADER);
  if( nSubmenu>0 ){
    int i;
    cgi_printf("<div class=\"submenu\">\n");
    qsort(aSubmenu, nSubmenu, sizeof(aSubmenu[0]), submenuCompare);
    for(i=0; i<nSubmenu; i++){
      struct Submenu *p = &aSubmenu[i];
      if( p->zLink==0 ){
        cgi_printf("<span class=\"label\">%h</span>\n",p->zLabel);
      }else{
        cgi_printf("<a class=\"label\" href=\"%s\">%h</a>\n",p->zLink,p->zLabel);
      }
    }
    cgi_printf("</div>\n");
  }
  cgi_printf("<div class=\"content\">\n");
  cgi_destination(CGI_BODY);

  /* Put the footer at the bottom of the page.
  */
  cgi_printf("</div><br clear=\"both\"/>\n");
  zFooter = db_get("footer", (char*)zDefaultFooter);
  if( g.thTrace ) Th_Trace("BEGIN_FOOTER<br />\n", -1);
  Th_Render(zFooter);
  if( g.thTrace ) Th_Trace("END_FOOTER<br />\n", -1);
  
  /* Render trace log if TH1 tracing is enabled. */
  if( g.thTrace ){
    cgi_append_content("<font color=\"red\"><hr>\n", -1);
    cgi_append_content(blob_str(&g.thLog), blob_size(&g.thLog));
    cgi_append_content("</font>\n", -1);
  }
}

/*
** Begin a side-box on the right-hand side of a page.  The title and
** the width of the box are given as arguments.  The width is usually
** a percentage of total screen width.
*/
void style_sidebox_begin(const char *zTitle, const char *zWidth){
  cgi_printf("<table width=\"%s\" align=\"right\" border=\"1\" cellpadding=5\n"
         " vspace=5 hspace=5>\n"
         "<tr><td>\n"
         "<b>%h</b>\n",zWidth,zTitle);
}

/* End the side-box
*/
void style_sidebox_end(void){
  cgi_printf("</td></tr></table>\n");
}

/* @-comment: // */
/*
** The default page header.
*/
const char zDefaultHeader[] = 
"<html>\n"
"<head>\n"
"<title>$<project_name>: $<title></title>\n"
"<link rel=\"alternate\" type=\"application/rss+xml\" title=\"RSS Feed\"\n"
"      href=\"$baseurl/timeline.rss\">\n"
"<link rel=\"stylesheet\" href=\"$baseurl/style.css?default\" type=\"text/css\"\n"
"      media=\"screen\">\n"
"</head>\n"
"<body>\n"
"<div class=\"header\">\n"
"  <div class=\"logo\">\n"
"    <img src=\"$baseurl/logo\" alt=\"logo\">\n"
"    <br><nobr>$<project_name></nobr>\n"
"  </div>\n"
"  <div class=\"title\">$<title></div>\n"
"  <div class=\"status\"><nobr><th1>\n"
"     if {[info exists login]} {\n"
"       puts \"Logged in as $login\"\n"
"     } else {\n"
"       puts \"Not logged in\"\n"
"     }\n"
"  </th1></nobr></div>\n"
"</div>\n"
"<div class=\"mainmenu\"><th1>\n"
"html \"<a href='$baseurl$index_page'>Home</a> \"\n"
"if {[anycap jor]} {\n"
"  html \"<a href='$baseurl/timeline'>Timeline</a> \"\n"
"}\n"
"if {[hascap oh]} {\n"
"  html \"<a href='$baseurl/dir?ci=tip'>Files</a> \"\n"
"}\n"
"if {[hascap o]} {\n"
"  html \"<a href='$baseurl/leaves'>Leaves</a> \"\n"
"  html \"<a href='$baseurl/brlist'>Branches</a> \"\n"
"  html \"<a href='$baseurl/taglist'>Tags</a> \"\n"
"}\n"
"if {[hascap r]} {\n"
"  html \"<a href='$baseurl/reportlist'>Tickets</a> \"\n"
"}\n"
"if {[hascap j]} {\n"
"  html \"<a href='$baseurl/wiki'>Wiki</a> \"\n"
"}\n"
"if {[hascap s]} {\n"
"  html \"<a href='$baseurl/setup'>Admin</a> \"\n"
"} elseif {[hascap a]} {\n"
"  html \"<a href='$baseurl/setup_ulist'>Users</a> \"\n"
"}\n"
"if {[info exists login]} {\n"
"  html \"<a href='$baseurl/login'>Logout</a> \"\n"
"} else {\n"
"  html \"<a href='$baseurl/login'>Login</a> \"\n"
"}\n"
"</th1></div>\n"
;

/*
** The default page footer
*/
const char zDefaultFooter[] = 
"<div class=\"footer\">\n"
"Fossil version $manifest_version $manifest_date\n"
"</div>\n"
"</body></html>\n"
;

/*
** The default Cascading Style Sheet.
*/
const char zDefaultCSS[] = 
"/* General settings for the entire page */\n"
"body {\n"
"  margin: 0ex 1ex;\n"
"  padding: 0px;\n"
"  background-color: white;\n"
"  font-family: sans-serif;\n"
"}\n"
"\n"
"/* The project logo in the upper left-hand corner of each page */\n"
"div.logo {\n"
"  display: table-cell;\n"
"  text-align: center;\n"
"  vertical-align: bottom;\n"
"  font-weight: bold;\n"
"  color: #558195;\n"
"}\n"
"\n"
"/* The page title centered at the top of each page */\n"
"div.title {\n"
"  display: table-cell;\n"
"  font-size: 2em;\n"
"  font-weight: bold;\n"
"  text-align: left;\n"
"  padding: 0 0 0 1em;\n"
"  color: #558195;\n"
"  vertical-align: bottom;\n"
"  width: 100%;\n"
"}\n"
"\n"
"/* The login status message in the top right-hand corner */\n"
"div.status {\n"
"  display: table-cell;\n"
"  text-align: right;\n"
"  vertical-align: bottom;\n"
"  color: #558195;\n"
"  font-size: 0.8em;\n"
"  font-weight: bold;\n"
"}\n"
"\n"
"/* The header across the top of the page */\n"
"div.header {\n"
"  display: table;\n"
"  width: 100%;\n"
"}\n"
"\n"
"/* The main menu bar that appears at the top of the page beneath\n"
"** the header */\n"
"div.mainmenu {\n"
"  padding: 5px 10px 5px 10px;\n"
"  font-size: 0.9em;\n"
"  font-weight: bold;\n"
"  text-align: center;\n"
"  letter-spacing: 1px;\n"
"  background-color: #558195;\n"
"  color: white;\n"
"}\n"
"\n"
"/* The submenu bar that *sometimes* appears below the main menu */\n"
"div.submenu {\n"
"  padding: 3px 10px 3px 0px;\n"
"  font-size: 0.9em;\n"
"  text-align: center;\n"
"  background-color: #456878;\n"
"  color: white;\n"
"}\n"
"div.mainmenu a, div.mainmenu a:visited, div.submenu a, div.submenu a:visited {\n"
"  padding: 3px 10px 3px 10px;\n"
"  color: white;\n"
"  text-decoration: none;\n"
"}\n"
"div.mainmenu a:hover, div.submenu a:hover {\n"
"  color: #558195;\n"
"  background-color: white;\n"
"}\n"
"\n"
"/* All page content from the bottom of the menu or submenu down to\n"
"** the footer */\n"
"div.content {\n"
"  padding: 0ex 1ex 0ex 2ex;\n"
"}\n"
"\n"
"/* Some pages have section dividers */\n"
"div.section {\n"
"  margin-bottom: 0px;\n"
"  margin-top: 1em;\n"
"  padding: 1px 1px 1px 1px;\n"
"  font-size: 1.2em;\n"
"  font-weight: bold;\n"
"  background-color: #558195;\n"
"  color: white;\n"
"}\n"
"\n"
"/* The \"Date\" that occurs on the left hand side of timelines */\n"
"div.divider {\n"
"  background: #a1c4d4;\n"
"  border: 2px #558195 solid;\n"
"  font-size: 1em; font-weight: normal;\n"
"  padding: .25em;\n"
"  margin: .2em 0 .2em 0;\n"
"  float: left;\n"
"  clear: left;\n"
"}\n"
"\n"
"/* The footer at the very bottom of the page */\n"
"div.footer {\n"
"  font-size: 0.8em;\n"
"  margin-top: 12px;\n"
"  padding: 5px 10px 5px 10px;\n"
"  text-align: right;\n"
"  background-color: #558195;\n"
"  color: white;\n"
"}\n"
"\n"
"/* Hyperlink colors in the footer */\n"
"div.footer a { color: white; }\n"
"div.footer a:link { color: white; }\n"
"div.footer a:visited { color: white; }\n"
"div.footer a:hover { background-color: white; color: #558195; }\n"
"\n"
"/* <verbatim> blocks */\n"
"pre.verbatim {\n"
"   background-color: #f5f5f5;\n"
"   padding: 0.5em;\n"
"}\n"
"\n"
"/* The label/value pairs on (for example) the ci page */\n"
"table.label-value th {\n"
"  vertical-align: top;\n"
"  text-align: right;\n"
"  padding: 0.2ex 2ex;\n"
"}\n"
;

/*
** WEBPAGE: style.css
*/
void page_style_css(void){
  char *zCSS = 0;

  cgi_set_content_type("text/css");
  zCSS = db_get("css",(char*)zDefaultCSS);
  cgi_append_content(zCSS, -1);
  g.isConst = 1;
}

/*
** WEBPAGE: test_env
*/
void page_test_env(void){
  style_header("Environment Test");
#if !defined(__MINGW32__)
  cgi_printf("uid=%d, gid=%d<br>\n",getuid(),getgid());
#endif
  cgi_printf("g.zBaseURL = %h<br>\n"
         "g.zTop = %h<br>\n",g.zBaseURL,g.zTop);
  cgi_print_all();
  style_footer();
}
