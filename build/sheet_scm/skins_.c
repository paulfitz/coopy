/*
** Copyright (c) 2009 D. Richard Hipp
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public
** License as published by the Free Software Foundation; either
** version 2 of the License, or (at your option) any later version.
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
** Implementation of the Setup page for "skins".
*/
#include <assert.h>
#include "config.h"
#include "skins.h"

/* @-comment: // */
/*
** A black-and-white theme with the project title in a bar across the top
** and no logo image.
*/
static const char zBuiltinSkin1[] = 
"REPLACE INTO config VALUES('css','/* General settings for the entire page */\n"
"body {\n"
"  margin: 0ex 1ex;\n"
"  padding: 0px;\n"
"  background-color: white;\n"
"  font-family: \"sans serif\";\n"
"}\n"
"\n"
"/* The project logo in the upper left-hand corner of each page */\n"
"div.logo {\n"
"  display: table-row;\n"
"  text-align: center;\n"
"  /* vertical-align: bottom;*/\n"
"  font-size: 2em;\n"
"  font-weight: bold;\n"
"  background-color: #707070;\n"
"  color: #ffffff;\n"
"}\n"
"\n"
"/* The page title centered at the top of each page */\n"
"div.title {\n"
"  display: table-cell;\n"
"  font-size: 1.5em;\n"
"  font-weight: bold;\n"
"  text-align: left;\n"
"  padding: 0 0 0 10px;\n"
"  color: #404040;\n"
"  vertical-align: bottom;\n"
"  width: 100%;\n"
"}\n"
"\n"
"/* The login status message in the top right-hand corner */\n"
"div.status {\n"
"  display: table-cell;\n"
"  text-align: right;\n"
"  vertical-align: bottom;\n"
"  color: #404040;\n"
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
"  background-color: #404040;\n"
"  color: white;\n"
"}\n"
"\n"
"/* The submenu bar that *sometimes* appears below the main menu */\n"
"div.submenu {\n"
"  padding: 3px 10px 3px 0px;\n"
"  font-size: 0.9em;\n"
"  text-align: center;\n"
"  background-color: #606060;\n"
"  color: white;\n"
"}\n"
"div.mainmenu a, div.mainmenu a:visited, div.submenu a, div.submenu a:visited {\n"
"  padding: 3px 10px 3px 10px;\n"
"  color: white;\n"
"  text-decoration: none;\n"
"}\n"
"div.mainmenu a:hover, div.submenu a:hover {\n"
"  color: #404040;\n"
"  background-color: white;\n"
"}\n"
"\n"
"/* All page content from the bottom of the menu or submenu down to\n"
"** the footer */\n"
"div.content {\n"
"  padding: 0ex 0ex 0ex 0ex;\n"
"}\n"
"/* Hyperlink colors */\n"
"div.content a { color: #604000; }\n"
"div.content a:link { color: #604000;}\n"
"div.content a:visited { color: #600000; }\n"
"\n"
"/* Some pages have section dividers */\n"
"div.section {\n"
"  margin-bottom: 0px;\n"
"  margin-top: 1em;\n"
"  padding: 1px 1px 1px 1px;\n"
"  font-size: 1.2em;\n"
"  font-weight: bold;\n"
"  background-color: #404040;\n"
"  color: white;\n"
"}\n"
"\n"
"/* The \"Date\" that occurs on the left hand side of timelines */\n"
"div.divider {\n"
"  background: #a0a0a0;\n"
"  border: 2px #505050 solid;\n"
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
"  background-color: #404040;\n"
"  color: white;\n"
"}\n"
"\n"
"/* The label/value pairs on (for example) the vinfo page */\n"
"table.label-value th {\n"
"  vertical-align: top;\n"
"  text-align: right;\n"
"  padding: 0.2ex 2ex;\n"
"}');\n"
"REPLACE INTO config VALUES('header','<html>\n"
"<head>\n"
"<title>$<project_name>: $<title></title>\n"
"<link rel=\"alternate\" type=\"application/rss+xml\" title=\"RSS Feed\"\n"
"      href=\"$baseurl/timeline.rss\">\n"
"<link rel=\"stylesheet\" href=\"$baseurl/style.css?blackwhite\" type=\"text/css\"\n"
"      media=\"screen\">\n"
"</head>\n"
"<body>\n"
"<div class=\"header\">\n"
"  <div class=\"logo\">\n"
"    <nobr>$<project_name></nobr>\n"
"  </div>\n"
"</div>\n"
"<div class=\"header\">\n"
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
"html \"<a href=''$baseurl$index_page''>Home</a> \"\n"
"if {[anycap jor]} {\n"
"  html \"<a href=''$baseurl/timeline''>Timeline</a> \"\n"
"}\n"
"if {[hascap oh]} {\n"
"  html \"<a href=''$baseurl/dir?ci=tip''>Files</a> \"\n"
"}\n"
"if {[hascap o]} {\n"
"  html \"<a href=''$baseurl/leaves''>Leaves</a> \"\n"
"  html \"<a href=''$baseurl/brlist''>Branches</a> \"\n"
"  html \"<a href=''$baseurl/taglist''>Tags</a> \"\n"
"}\n"
"if {[hascap r]} {\n"
"  html \"<a href=''$baseurl/reportlist''>Tickets</a> \"\n"
"}\n"
"if {[hascap j]} {\n"
"  html \"<a href=''$baseurl/wiki''>Wiki</a> \"\n"
"}\n"
"if {[hascap s]} {\n"
"  html \"<a href=''$baseurl/setup''>Admin</a> \"\n"
"} elseif {[hascap a]} {\n"
"  html \"<a href=''$baseurl/setup_ulist''>Users</a> \"\n"
"}\n"
"if {[info exists login]} {\n"
"  html \"<a href=''$baseurl/login''>Logout</a> \"\n"
"} else {\n"
"  html \"<a href=''$baseurl/login''>Login</a> \"\n"
"}\n"
"</th1></div>\n"
"');\n"
"REPLACE INTO config VALUES('footer','<div class=\"footer\">\n"
"Fossil version $manifest_version $manifest_date\n"
"</div>\n"
"</body></html>\n"
"');\n"
;

