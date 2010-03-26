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
** This file contains code to implement the timeline web page
**
*/
#include <string.h>
#include <time.h>
#include "config.h"
#include "timeline.h"

/*
** Shorten a UUID so that is the minimum length needed to contain
** at least one digit in the range 'a'..'f'.  The minimum length is 10.
*/
static void shorten_uuid(char *zDest, const char *zSrc){
  int i;
  for(i=0; i<10 && zSrc[i]<='9'; i++){}
  memcpy(zDest, zSrc, 10);
  if( i==10 && zSrc[i] ){
    do{
      zDest[i] = zSrc[i];
      i++;
    }while( zSrc[i-1]<='9' );
  }else{
    i = 10;
  }
  zDest[i] = 0;
}


/*
** Generate a hyperlink to a version.
*/
void hyperlink_to_uuid(const char *zUuid){
  char zShortUuid[UUID_SIZE+1];
  shorten_uuid(zShortUuid, zUuid);
  if( g.okHistory ){
    cgi_printf("<a href=\"%s/info/%s\">[%s]</a>\n",g.zBaseURL,zShortUuid,zShortUuid);
  }else{
    cgi_printf("<b>[%s]</b>\n",zShortUuid);
  }
}

/*
** Generate a hyperlink that invokes javascript to highlight
** a version on mouseover.
*/
void hyperlink_to_uuid_with_mouseover(
  const char *zUuid,   /* The UUID to display */
  const char *zIn,     /* Javascript proc for mouseover */
  const char *zOut,    /* Javascript proc for mouseout */
  int id               /* Argument to javascript procs */
){
  char zShortUuid[UUID_SIZE+1];
  shorten_uuid(zShortUuid, zUuid);
  if( g.okHistory ){
    cgi_printf("<a onmouseover='%s(\"m%d\")' onmouseout='%s(\"m%d\")'\n"
           "   href=\"%s/vinfo/%s\">[%s]</a>\n",zIn,id,zOut,id,g.zBaseURL,zShortUuid,zShortUuid);
  }else{
    cgi_printf("<b onmouseover='%s(\"m%d\")' onmouseout='%s(\"m%d\")'>\n"
           "[%s]</b>\n",zIn,id,zOut,id,zShortUuid);
  }
}

/*
** Generate a hyperlink to a diff between two versions.
*/
void hyperlink_to_diff(const char *zV1, const char *zV2){
  if( g.okHistory ){
    if( zV2==0 ){
      cgi_printf("<a href=\"%s/diff?v2=%s\">[diff]</a>\n",g.zBaseURL,zV1);
    }else{
      cgi_printf("<a href=\"%s/diff?v1=%s&v2=%s\">[diff]</a>\n",g.zBaseURL,zV1,zV2);
    }
  }
}

/*
** Generate a hyperlink to a date & time.
*/
void hyperlink_to_date(const char *zDate, const char *zSuffix){
  if( zSuffix==0 ) zSuffix = "";
  if( g.okHistory ){
    cgi_printf("<a href=\"%s/timeline?c=%T\">%s</a>%s\n",g.zTop,zDate,zDate,zSuffix);
  }else{
    cgi_printf("%s%s\n",zDate,zSuffix);
  }
}

/*
** Generate a hyperlink to a user.  This will link to a timeline showing
** events by that user.  If the date+time is specified, then the timeline
** is centered on that date+time.
*/
void hyperlink_to_user(const char *zU, const char *zD, const char *zSuf){
  if( zSuf==0 ) zSuf = "";
  if( g.okHistory ){
    if( zD && zD[0] ){
      cgi_printf("<a href=\"%s/timeline?c=%T&u=%T\">%h</a>%s\n",g.zTop,zD,zU,zU,zSuf);
    }else{
      cgi_printf("<a href=\"%s/timeline?u=%T\">%h</a>%s\n",g.zTop,zU,zU,zSuf);
    }
  }else{
    cgi_printf("%s\n",zU);
  }
}

/*
** Count the number of primary non-branch children for the given check-in.
**
** A primary child is one where the parent is the primary parent, not
** a merge parent.
**
** A non-branch child is one which is on the same branch as the parent.
*/
int count_nonbranch_children(int pid){
  int nNonBranch;
  static const char zSql[] = 
    "SELECT count(*) FROM plink\n"
    " WHERE pid=%d AND isprim\n"
    "   AND coalesce((SELECT value FROM tagxref\n"
    "                  WHERE tagid=%d AND rid=plink.pid), 'trunk')\n"
    "      =coalesce((SELECT value FROM tagxref\n"
    "                  WHERE tagid=%d AND rid=plink.cid), 'trunk')\n"
  ;
  nNonBranch = db_int(0, zSql, pid, TAG_BRANCH, TAG_BRANCH);
  return nNonBranch;
}

/*
** Allowed flags for the tmFlags argument to www_print_timeline
*/
#if INTERFACE
#define TIMELINE_ARTID    0x0001  /* Show artifact IDs on non-check-in lines */
#define TIMELINE_LEAFONLY 0x0002  /* Show "Leaf", but not "Merge", "Fork" etc */
#define TIMELINE_BRIEF    0x0004  /* Combine adjacent elements of same object */
#define TIMELINE_GRAPH    0x0008  /* Compute a graph */
#define TIMELINE_DISJOINT 0x0010  /* Elements are not contiguous */
#endif

