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
** This file contains code to implement the stat web page
**
*/
#include <string.h>
#include "config.h"
#include "stat.h"

/*
** WEBPAGE: stat
**
** Show statistics and global information about the repository.
*/
void stat_page(void){
  i64 t;
  int n, m, fsize;
  char zBuf[100];
  login_check_credentials();
  if( !g.okRead ){ login_needed(); return; }
  style_header("Repository Statistics");
  cgi_printf("<p><table class=\"label-value\">\n"
         "<tr><th>Repository&nbsp;Size:</th><td>\n");
  fsize = file_size(g.zRepositoryName);
  cgi_printf("%d bytes\n"
         "</td></tr>\n"
         "<tr><th>Number&nbsp;Of&nbsp;Artifacts:</th><td>\n",fsize);
  n = db_int(0, "SELECT count(*) FROM blob");
  m = db_int(0, "SELECT count(*) FROM delta");
  cgi_printf("%d (stored as %d full text and %d delta blobs)\n"
         "</td></tr>\n",n,n-m,m);
  if( n>0 ){
    int a, b;
    cgi_printf("<tr><th>Uncompressed&nbsp;Artifact&nbsp;Size:</th><td>\n");
    t = db_int64(0, "SELECT total(size) FROM blob WHERE size>0");
    sqlite3_snprintf(sizeof(zBuf), zBuf, "%lld", t);
    cgi_printf("%d bytes average, %s bytes total\n"
           "</td></tr>\n"
           "<tr><th>Compression&nbsp;Ratio:</th><td>\n",(int)(((double)t)/(double)n),zBuf);
    if( t/fsize < 5 ){
      b = 10;
      fsize /= 10;
    }else{
      b = 1;
    }
    a = t/fsize;
    cgi_printf("%d:%d\n"
           "</td></tr>\n",a,b);
  }
  cgi_printf("<tr><th>Number&nbsp;Of&nbsp;Check-ins:</th><td>\n");
  n = db_int(0, "SELECT count(distinct mid) FROM mlink");
  cgi_printf("%d\n"
         "</td></tr>\n"
         "<tr><th>Number&nbsp;Of&nbsp;Files:</th><td>\n",n);
  n = db_int(0, "SELECT count(*) FROM filename");
  cgi_printf("%d\n"
         "</td></tr>\n"
         "<tr><th>Number&nbsp;Of&nbsp;Wiki&nbsp;Pages:</th><td>\n",n);
  n = db_int(0, "SELECT count(*) FROM tag WHERE +tagname GLOB 'wiki-*'");
  cgi_printf("%d\n"
         "</td></tr>\n"
         "<tr><th>Number&nbsp;Of&nbsp;Tickets:</th><td>\n",n);
  n = db_int(0, "SELECT count(*) FROM tag WHERE +tagname GLOB 'tkt-*'");
  cgi_printf("%d\n"
         "</td></tr>\n"
         "<tr><th>Duration&nbsp;Of&nbsp;Project:</th><td>\n",n);
  n = db_int(0, "SELECT julianday('now') - (SELECT min(mtime) FROM event) + 0.99");
  cgi_printf("%d days\n"
         "</td></tr>\n"
         "<tr><th>Project&nbsp;ID:</th><td>\n"
         "%h\n"
         "</td></tr>\n"
         "<tr><th>Server&nbsp;ID:</th><td>\n"
         "%h\n"
         "</td></tr>\n"
         "</table></p>\n",n,db_get("project-code",""),db_get("server-code",""));
  style_footer();
}