/*
** A tan theme with the project title above the user identification
** and no logo image.
*/
static const char zBuiltinSkin2[] = 
"REPLACE INTO config VALUES('css','/* General settings for the entire page */\n"
"body {\n"
"  margin: 0ex 0ex;\n"
"  padding: 0px;\n"
"  background-color: #fef3bc;\n"
"  font-family: sans-serif;\n"
"}\n"
"\n"
"/* The project logo in the upper left-hand corner of each page */\n"
"div.logo {\n"
"  display: inline;\n"
"  text-align: center;\n"
"  vertical-align: bottom;\n"
"  font-weight: bold;\n"
"  font-size: 2.5em;\n"
"  color: #a09048;\n"
"}\n"
"\n"
"/* The page title centered at the top of each page */\n"
"div.title {\n"
"  display: table-cell;\n"
"  font-size: 2em;\n"
"  font-weight: bold;\n"
"  text-align: left;\n"
"  padding: 0 0 0 5px;\n"
"  color: #a09048;\n"
"  vertical-align: bottom;\n"
"  width: 100%;\n"
"}\n"
"\n"
"/* The login status message in the top right-hand corner */\n"
"div.status {\n"
"  display: table-cell;\n"
"  text-align: right;\n"
"  vertical-align: bottom;\n"
"  color: #a09048;\n"
"  padding: 5px 5px 0 0;\n"
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
"  background-color: #a09048;\n"
"  color: black;\n"
"}\n"
"\n"
"/* The submenu bar that *sometimes* appears below the main menu */\n"
"div.submenu {\n"
"  padding: 3px 10px 3px 0px;\n"
"  font-size: 0.9em;\n"
"  text-align: center;\n"
"  background-color: #c0af58;\n"
"  color: white;\n"
"}\n"
"div.mainmenu a, div.mainmenu a:visited, div.submenu a, div.submenu a:visited {\n"
"  padding: 3px 10px 3px 10px;\n"
"  color: white;\n"
"  text-decoration: none;\n"
"}\n"
"div.mainmenu a:hover, div.submenu a:hover {\n"
"  color: #a09048;\n"
"  background-color: white;\n"
"}\n"
"\n"
"/* All page content from the bottom of the menu or submenu down to\n"
"** the footer */\n"
"div.content {\n"
"  padding: 1ex 5px;\n"
"}\n"
"div.content a { color: #706532; }\n"
"div.content a:link { color: #706532; }\n"
"div.content a:visited { color: #704032; }\n"
"div.content a:hover { background-color: white; color: #706532; }\n"
"\n"
"/* Some pages have section dividers */\n"
"div.section {\n"
"  margin-bottom: 0px;\n"
"  margin-top: 1em;\n"
"  padding: 3px 3px 0 3px;\n"
"  font-size: 1.2em;\n"
"  font-weight: bold;\n"
"  background-color: #a09048;\n"
"  color: white;\n"
"}\n"
"\n"
"/* The \"Date\" that occurs on the left hand side of timelines */\n"
"div.divider {\n"
"  background: #e1d498;\n"
"  border: 2px #a09048 solid;\n"
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
"  background-color: #a09048;\n"
"  color: white;\n"
"}\n"
"\n"
"/* Hyperlink colors */\n"
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
"');\n"
"REPLACE INTO config VALUES('header','<html>\n"
"<head>\n"
"<title>$<project_name>: $<title></title>\n"
"<link rel=\"alternate\" type=\"application/rss+xml\" title=\"RSS Feed\"\n"
"      href=\"$baseurl/timeline.rss\">\n"
"<link rel=\"stylesheet\" href=\"$baseurl/style.css?tan\" type=\"text/css\"\n"
"      media=\"screen\">\n"
"</head>\n"
"<body>\n"
"<div class=\"header\">\n"
"  <div class=\"title\">$<title></div>\n"
"  <div class=\"status\">\n"
"    <div class=\"logo\"><nobr>$<project_name></nobr></div><br/>\n"
"    <nobr><th1>\n"
"     if {[info exists login]} {\n"
"       puts \"Logged in as $login\"\n"
"     } else {\n"
"       puts \"Not logged in\"\n"
"     }\n"
"  </th1></nobr></div>\n"
"</div>\n"
"<div class=\"mainmenu\"><th1>\n"
"html \"<a href=''$baseurl$index_page''>Home</a> \"\n"
"if {[anycap jor]} {\n"
"  html \"<a href=''$baseurl/timeline''>Timeline</a> \"\n"
"}\n"
"if {[hascap oh]} {\n"
"  html \"<a href=''$baseurl/dir?ci=tip''>Files</a> \"\n"
"}\n"
"if {[hascap o]} {\n"
"  html \"<a href=''$baseurl/leaves''>Leaves</a> \"\n"
"  html \"<a href=''$baseurl/brlist''>Branches</a> \"\n"
"  html \"<a href=''$baseurl/taglist''>Tags</a> \"\n"
"}\n"
"if {[hascap r]} {\n"
"  html \"<a href=''$baseurl/reportlist''>Tickets</a> \"\n"
"}\n"
"if {[hascap j]} {\n"
"  html \"<a href=''$baseurl/wiki''>Wiki</a> \"\n"
"}\n"
"if {[hascap s]} {\n"
"  html \"<a href=''$baseurl/setup''>Admin</a> \"\n"
"} elseif {[hascap a]} {\n"
"  html \"<a href=''$baseurl/setup_ulist''>Users</a> \"\n"
"}\n"
"if {[info exists login]} {\n"
"  html \"<a href=''$baseurl/login''>Logout</a> \"\n"
"} else {\n"
"  html \"<a href=''$baseurl/login''>Login</a> \"\n"
"}\n"
"</th1></div>\n"
"');\n"
"REPLACE INTO config VALUES('footer','<div class=\"footer\">\n"
"Fossil version $manifest_version $manifest_date\n"
"</div>\n"
"</body></html>\n"
"');\n"
;