/*
** Output a timeline in the web format given a query.  The query
** should return these columns:
**
**    0.  rid
**    1.  UUID
**    2.  Date/Time
**    3.  Comment string
**    4.  User
**    5.  Number of non-merge children
**    6.  Number of parents
**    7.  True if is a leaf
**    8.  background color
**    9.  type ("ci", "w", "t")
**   10.  list of symbolic tags.
**   11.  tagid for ticket or wiki
**   12.  Short comment to user for repeated tickets and wiki
*/
void www_print_timeline(
  Stmt *pQuery,          /* Query to implement the timeline */
  int tmFlags,           /* Flags controlling display behavior */
  void (*xExtra)(int)    /* Routine to call on each line of display */
){
  int wikiFlags;
  int mxWikiLen;
  Blob comment;
  int prevTagid = 0;
  int suppressCnt = 0;
  char zPrevDate[20];
  GraphContext *pGraph = 0;

  zPrevDate[0] = 0;
  mxWikiLen = db_get_int("timeline-max-comment", 0);
  if( db_get_boolean("timeline-block-markup", 0) ){
    wikiFlags = WIKI_INLINE;
  }else{
    wikiFlags = WIKI_INLINE | WIKI_NOBLOCK;
  }
  if( tmFlags & TIMELINE_GRAPH ){
    pGraph = graph_init();
    cgi_printf("<div id=\"canvas\" style=\"position:relative;width:1px;height:1px;\"></div>\n");
  }

  db_multi_exec(
     "CREATE TEMP TABLE IF NOT EXISTS seen(rid INTEGER PRIMARY KEY);"
     "DELETE FROM seen;"
  );
  cgi_printf("<table cellspacing=0 border=0 cellpadding=0>\n");
  blob_zero(&comment);
  while( db_step(pQuery)==SQLITE_ROW ){
    int rid = db_column_int(pQuery, 0);
    const char *zUuid = db_column_text(pQuery, 1);
    int nPChild = db_column_int(pQuery, 5);
    int nParent = db_column_int(pQuery, 6);
    int isLeaf = db_column_int(pQuery, 7);
    const char *zBgClr = db_column_text(pQuery, 8);
    const char *zDate = db_column_text(pQuery, 2);
    const char *zType = db_column_text(pQuery, 9);
    const char *zUser = db_column_text(pQuery, 4);
    const char *zTagList = db_column_text(pQuery, 10);
    int tagid = db_column_int(pQuery, 11);
    int commentColumn = 3;    /* Column containing comment text */
    char zTime[8];
    if( tagid ){
      if( tagid==prevTagid ){
        if( tmFlags & TIMELINE_BRIEF ){
          suppressCnt++;
          continue;
        }else{
          commentColumn = 12;
        }
      }
    }
    prevTagid = tagid;
    if( suppressCnt ){
      cgi_printf("<tr><td><td><td>\n"
             "<small><i>... %d similar\n"
             "event%s omitted.</i></small></tr>\n",suppressCnt,suppressCnt>1?"s":"");
      suppressCnt = 0;
    }
    if( strcmp(zType,"div")==0 ){
      cgi_printf("<tr><td colspan=3><hr></td></tr>\n");
      continue;
    }
    db_multi_exec("INSERT OR IGNORE INTO seen VALUES(%d)", rid);
    if( memcmp(zDate, zPrevDate, 10) ){
      sprintf(zPrevDate, "%.10s", zDate);
      cgi_printf("<tr><td>\n"
             "  <div class=\"divider\"><nobr>%s</nobr></div>\n"
             "</td></tr>\n",zPrevDate);
    }
    memcpy(zTime, &zDate[11], 5);
    zTime[5] = 0;
    cgi_printf("<tr>\n"
           "<td valign=\"top\" align=\"right\">%s</td>\n"
           "<td width=\"20\" align=\"left\" valign=\"top\">\n"
           "<div id=\"m%d\"></div>\n",zTime,rid);
    if( zBgClr && zBgClr[0] ){
      cgi_printf("<td valign=\"top\" align=\"left\" bgcolor=\"%h\">\n",zBgClr);
    }else{
      cgi_printf("<td valign=\"top\" align=\"left\">\n");
    }
    if( zType[0]=='c' ){
      const char *azTag[5];
      int nTag = 0;
      hyperlink_to_uuid(zUuid);
      if( (tmFlags & TIMELINE_LEAFONLY)==0 ){
        if( nParent>1 ){
          azTag[nTag++] = "Merge";
        }
        if( nPChild>1 ){
          if( count_nonbranch_children(rid)>1 ){
            azTag[nTag++] = "Fork";
          }else{
            azTag[nTag++] = "Branch-Point";
          }
        }
      }
      if( isLeaf ){
        if( db_exists("SELECT 1 FROM tagxref"
                      " WHERE rid=%d AND tagid=%d AND tagtype>0",
                      rid, TAG_CLOSED) ){
          azTag[nTag++] = "Closed-Leaf";
        }else{
          azTag[nTag++] = "Leaf";
        }
      }
      if( nTag>0 ){
        int i;
        for(i=0; i<nTag; i++){
          cgi_printf("<b>%s%s</b>\n",azTag[i],i==nTag-1?"":",");
        }
      }
      if( pGraph ){
        int nParent = 0;
        int aParent[32];
        const char *zBr;
        static Stmt qparent;
        static Stmt qbranch;
        db_static_prepare(&qparent,
          "SELECT pid FROM plink WHERE cid=:rid ORDER BY isprim DESC"
        );
        db_static_prepare(&qbranch,
          "SELECT value FROM tagxref WHERE tagid=%d AND tagtype>0 AND rid=:rid",
          TAG_BRANCH
        );
        db_bind_int(&qparent, ":rid", rid);
        while( db_step(&qparent)==SQLITE_ROW && nParent<32 ){
          aParent[nParent++] = db_column_int(&qparent, 0);
        }
        db_reset(&qparent);
        db_bind_int(&qbranch, ":rid", rid);
        if( db_step(&qbranch)==SQLITE_ROW ){
          zBr = db_column_text(&qbranch, 0);
        }else{
          zBr = "trunk";
        }
        graph_add_row(pGraph, rid, isLeaf, nParent, aParent, zBr);
        db_reset(&qbranch);
      }
    }else if( (tmFlags & TIMELINE_ARTID)!=0 ){
      hyperlink_to_uuid(zUuid);
    }
    db_column_blob(pQuery, commentColumn, &comment);
    if( mxWikiLen>0 && blob_size(&comment)>mxWikiLen ){
      Blob truncated;
      blob_zero(&truncated);
      blob_append(&truncated, blob_buffer(&comment), mxWikiLen);
      blob_append(&truncated, "...", 3);
      wiki_convert(&truncated, 0, wikiFlags);
      blob_reset(&truncated);
    }else{
      wiki_convert(&comment, 0, wikiFlags);
    }
    blob_reset(&comment);
    if( zTagList && zTagList[0] ){
      cgi_printf("(user: %h, tags: %h)\n",zUser,zTagList);
    }else{
      cgi_printf("(user: %h)\n",zUser);
    }
    if( xExtra ){
      xExtra(rid);
    }
    cgi_printf("</td></tr>\n");
  }
  if( suppressCnt ){
    cgi_printf("<tr><td><td><td>\n"
           "<small><i>... %d similar\n"
           "event%s omitted.</i></small></tr>\n",suppressCnt,suppressCnt>1?"s":"");
    suppressCnt = 0;
  }
  if( pGraph ){
    graph_finish(pGraph, (tmFlags & TIMELINE_DISJOINT)!=0);
    if( pGraph->nErr ){
      graph_free(pGraph);
      pGraph = 0;
    }else{
      cgi_printf("<tr><td><td><div style=\"width:%dpx;\"></div>\n",pGraph->mxRail*20+30);
    }
  }
  cgi_printf("</table>\n");
  if( pGraph && pGraph->nErr==0 ){
    GraphRow *pRow;
    int i;
    char cSep;
    cgi_printf("<script type=\"text/JavaScript\">\n");
    cgi_printf("var rowinfo = [\n");
    for(pRow=pGraph->pFirst; pRow; pRow=pRow->pNext){
      cgi_printf("{id:\"m%d\",r:%d,d:%d,mo:%d,mu:%d,u:%d,au:",
        pRow->rid,
        pRow->iRail,
        pRow->bDescender,
        pRow->mergeOut,
        pRow->mergeUpto,
        pRow->aiRaiser[pRow->iRail]
      );
      cSep = '[';
      for(i=0; i<GR_MAX_RAIL; i++){
        if( i==pRow->iRail ) continue;
        if( pRow->aiRaiser[i]>0 ){
          cgi_printf("%c%d,%d", cSep, pGraph->railMap[i], pRow->aiRaiser[i]);
          cSep = ',';
        }
      }
      if( cSep=='[' ) cgi_printf("[");
      cgi_printf("],mi:");
      cSep = '[';
      for(i=0; i<GR_MAX_RAIL; i++){
        if( pRow->mergeIn & (1<<i) ){
          cgi_printf("%c%d", cSep, pGraph->railMap[i]);
          cSep = ',';
        }
      }
      if( cSep=='[' ) cgi_printf("[");
      cgi_printf("]}%s", pRow->pNext ? ",\n" : "];\n");
    }
    cgi_printf("var nrail = %d\n", pGraph->mxRail+1);
    graph_free(pGraph);
    cgi_printf("var canvasDiv = document.getElementById(\"canvas\");\n"
           "var realCanvas = null;\n"
           "function drawBox(color,x0,y0,x1,y1){\n"
           "  var n = document.createElement(\"div\");\n"
           "  if( x0>x1 ){ var t=x0; x0=x1; x1=t; }\n"
           "  if( y0>y1 ){ var t=y0; y0=y1; y1=t; }\n"
           "  var w = x1-x0+1;\n"
           "  var h = y1-y0+1;\n"
           "  canvasDiv.appendChild(n);\n"
           "  n.style.position = \"absolute\";\n"
           "  n.style.overflow = \"hidden\";\n"
           "  n.style.left = x0+\"px\";\n"
           "  n.style.top = y0+\"px\";\n"
           "  n.style.width = w+\"px\";\n"
           "  n.style.height = h+\"px\";\n"
           "  n.style.backgroundColor = color;\n"
           "}\n"
           "function absoluteY(id){\n"
           "  var obj = document.getElementById(id);\n"
           "  if( !obj ) return;\n"
           "  var top = 0;\n"
           "  if( obj.offsetParent ){\n"
           "    do{\n"
           "      top += obj.offsetTop;\n"
           "    }while( obj = obj.offsetParent );\n"
           "  }\n"
           "  return top;\n"
           "}\n"
           "function absoluteX(id){\n"
           "  var obj = document.getElementById(id);\n"
           "  if( !obj ) return;\n"
           "  var left = 0;\n"
           "  if( obj.offsetParent ){\n"
           "    do{\n"
           "      left += obj.offsetLeft;\n"
           "    }while( obj = obj.offsetParent );\n"
           "  }\n"
           "  return left;\n"
           "}\n"
           "function drawUpArrow(x,y0,y1){\n"
           "  drawBox(\"black\",x,y0,x+1,y1);\n"
           "  if( y0+8>=y1 ){\n"
           "    drawBox(\"black\",x-1,y0+1,x+2,y0+2);\n"
           "    drawBox(\"black\",x-2,y0+3,x+3,y0+4);\n"
           "  }else{\n"
           "    drawBox(\"black\",x-1,y0+2,x+2,y0+4);\n"
           "    drawBox(\"black\",x-2,y0+5,x+3,y0+7);\n"
           "  }\n"
           "}\n"
           "function drawThinArrow(y,xFrom,xTo){\n"
           "  if( xFrom<xTo ){\n"
           "    drawBox(\"black\",xFrom,y,xTo,y);\n"
           "    drawBox(\"black\",xTo-4,y-1,xTo-2,y+1);\n"
           "    if( xTo>xFrom-8 ) drawBox(\"black\",xTo-6,y-2,xTo-5,y+2);\n"
           "  }else{\n"
           "    drawBox(\"black\",xTo,y,xFrom,y);\n"
           "    drawBox(\"black\",xTo+2,y-1,xTo+4,y+1);\n"
           "    if( xTo+8<xFrom ) drawBox(\"black\",xTo+5,y-2,xTo+6,y+2);\n"
           "  }\n"
           "}\n"
           "function drawThinLine(x0,y0,x1,y1){\n"
           "  drawBox(\"black\",x0,y0,x1,y1);\n"
           "}\n"
           "function drawNode(p, left, btm){\n"
           "  drawBox(\"black\",p.x-5,p.y-5,p.x+6,p.y+6);\n"
           "  drawBox(\"white\",p.x-4,p.y-4,p.x+5,p.y+5);\n"
           "  if( p.u>0 ){\n"
           "    var u = rowinfo[p.u-1];\n"
           "    drawUpArrow(p.x, u.y+6, p.y-5);\n"
           "  }\n"
           "  if( p.d ){\n"
           "    drawUpArrow(p.x, p.y+6, btm);\n"
           "  } \n"
           "  if( p.mo>=0 ){\n"
           "    var x1 = p.mo*20 + left;\n"
           "    var y1 = p.y-3;\n"
           "    var x0 = x1>p.x ? p.x+7 : p.x-6;\n"
           "    var u = rowinfo[p.mu-1];\n"
           "    var y0 = u.y+5;\n"
           "    drawThinLine(x0,y1,x1,y1);\n"
           "    drawThinLine(x1,y0,x1,y1);\n"
           "  }\n"
           "  var n = p.au.length;\n"
           "  for(var i=0; i<n; i+=2){\n"
           "    var x1 = p.au[i]*20 + left;\n"
           "    var x0 = x1>p.x ? p.x+7 : p.x-6;\n"
           "    drawBox(\"black\",x0,p.y,x1,p.y+1);\n"
           "    var u = rowinfo[p.au[i+1]-1];\n"
           "    drawUpArrow(x1, u.y+6, p.y);\n"
           "  }\n"
           "  for(var j in p.mi){\n"
           "    var y0 = p.y+5;\n"
           "    var mx = p.mi[j]*20 + left;\n"
           "    if( mx>p.x ){\n"
           "      drawThinArrow(y0,mx,p.x+5);\n"
           "    }else{\n"
           "      drawThinArrow(y0,mx,p.x-5);\n"
           "    }\n"
           "  }\n"
           "}\n"
           "function renderGraph(){\n"
           "  var canvasDiv = document.getElementById(\"canvas\");\n"
           "  while( canvasDiv.hasChildNodes() ){\n"
           "    canvasDiv.removeChild(canvasDiv.firstChild);\n"
           "  }\n"
           "  var canvasY = absoluteY(\"canvas\");\n"
           "  var left = absoluteX(rowinfo[0].id) - absoluteX(\"canvas\") + 15;\n"
           "  var width = nrail*20;\n"
           "  for(var i in rowinfo){\n"
           "    rowinfo[i].y = absoluteY(rowinfo[i].id) + 10 - canvasY;\n"
           "    rowinfo[i].x = left + rowinfo[i].r*20;\n"
           "  }\n"
           "  var btm = rowinfo[rowinfo.length-1].y + 20;\n"
           "  canvasDiv.innerHTML = '<canvas id=\"timeline-canvas\" '+\n"
           "     'style=\"position:absolute;left:'+(left-5)+'px;\"' +\n"
           "     ' width=\"'+width+'\" height=\"'+btm+'\"></canvas>';\n"
           "  realCanvas = document.getElementById('timeline-canvas');\n"
           "  var context;\n"
           "  if( realCanvas && realCanvas.getContext\n"
           "       && (context = realCanvas.getContext('2d'))) {\n"
           "    drawBox = function(color,x0,y0,x1,y1) {\n"
           "      var colors = {\n"
           "         'white':'rgba(255,255,255,1)',\n"
           "         'black':'rgba(0,0,0,1)'\n"
           "      };\n"
           "      if( x0>x1 ){ var t=x0; x0=x1; x1=t; }\n"
           "      if( y0>y1 ){ var t=y0; y0=y1; y1=t; }\n"
           "      if(isNaN(x0) || isNaN(y0) || isNaN(x1) || isNaN(y1)) return;\n"
           "      context.fillStyle = colors[color];\n"
           "      context.fillRect(x0-left+5,y0,x1-x0+1,y1-y0+1);\n"
           "    };\n"
           "  }\n"
           "  for(var i in rowinfo){\n"
           "    drawNode(rowinfo[i], left, btm);\n"
           "  }\n"
           "}\n"
           "var lastId = rowinfo[rowinfo.length-1].id;\n"
           "var lastY = 0;\n"
           "function checkHeight(){\n"
           "  var h = absoluteY(lastId);\n"
           "  if( h!=lastY ){\n"
           "    renderGraph();\n"
           "    lastY = h;\n"
           "  }\n"
           "  setTimeout(\"checkHeight();\", 1000);\n"
           "}\n"
           "checkHeight();\n"
           "</script>\n");
  }
}