/*
** Black letters on a white or cream background with the main menu
** stuck on the left-hand side.
*/
static const char zBuiltinSkin3[] = 
"REPLACE INTO config VALUES('css','/* General settings for the entire page */\n"
"body {\n"
"    margin:0px 0px 0px 0px;\n"
"    padding:0px;\n"
"    font-family:verdana, arial, helvetica, \"sans serif\";\n"
"    color:#333;\n"
"    background-color:white;\n"
"}\n"
"\n"
"/* consistent colours */\n"
"h2 {\n"
"  color: #333;\n"
"}\n"
"h3 {\n"
"  color: #333;\n"
"}\n"
"\n"
"/* The project logo in the upper left-hand corner of each page */\n"
"div.logo {\n"
"  display: table-cell;\n"
"  text-align: left;\n"
"  vertical-align: bottom;\n"
"  font-weight: bold;\n"
"  color: #333;\n"
"}\n"
"\n"
"/* The page title centered at the top of each page */\n"
"div.title {\n"
"  display: table-cell;\n"
"  font-size: 2em;\n"
"  font-weight: bold;\n"
"  text-align: center;\n"
"  color: #333;\n"
"  vertical-align: bottom;\n"
"  width: 100%;\n"
"}\n"
"\n"
"/* The login status message in the top right-hand corner */\n"
"div.status {\n"
"  display: table-cell;\n"
"  padding-right: 10px;\n"
"  text-align: right;\n"
"  vertical-align: bottom;\n"
"  padding-bottom: 5px;\n"
"  color: #333;\n"
"  font-size: 0.8em;\n"
"  font-weight: bold;\n"
"}\n"
"\n"
"/* The header across the top of the page */\n"
"div.header {\n"
"    margin:10px 0px 10px 0px;\n"
"    padding:1px 0px 0px 20px;\n"
"    border-style:solid;\n"
"    border-color:black;\n"
"    border-width:1px 0px;\n"
"    background-color:#eee;\n"
"}\n"
"\n"
"/* The main menu bar that appears at the top left of the page beneath\n"
"** the header. Width must be co-ordinated with the container below */\n"
"div.mainmenu {\n"
"  float: left;\n"
"  margin-left: 10px;\n"
"  margin-right: 10px;\n"
"  font-size: 0.9em;\n"
"  font-weight: bold;\n"
"  padding:5px;\n"
"  background-color:#eee;\n"
"  border:1px solid #999;\n"
"  width:8em;\n"
"}\n"
"\n"
"/* Main menu is now a list */\n"
"div.mainmenu ul {\n"
"  padding: 0;\n"
"  list-style:none;\n"
"}\n"
"div.mainmenu a, div.mainmenu a:visited{\n"
"  padding: 1px 10px 1px 10px;\n"
"  color: #333;\n"
"  text-decoration: none;\n"
"}\n"
"div.mainmenu a:hover {\n"
"  color: #eee;\n"
"  background-color: #333;\n"
"}\n"
"\n"
"/* Container for the sub-menu and content so they don''t spread\n"
"** out underneath the main menu */\n"
"#container {\n"
"  padding-left: 9em;\n"
"}\n"
"\n"
"/* The submenu bar that *sometimes* appears below the main menu */\n"
"div.submenu {\n"
"  padding: 3px 10px 3px 10px;\n"
"  font-size: 0.9em;\n"
"  text-align: center;\n"
"  border:1px solid #999;\n"
"  border-width:1px 0px;\n"
"  background-color: #eee;\n"
"  color: #333;\n"
"}\n"
"div.submenu a, div.submenu a:visited {\n"
"  padding: 3px 10px 3px 10px;\n"
"  color: #333;\n"
"  text-decoration: none;\n"
"}\n"
"div.submenu a:hover {\n"
"  color: #eee;\n"
"  background-color: #333;\n"
"}\n"
"\n"
"/* All page content from the bottom of the menu or submenu down to\n"
"** the footer */\n"
"div.content {\n"
"  float right;\n"
"  padding: 2ex 1ex 0ex 2ex;\n"
"}\n"
"\n"
"/* Some pages have section dividers */\n"
"div.section {\n"
"  margin-bottom: 0px;\n"
"  margin-top: 1em;\n"
"  padding: 1px 1px 1px 1px;\n"
"  font-size: 1.2em;\n"
"  font-weight: bold;\n"
"  border-style:solid;\n"
"  border-color:#999;\n"
"  border-width:1px 0px;\n"
"  background-color: #eee;\n"
"  color: #333;\n"
"}\n"
"\n"
"/* The \"Date\" that occurs on the left hand side of timelines */\n"
"div.divider {\n"
"  background: #eee;\n"
"  border: 2px #999 solid;\n"
"  font-size: 1em; font-weight: normal;\n"
"  padding: .25em;\n"
"  margin: .2em 0 .2em 0;\n"
"  float: left;\n"
"  clear: left;\n"
"  color: #333\n"
"}\n"
"\n"
"/* The footer at the very bottom of the page */\n"
"div.footer {\n"
"  font-size: 0.8em;\n"
"  margin-top: 12px;\n"
"  padding: 5px 10px 5px 10px;\n"
"  text-align: right;\n"
"  background-color: #eee;\n"
"  color: #555;\n"
"}\n"
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
"}');\n"
"REPLACE INTO config VALUES('header','<html>\n"
"<head>\n"
"<title>$<project_name>: $<title></title>\n"
"<link rel=\"alternate\" type=\"application/rss+xml\" title=\"RSS Feed\"\n"
"      href=\"$baseurl/timeline.rss\">\n"
"<link rel=\"stylesheet\" href=\"$baseurl/style.css?black2\" type=\"text/css\"\n"
"      media=\"screen\">\n"
"</head>\n"
"<body>\n"
"<div class=\"header\">\n"
"  <div class=\"logo\">\n"
"    <!-- <img src=\"$baseurl/logo\" alt=\"logo\"> -->\n"
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
"<div class=\"mainmenu\"><ul><th1>\n"
"html \"<li><a href=''$baseurl$index_page''>Home</a></li>\"\n"
"if {[anycap jor]} {\n"
"  html \"<li><a href=''$baseurl/timeline''>Timeline</a></li>\"\n"
"}\n"
"if {[hascap oh]} {\n"
"  html \"<li><a href=''$baseurl/dir?ci=tip''>Files</a></li>\"\n"
"}\n"
"if {[hascap o]} {\n"
"  html \"<li><a href=''$baseurl/leaves''>Leaves</a></li>\"\n"
"  html \"<li><a href=''$baseurl/brlist''>Branches</a></li>\"\n"
"  html \"<li><a href=''$baseurl/taglist''>Tags</a></li>\"\n"
"}\n"
"if {[hascap r]} {\n"
"  html \"<li><a href=''$baseurl/reportlist''>Tickets</a></li>\"\n"
"}\n"
"if {[hascap j]} {\n"
"  html \"<li><a href=''$baseurl/wiki''>Wiki</a></li>\"\n"
"}\n"
"if {[hascap s]} {\n"
"  html \"<li><a href=''$baseurl/setup''>Admin</a></li>\"\n"
"} elseif {[hascap a]} {\n"
"  html \"<li><a href=''$baseurl/setup_ulist''>Users</a></li>\"\n"
"}\n"
"if {[info exists login]} {\n"
"  html \"<li><a href=''$baseurl/login''>Logout</a></li>\"\n"
"} else {\n"
"  html \"<li><a href=''$baseurl/login''>Login</a></li>\"\n"
"}\n"
"</th1></ul></div>\n"
"<div id=\"container\">\n"
"');\n"
"REPLACE INTO config VALUES('footer','</div>\n"
"<div class=\"footer\">\n"
"Fossil version $manifest_version $manifest_date\n"
"</div>\n"
"</body></html>\n"
"');\n"
;
/*
** An array of available built-in skins.
*/
static struct BuiltinSkin {
  const char *zName;
  const char *zValue;
} aBuiltinSkin[] = {
  { "Default",                     0 /* Filled in at runtime */ },
  { "Plain Gray, No Logo",         zBuiltinSkin1                },
  { "Khaki, No Logo",              zBuiltinSkin2                },
  { "Black & White, Menu on Left", zBuiltinSkin3                },
};