/*
** Create a temporary table suitable for storing timeline data.
*/
static void timeline_temp_table(void){
  static const char zSql[] = 
    "CREATE TEMP TABLE IF NOT EXISTS timeline(\n"
    "  rid INTEGER PRIMARY KEY,\n"
    "  uuid TEXT,\n"
    "  timestamp TEXT,\n"
    "  comment TEXT,\n"
    "  user TEXT,\n"
    "  nchild INTEGER,\n"
    "  nparent INTEGER,\n"
    "  isleaf BOOLEAN,\n"
    "  bgcolor TEXT,\n"
    "  etype TEXT,\n"
    "  taglist TEXT,\n"
    "  tagid INTEGER,\n"
    "  short TEXT\n"
    ")\n"
  ;
  db_multi_exec(zSql);
}

/*
** Return a pointer to a constant string that forms the basis
** for a timeline query for the WWW interface.
*/
const char *timeline_query_for_www(void){
  static char *zBase = 0;
  static const char zBaseSql[] =
    "SELECT\n"
    "  blob.rid,\n"
    "  uuid,\n"
    "  datetime(event.mtime,'localtime') AS timestamp,\n"
    "  coalesce(ecomment, comment),\n"
    "  coalesce(euser, user),\n"
    "  (SELECT count(*) FROM plink WHERE pid=blob.rid AND isprim=1),\n"
    "  (SELECT count(*) FROM plink WHERE cid=blob.rid),\n"
    "  NOT EXISTS(SELECT 1 FROM plink\n"
    "              WHERE pid=blob.rid\n"
    "               AND coalesce((SELECT value FROM tagxref\n"
    "                             WHERE tagid=%d AND rid=plink.pid), 'trunk')\n"
    "                 = coalesce((SELECT value FROM tagxref\n"
    "                             WHERE tagid=%d AND rid=plink.cid), 'trunk')),\n"
    "  bgcolor,\n"
    "  event.type,\n"
    "  (SELECT group_concat(substr(tagname,5), ', ') FROM tag, tagxref\n"
    "    WHERE tagname GLOB 'sym-*' AND tag.tagid=tagxref.tagid\n"
    "      AND tagxref.rid=blob.rid AND tagxref.tagtype>0),\n"
    "  tagid,\n"
    "  brief\n"
    " FROM event JOIN blob\n"
    "WHERE blob.rid=event.objid\n"
  ;
  if( zBase==0 ){
    zBase = mprintf(zBaseSql, TAG_BRANCH, TAG_BRANCH);
  }
  return zBase;
}