/*
** For a skin named zSkinName, compute the name of the CONFIG table
** entry where that skin is stored and return it.
**
** Return NULL if zSkinName is NULL or an empty string.
**
** If ifExists is true, and the named skin does not exist, return NULL.
*/
static char *skinVarName(const char *zSkinName, int ifExists){
  char *z;
  if( zSkinName==0 || zSkinName[0]==0 ) return 0;
  z = mprintf("skin:%s", zSkinName);
  if( ifExists && !db_exists("SELECT 1 FROM config WHERE name=%Q", z) ){
    free(z);
    z = 0;
  }
  return z;
}

/*
** Construct and return a string that represents the current skin if
** useDefault==0 or a string for the default skin if useDefault==1.
**
** Memory to hold the returned string is obtained from malloc.
*/
static char *getSkin(int useDefault){
  Blob val;
  blob_zero(&val);
  blob_appendf(&val, "REPLACE INTO config VALUES('css',%Q);\n",
     useDefault ? zDefaultCSS : db_get("css", (char*)zDefaultCSS)
  );
  blob_appendf(&val, "REPLACE INTO config VALUES('header',%Q);\n",
     useDefault ? zDefaultHeader : db_get("header", (char*)zDefaultHeader)
  );
  blob_appendf(&val, "REPLACE INTO config VALUES('footer',%Q);\n",
     useDefault ? zDefaultFooter : db_get("footer", (char*)zDefaultFooter)
  );
  return blob_str(&val);
}