/*
** Generate a submenu element with a single parameter change.
*/
static void timeline_submenu(
  HQuery *pUrl,            /* Base URL */
  const char *zMenuName,   /* Submenu name */
  const char *zParam,      /* Parameter value to add or change */
  const char *zValue,      /* Value of the new parameter */
  const char *zRemove      /* Parameter to omit */
){
  style_submenu_element(zMenuName, zMenuName, "%s",
                        url_render(pUrl, zParam, zValue, zRemove, 0));
}


/*
** zDate is a localtime date.  Insert records into the
** "timeline" table to cause <hr> to be inserted before and after
** entries of that date.
*/
static void timeline_add_dividers(const char *zDate){
  db_multi_exec(
    "INSERT INTO timeline(rid,timestamp,etype)"
    "VALUES(-1,datetime(%Q,'-1 second') || '.9','div')",
    zDate
  );
  db_multi_exec(
    "INSERT INTO timeline(rid,timestamp,etype)"
    "VALUES(-2,datetime(%Q) || '.1','div')",
     zDate
  );
}


/*
** WEBPAGE: timeline
**
** Query parameters:
**
**    a=TIMESTAMP    after this date
**    b=TIMESTAMP    before this date.
**    c=TIMESTAMP    "circa" this date.
**    n=COUNT        number of events in output
**    p=RID          artifact RID and up to COUNT parents and ancestors
**    d=RID          artifact RID and up to COUNT descendants
**    t=TAGID        show only check-ins with the given tagid
**    u=USER         only if belonging to this user
**    y=TYPE         'ci', 'w', 't'
**    s=TEXT         string search (comment and brief)
**    ng             Suppress the graph if present
**
** p= and d= can appear individually or together.  If either p= or d=
** appear, then u=, y=, a=, and b= are ignored.
**
** If a= and b= appear, only a= is used.  If neither appear, the most
** recent events are choosen.
**
** If n= is missing, the default count is 20.
*/
void page_timeline(void){
  Stmt q;                            /* Query used to generate the timeline */
  Blob sql;                          /* text of SQL used to generate timeline */
  Blob desc;                         /* Description of the timeline */
  int nEntry = atoi(PD("n","20"));   /* Max number of entries on timeline */
  int p_rid = atoi(PD("p","0"));     /* artifact p and its parents */
  int d_rid = atoi(PD("d","0"));     /* artifact d and its descendants */
  const char *zUser = P("u");        /* All entries by this user if not NULL */
  const char *zType = PD("y","all"); /* Type of events.  All if NULL */
  const char *zAfter = P("a");       /* Events after this time */
  const char *zBefore = P("b");      /* Events before this time */
  const char *zCirca = P("c");       /* Events near this time */
  const char *zTagName = P("t");     /* Show events with this tag */
  const char *zSearch = P("s");      /* Search string */
  HQuery url;                        /* URL for various branch links */
  int tagid;                         /* Tag ID */
  int tmFlags;                       /* Timeline flags */

  /* To view the timeline, must have permission to read project data.
  */
  login_check_credentials();
  if( !g.okRead && !g.okRdTkt && !g.okRdWiki ){ login_needed(); return; }
  if( zTagName && g.okRead ){
    tagid = db_int(0, "SELECT tagid FROM tag WHERE tagname='sym-%q'", zTagName);
  }else{
    tagid = 0;
  }
  if( zType[0]=='a' ){
    tmFlags = TIMELINE_BRIEF | TIMELINE_GRAPH;
  }else{
    tmFlags = TIMELINE_GRAPH;
  }
  if( P("ng")!=0 || zSearch!=0 ){
    tmFlags &= ~TIMELINE_GRAPH;
  }

  style_header("Timeline");
  login_anonymous_available();
  timeline_temp_table();
  blob_zero(&sql);
  blob_zero(&desc);
  blob_append(&sql, "INSERT OR IGNORE INTO timeline ", -1);
  blob_append(&sql, timeline_query_for_www(), -1);
  if( (p_rid || d_rid) && g.okRead ){
    /* If p= or d= is present, ignore all other parameters other than n= */
    char *zUuid;
    int np, nd;

    if( p_rid && d_rid ){
      if( p_rid!=d_rid ) p_rid = d_rid;
      if( P("n")==0 ) nEntry = 10;
    }
    db_multi_exec(
       "CREATE TEMP TABLE IF NOT EXISTS ok(rid INTEGER PRIMARY KEY)"
    );
    zUuid = db_text("", "SELECT uuid FROM blob WHERE rid=%d",
                         p_rid ? p_rid : d_rid);
    blob_appendf(&sql, " AND event.objid IN ok");
    nd = 0;
    if( d_rid ){
      compute_descendants(d_rid, nEntry+1);
      nd = db_int(0, "SELECT count(*)-1 FROM ok");
      if( nd>=0 ){
        db_multi_exec("%s", blob_str(&sql));
        blob_appendf(&desc, "%d descendant%s", nd,(1==nd)?"":"s");
      }
      timeline_add_dividers(
        db_text("1","SELECT datetime(mtime,'localtime') FROM event"
                    " WHERE objid=%d", d_rid)
      );
      db_multi_exec("DELETE FROM ok");
    }
    if( p_rid ){
      compute_ancestors(p_rid, nEntry+1);
      np = db_int(0, "SELECT count(*)-1 FROM ok");
      if( np>0 ){
        if( nd>0 ) blob_appendf(&desc, " and ");
        blob_appendf(&desc, "%d ancestors", np);
        db_multi_exec("%s", blob_str(&sql));
      }
      if( d_rid==0 ){
        timeline_add_dividers(  
          db_text("1","SELECT datetime(mtime,'localtime') FROM event"
                      " WHERE objid=%d", p_rid)
        );
      }
    }
    if( g.okHistory ){
      blob_appendf(&desc, " of <a href='%s/info/%s'>[%.10s]</a>",
                   g.zBaseURL, zUuid, zUuid);
    }else{
      blob_appendf(&desc, " of check-in [%.10s]", zUuid);
    }
  }else{
    int n;
    const char *zEType = "event";
    char *zDate;
    char *zNEntry = mprintf("%d", nEntry);
    url_initialize(&url, "timeline");
    url_add_parameter(&url, "n", zNEntry);
    if( tagid>0 ){
      zType = "ci";
      url_add_parameter(&url, "t", zTagName);
      blob_appendf(&sql, " AND EXISTS (SELECT 1 FROM tagxref WHERE tagid=%d"
                                        " AND tagtype>0 AND rid=blob.rid)",
                   tagid);
    }
    if( (zType[0]=='w' && !g.okRdWiki)
     || (zType[0]=='t' && !g.okRdTkt)
     || (zType[0]=='c' && !g.okRead)
    ){
      zType = "all";
    }
    if( zType[0]=='a' ){
      if( !g.okRead || !g.okRdWiki || !g.okRdTkt ){
        char cSep = '(';
        blob_appendf(&sql, " AND event.type IN ");
        if( g.okRead ){
          blob_appendf(&sql, "%c'ci'", cSep);
          cSep = ',';
        }
        if( g.okRdWiki ){
          blob_appendf(&sql, "%c'w'", cSep);
          cSep = ',';
        }
        if( g.okRdTkt ){
          blob_appendf(&sql, "%c't'", cSep);
          cSep = ',';
        }
        blob_appendf(&sql, ")");
      }
    }else{ /* zType!="all" */
      blob_appendf(&sql, " AND event.type=%Q", zType);
      url_add_parameter(&url, "y", zType);
      if( zType[0]=='c' ){
        zEType = "checkin";
      }else if( zType[0]=='w' ){
        zEType = "wiki edit";
      }else if( zType[0]=='t' ){
        zEType = "ticket change";
      }
    }
    if( zUser ){
      blob_appendf(&sql, " AND event.user=%Q", zUser);
      url_add_parameter(&url, "u", zUser);
    }
    if ( zSearch ){
      blob_appendf(&sql,
        " AND (event.comment LIKE '%%%q%%' OR event.brief LIKE '%%%q%%')",
        zSearch, zSearch);
      url_add_parameter(&url, "s", zSearch);
    }
    if( zAfter ){
      while( isspace(zAfter[0]) ){ zAfter++; }
      if( zAfter[0] ){
        blob_appendf(&sql, 
           " AND event.mtime>=(SELECT julianday(%Q, 'utc'))"
           " ORDER BY event.mtime ASC", zAfter);
        url_add_parameter(&url, "a", zAfter);
        zBefore = 0;
      }else{
        zAfter = 0;
      }
    }else if( zBefore ){
      while( isspace(zBefore[0]) ){ zBefore++; }
      if( zBefore[0] ){
        blob_appendf(&sql, 
           " AND event.mtime<=(SELECT julianday(%Q, 'utc'))"
           " ORDER BY event.mtime DESC", zBefore);
        url_add_parameter(&url, "b", zBefore);
       }else{
        zBefore = 0;
      }
    }else if( zCirca ){
      while( isspace(zCirca[0]) ){ zCirca++; }
      if( zCirca[0] ){
        double rCirca = db_double(0.0, "SELECT julianday(%Q, 'utc')", zCirca);
        Blob sql2;
        blob_init(&sql2, blob_str(&sql), -1);
        blob_appendf(&sql2,
            " AND event.mtime<=%f ORDER BY event.mtime DESC LIMIT %d",
            rCirca, (nEntry+1)/2
        );
        db_multi_exec("%s", blob_str(&sql2));
        blob_reset(&sql2);
        blob_appendf(&sql,
            " AND event.mtime>=%f ORDER BY event.mtime ASC",
            rCirca
        );
        nEntry -= (nEntry+1)/2;
        timeline_add_dividers(zCirca);
        url_add_parameter(&url, "c", zCirca);
      }else{
        zCirca = 0;
      }
    }else{
      blob_appendf(&sql, " ORDER BY event.mtime DESC");
    }
    blob_appendf(&sql, " LIMIT %d", nEntry);
    db_multi_exec("%s", blob_str(&sql));

    n = db_int(0, "SELECT count(*) FROM timeline");
    if( n<nEntry && zAfter ){
      cgi_redirect(url_render(&url, "a", 0, "b", 0));
    }
    if( zAfter==0 && zBefore==0 && zCirca==0 ){
      blob_appendf(&desc, "%d most recent %ss", n, zEType);
    }else{
      blob_appendf(&desc, "%d %ss", n, zEType);
    }
    if( zUser ){
      blob_appendf(&desc, " by user %h", zUser);
      tmFlags |= TIMELINE_DISJOINT;
    }
    if( tagid>0 ){
      blob_appendf(&desc, " tagged with \"%h\"", zTagName);
      tmFlags |= TIMELINE_DISJOINT;
    }
    if( zAfter ){
      blob_appendf(&desc, " occurring on or after %h.<br>", zAfter);
    }else if( zBefore ){
      blob_appendf(&desc, " occurring on or before %h.<br>", zBefore);
    }else if( zCirca ){
      blob_appendf(&desc, " occurring around %h.<br>", zCirca);
    }
    if( zSearch ){
      blob_appendf(&desc, " matching \"%h\"", zSearch);
    }
    if( g.okHistory ){
      if( zAfter || n==nEntry ){
        zDate = db_text(0, "SELECT min(timestamp) FROM timeline");
        timeline_submenu(&url, "Older", "b", zDate, "a");
        free(zDate);
      }
      if( zBefore || (zAfter && n==nEntry) ){
        zDate = db_text(0, "SELECT max(timestamp) FROM timeline");
        timeline_submenu(&url, "Newer", "a", zDate, "b");
        free(zDate);
      }else if( tagid==0 ){
        if( zType[0]!='a' ){
          timeline_submenu(&url, "All Types", "y", "all", 0);
        }
        if( zType[0]!='w' && g.okRdWiki ){
          timeline_submenu(&url, "Wiki Only", "y", "w", 0);
        }
        if( zType[0]!='c' && g.okRead ){
          timeline_submenu(&url, "Checkins Only", "y", "ci", 0);
        }
        if( zType[0]!='t' && g.okRdTkt ){
          timeline_submenu(&url, "Tickets Only", "y", "t", 0);
        }
      }
      if( nEntry>20 ){
        timeline_submenu(&url, "20 Events", "n", "20", 0);
      }
      if( nEntry<200 ){
        timeline_submenu(&url, "200 Events", "n", "200", 0);
      }
    }
  }
  blob_zero(&sql);
  db_prepare(&q, "SELECT * FROM timeline ORDER BY timestamp DESC");
  cgi_printf("<h2>%b</h2>\n",&desc);
  blob_reset(&desc);
  www_print_timeline(&q, tmFlags, 0);
  db_finalize(&q);
  style_footer();
}

/*
** The input query q selects various records.  Print a human-readable
** summary of those records.
**
** Limit the number of entries printed to nLine.
** 
** The query should return these columns:
**
**    0.  rid
**    1.  uuid
**    2.  Date/Time
**    3.  Comment string and user
**    4.  Number of non-merge children
**    5.  Number of parents
*/
void print_timeline(Stmt *q, int mxLine){
  int nLine = 0;
  char zPrevDate[20];
  const char *zCurrentUuid=0;
  zPrevDate[0] = 0;

  if( g.localOpen ){
    int rid = db_lget_int("checkout", 0);
    zCurrentUuid = db_text(0, "SELECT uuid FROM blob WHERE rid=%d", rid);
  }

  while( db_step(q)==SQLITE_ROW && nLine<=mxLine ){
    int rid = db_column_int(q, 0);
    const char *zId = db_column_text(q, 1);
    const char *zDate = db_column_text(q, 2);
    const char *zCom = db_column_text(q, 3);
    int nChild = db_column_int(q, 4);
    int nParent = db_column_int(q, 5);
    char *zFree = 0;
    int n = 0;
    char zPrefix[80];
    char zUuid[UUID_SIZE+1];
    
    sprintf(zUuid, "%.10s", zId);
    if( memcmp(zDate, zPrevDate, 10) ){
      _ssfossil_printf("=== %.10s ===\n", zDate);
      memcpy(zPrevDate, zDate, 10);
      nLine++;
    }
    if( zCom==0 ) zCom = "";
    _ssfossil_printf("%.8s ", &zDate[11]);
    zPrefix[0] = 0;
    if( nParent>1 ){
      sqlite3_snprintf(sizeof(zPrefix), zPrefix, "*MERGE* ");
      n = strlen(zPrefix);
    }
    if( nChild>1 ){
      const char *zBrType;
      if( count_nonbranch_children(rid)>1 ){
        zBrType = "*FORK* ";
      }else{
        zBrType = "*BRANCH* ";
      }
      sqlite3_snprintf(sizeof(zPrefix)-n, &zPrefix[n], zBrType);
      n = strlen(zPrefix);
    }
    if( zCurrentUuid && strcmp(zCurrentUuid,zId)==0 ){
      sqlite3_snprintf(sizeof(zPrefix)-n, &zPrefix[n], "*CURRENT* ");
      n += strlen(zPrefix);
    }
    zFree = sqlite3_mprintf("[%.10s] %s%s", zUuid, zPrefix, zCom);
    nLine += comment_print(zFree, 9, 79);
    sqlite3_free(zFree);
  }
}