/*
** Construct the default skin string and fill in the corresponding
** entry in aBuildinSkin[]
*/
static void setDefaultSkin(void){
  aBuiltinSkin[0].zValue = getSkin(1);
}

/*
** WEBPAGE: setup_skin
*/
void setup_skin(void){
  const char *z;
  char *zName;
  char *zErr = 0;
  const char *zCurrent;  /* Current skin */
  int i;                 /* Loop counter */
  Stmt q;

  login_check_credentials();
  if( !g.okSetup ){
    login_needed();
  }
  db_begin_transaction();

  /* Process requests to delete a user-defined skin */
  if( P("del1") && (zName = skinVarName(P("sn"), 1))!=0 ){
    style_header("Confirm Custom Skin Delete");
    cgi_printf("<form action=\"%s/setup_skin\" method=\"POST\">\n"
           "<p>Deletion of a custom skin is a permanent action that cannot\n"
           "be undone.  Please confirm that this is what you want to do:</p>\n"
           "<input type=\"hidden\" name=\"sn\" value=\"%h\">\n"
           "<input type=\"submit\" name=\"del2\" value=\"Confirm - Delete The Skin\">\n"
           "<input type=\"submit\" name=\"cancel\" value=\"Cancel - Do Not Delete\">\n",g.zBaseURL,P("sn"));
    login_insert_csrf_secret();
    cgi_printf("</form>\n");
    style_footer();
    return;
  }
  if( P("del2")!=0 && (zName = skinVarName(P("sn"), 1))!=0 ){
    db_multi_exec("DELETE FROM config WHERE name=%Q", zName);
  }

  setDefaultSkin();
  zCurrent = getSkin(0);

  if( P("save")!=0 && (zName = skinVarName(P("save"),0))!=0 ){
    if( db_exists("SELECT 1 FROM config WHERE name=%Q", zName)
          || strcmp(zName, "Default")==0 ){
      zErr = mprintf("Skin name \"%h\" already exists. "
                     "Choose a different name.", P("sn"));
    }else{
      db_multi_exec("INSERT INTO config VALUES(%Q,%Q)",
         zName, zCurrent
      );
    }
  }

  /* The user pressed the "Use This Skin" button. */
  if( P("load") && (z = P("sn"))!=0 && z[0] ){
    int seen = 0;
    for(i=0; i<sizeof(aBuiltinSkin)/sizeof(aBuiltinSkin[0]); i++){
      if( strcmp(aBuiltinSkin[i].zValue, zCurrent)==0 ){
        seen = 1;
        break;
      }
    }
    if( !seen ){
      seen = db_exists("SELECT 1 FROM config WHERE name GLOB 'skin:*'"
                       " AND value=%Q", zCurrent);
    }
    if( !seen ){
      db_multi_exec(
        "INSERT INTO config VALUES("
        "  strftime('skin:Backup On %%Y-%%m-%%d %%H:%%M:%%S'),"
        "  %Q)", zCurrent
      );
    }
    seen = 0;
    for(i=0; i<sizeof(aBuiltinSkin)/sizeof(aBuiltinSkin[0]); i++){
      if( strcmp(aBuiltinSkin[i].zName, z)==0 ){
        seen = 1;
        zCurrent = aBuiltinSkin[i].zValue;
        db_multi_exec("%s", zCurrent);
        break;
      }
    }
    if( !seen ){
      zName = skinVarName(z,0);
      zCurrent = db_get(zName, 0);
      db_multi_exec("%s", zCurrent);
    }
  }

  style_header("Skins");
  cgi_printf("<p>A \"skin\" is a combination of\n"
         "<a href=\"setup_editcss\">CSS</a>, \n"
         "<a href=\"setup_header\">Header</a>, and \n"
         "<a href=\"setup_footer\">Footer</a> that determines the look and feel\n"
         "of the web interface.</p>\n"
         "\n"
         "<h2>Available Skins:</h2>\n"
         "<ol>\n");
  for(i=0; i<sizeof(aBuiltinSkin)/sizeof(aBuiltinSkin[0]); i++){
    z = aBuiltinSkin[i].zName;
    if( strcmp(aBuiltinSkin[i].zValue, zCurrent)==0 ){
      cgi_printf("<li><p>%h.&nbsp;&nbsp; <b>Currently In Use</b></p>\n",z);
    }else{
      cgi_printf("<li><form action=\"%s/setup_skin\" method=\"POST\">\n"
             "%h.&nbsp;&nbsp; \n"
             "<input type=\"hidden\" name=\"sn\" value=\"%h\">\n"
             "<input type=\"submit\" name=\"load\" value=\"Use This Skin\">\n"
             "</form></li>\n",g.zBaseURL,z,z);
    }
  }
  db_prepare(&q,
     "SELECT substr(name, 6), value FROM config"
     " WHERE name GLOB 'skin:*'"
     " ORDER BY name"
  );
  while( db_step(&q)==SQLITE_ROW ){
    const char *zN = db_column_text(&q, 0);
    const char *zV = db_column_text(&q, 1);
    if( strcmp(zV, zCurrent)==0 ){
      cgi_printf("<li><p>%h.&nbsp;&nbsp;  <b>Currently In Use</b></p>\n",zN);
    }else{
      cgi_printf("<li><form action=\"%s/setup_skin\" method=\"POST\">\n"
             "%h.&nbsp;&nbsp; \n"
             "<input type=\"hidden\" name=\"sn\" value=\"%h\">\n"
             "<input type=\"submit\" name=\"load\" value=\"Use This Skin\">\n"
             "<input type=\"submit\" name=\"del1\" value=\"Delete This Skin\">\n"
             "</form></li>\n",g.zBaseURL,zN,zN);
    }
  }
  db_finalize(&q);
  cgi_printf("</ol>\n");
  style_footer();
  db_end_transaction(0);
}