/*
** Return a pointer to a static string that forms the basis for
** a timeline query for display on a TTY.
*/
const char *timeline_query_for_tty(void){
  static const char zBaseSql[] = 
    "SELECT\n"
    "  blob.rid,\n"
    "  uuid,\n"
    "  datetime(event.mtime,'localtime'),\n"
    "  coalesce(ecomment,comment)\n"
    "    || ' (user: ' || coalesce(euser,user,'?')\n"
    "    || (SELECT case when length(x)>0 then ' tags: ' || x else '' end\n"
    "          FROM (SELECT group_concat(substr(tagname,5), ', ') AS x\n"
    "                  FROM tag, tagxref\n"
    "                 WHERE tagname GLOB 'sym-*' AND tag.tagid=tagxref.tagid\n"
    "                   AND tagxref.rid=blob.rid AND tagxref.tagtype>0))\n"
    "    || ')',\n"
    "  (SELECT count(*) FROM plink WHERE pid=blob.rid AND isprim),\n"
    "  (SELECT count(*) FROM plink WHERE cid=blob.rid)\n"
    "FROM event, blob\n"
    "WHERE blob.rid=event.objid\n"
  ;
  return zBaseSql;
}

/*
** Return true if the input string is a date in the ISO 8601 format:
** YYYY-MM-DD.
*/
static int isIsoDate(const char *z){
  return strlen(z)==10
      && z[4]=='-'
      && z[7]=='-'
      && isdigit(z[0])
      && isdigit(z[5]);
}

/*
** COMMAND: timeline
**
** Usage: %fossil timeline ?WHEN? ?BASELINE|DATETIME? ?-n N? ?-t TYPE?
**
** Print a summary of activity going backwards in date and time
** specified or from the current date and time if no arguments
** are given.  Show as many as N (default 20) check-ins.  The
** WHEN argument can be any unique abbreviation of one of these
** keywords:
**
**     before
**     after
**     descendants | children
**     ancestors | parents
**
** The BASELINE can be any unique prefix of 4 characters or more.
** The DATETIME should be in the ISO8601 format.  For
** examples: "2007-08-18 07:21:21".  You can also say "current"
** for the current version or "now" for the current time.
**
** The optional TYPE argument may any types supported by the /timeline
** page. For example:
**
**     w  = wiki commits only
**     ci = file commits only
**     t  = tickets only
*/
void timeline_cmd(void){
  Stmt q;
  int n, k;
  const char *zCount;
  const char *zType;
  char *zOrigin;
  char *zDate;
  Blob sql;
  int objid = 0;
  Blob uuid;
  int mode = 0 ;       /* 0:none  1: before  2:after  3:children  4:parents */
  db_find_and_open_repository(1);
  zCount = find_option("count","n",1);
  zType = find_option("type","t",1);
  if( zCount ){
    n = atoi(zCount);
  }else{
    n = 20;
  }
  if( g.argc>=4 ){
    k = strlen(g.argv[2]);
    if( strncmp(g.argv[2],"before",k)==0 ){
      mode = 1;
    }else if( strncmp(g.argv[2],"after",k)==0 && k>1 ){
      mode = 2;
    }else if( strncmp(g.argv[2],"descendants",k)==0 ){
      mode = 3;
    }else if( strncmp(g.argv[2],"children",k)==0 ){
      mode = 3;
    }else if( strncmp(g.argv[2],"ancestors",k)==0 && k>1 ){
      mode = 4;
    }else if( strncmp(g.argv[2],"parents",k)==0 ){
      mode = 4;
    }else if(!zType && !zCount){
      usage("?WHEN? ?BASELINE|DATETIME? ?-n|--count N? ?-t TYPE?");
    }
    if( '-' != *g.argv[3] ){
	zOrigin = g.argv[3];
    }else{
	zOrigin = "now";
    }
  }else if( g.argc==3 ){
    zOrigin = g.argv[2];
  }else{
    zOrigin = "now";
  }
  k = strlen(zOrigin);
  blob_zero(&uuid);
  blob_append(&uuid, zOrigin, -1);
  if( strcmp(zOrigin, "now")==0 ){
    if( mode==3 || mode==4 ){
      fossil_fatal("cannot compute descendants or ancestors of a date");
    }
    zDate = mprintf("(SELECT datetime('now'))");
  }else if( strncmp(zOrigin, "current", k)==0 ){
    if( !g.localOpen ){
      fossil_fatal("must be within a local checkout to use 'current'");
    }
    objid = db_lget_int("checkout",0);
    zDate = mprintf("(SELECT mtime FROM plink WHERE cid=%d)", objid);
  }else if( name_to_uuid(&uuid, 0)==0 ){
    objid = db_int(0, "SELECT rid FROM blob WHERE uuid=%B", &uuid);
    zDate = mprintf("(SELECT mtime FROM plink WHERE cid=%d)", objid);
  }else{
    const char *zShift = "";
    if( mode==3 || mode==4 ){
      fossil_fatal("cannot compute descendants or ancestors of a date");
    }
    if( mode==0 ){
      if( isIsoDate(zOrigin) ) zShift = ",'+1 day'";
    }
    zDate = mprintf("(SELECT julianday(%Q%s, 'utc'))", zOrigin, zShift);
  }
  if( mode==0 ) mode = 1;
  blob_zero(&sql);
  blob_append(&sql, timeline_query_for_tty(), -1);
  blob_appendf(&sql, "  AND event.mtime %s %s",
     (mode==1 || mode==4) ? "<=" : ">=",
     zDate
  );

  if( mode==3 || mode==4 ){
    db_multi_exec("CREATE TEMP TABLE ok(rid INTEGER PRIMARY KEY)");
    if( mode==3 ){
      compute_descendants(objid, n);
    }else{
      compute_ancestors(objid, n);
    }
    blob_appendf(&sql, " AND blob.rid IN ok");
  }
  if( zType && (zType[0]!='a') ){
    blob_appendf(&sql, " AND event.type=%Q ", zType);
  }

  blob_appendf(&sql, " ORDER BY event.mtime DESC");
  db_prepare(&q, blob_str(&sql));
  blob_reset(&sql);
  print_timeline(&q, n);
  db_finalize(&q);
}

/*
** This is a version of the "localtime()" function from the standard
** C library.  It converts a unix timestamp (seconds since 1970) into
** a broken-out local time structure.
**
** This modified version of localtime() works like the library localtime()
** by default.  Except if the timeline-utc property is set, this routine
** uses gmttime() instead.  Thus by setting the timeline-utc property, we
** can get all localtimes to be displayed at UTC time.
*/
struct tm *fossil_localtime(const time_t *clock){
  if( g.fTimeFormat==0 ){
    if( db_get_int("timeline-utc", 1) ){
      g.fTimeFormat = 1;
    }else{
      g.fTimeFormat = 2;
    }
  }
  if( g.fTimeFormat==1 ){
    return gmtime(clock);
  }else{
    return localtime(clock);
  }
}
