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
** This file contains string constants that implement the database schema.
*/
#include "config.h"
#include "schema.h"

/*
** The database schema for the ~/.fossil configuration database.
*/
const char zConfigSchema[] = 



"CREATE TABLE global_config(\n"
"  name TEXT PRIMARY KEY,\n"
"  value TEXT\n"
");\n"
;

#if INTERFACE
/*
** The content tables have a content version number which rarely
** changes.  The aux tables have an arbitrary version number (typically
** a date) which can change frequently.  When the content schema changes,
** we have to execute special procedures to update the schema.  When
** the aux schema changes, all we need to do is rebuild the database.
*/
#define CONTENT_SCHEMA  "1"
#define AUX_SCHEMA      "2006-12-23"

#endif /* INTERFACE */


/*
** The schema for a repository database.  
**
** Schema1[] contains parts of the schema that are fixed and unchanging
** across versions.  Schema2[] contains parts of the schema that can
** change from one version to the next.  The information in Schema2[]
** can be reconstructed from the information in Schema1[].
*/
const char zRepositorySchema1[] = 









"CREATE TABLE blob(\n"
"  rid INTEGER PRIMARY KEY,\n"
"  rcvid INTEGER,\n"
"  size INTEGER,\n"
"  uuid TEXT UNIQUE NOT NULL,\n"
"  content BLOB,\n"
"  CHECK( length(uuid)==40 AND rid>0 )\n"
");\n"
"CREATE TABLE delta(\n"
"  rid INTEGER PRIMARY KEY,\n"
"  srcid INTEGER NOT NULL REFERENCES blob\n"
");\n"
"CREATE INDEX delta_i1 ON delta(srcid);\n"
"\n"



"CREATE TABLE rcvfrom(\n"
"  rcvid INTEGER PRIMARY KEY,\n"
"  uid INTEGER REFERENCES user,\n"
"  mtime DATETIME,\n"
"  nonce TEXT UNIQUE,\n"
"  ipaddr TEXT\n"
");\n"
"\n"









"CREATE TABLE user(\n"
"  uid INTEGER PRIMARY KEY,\n"
"  login TEXT,\n"
"  pw TEXT,\n"
"  cap TEXT,\n"
"  cookie TEXT,\n"
"  ipaddr TEXT,\n"
"  cexpire DATETIME,\n"
"  info TEXT,\n"
"  photo BLOB\n"
");\n"
"\n"



"CREATE TABLE config(\n"
"  name TEXT PRIMARY KEY NOT NULL,\n"
"  value CLOB,\n"
"  CHECK( typeof(name)='text' AND length(name)>=1 )\n"
");\n"
"\n"










"CREATE TABLE shun(uuid UNIQUE);\n"
"\n"




"CREATE TABLE private(rid INTEGER PRIMARY KEY);\n"
"\n"



"CREATE TABLE reportfmt(\n"
"   rn integer primary key,\n"
"   owner text,\n"
"   title text,\n"
"   cols text,\n"
"   sqlcode text\n"
");\n"
"INSERT INTO reportfmt(title,cols,sqlcode) VALUES('All Tickets','#ffffff Key:\n"
"#f2dcdc Active\n"
"#e8e8e8 Review\n"
"#cfe8bd Fixed\n"
"#bde5d6 Tested\n"
"#cacae5 Deferred\n"
"#c8c8c8 Closed','SELECT\n"
"  CASE WHEN status IN (''Open'',''Verified'') THEN ''#f2dcdc''\n"
"       WHEN status=''Review'' THEN ''#e8e8e8''\n"
"       WHEN status=''Fixed'' THEN ''#cfe8bd''\n"
"       WHEN status=''Tested'' THEN ''#bde5d6''\n"
"       WHEN status=''Deferred'' THEN ''#cacae5''\n"
"       ELSE ''#c8c8c8'' END AS ''bgcolor'',\n"
"  substr(tkt_uuid,1,10) AS ''#'',\n"
"  datetime(tkt_mtime) AS ''mtime'',\n"
"  type,\n"
"  status,\n"
"  subsystem,\n"
"  title\n"
"FROM ticket');\n"
"\n"








"CREATE TABLE concealed(\n"
"  hash TEXT PRIMARY KEY,\n"
"  content TEXT\n"
");\n"
;

const char zRepositorySchema2[] =


"CREATE TABLE filename(\n"
"  fnid INTEGER PRIMARY KEY,\n"
"  name TEXT UNIQUE\n"
");\n"
"\n"






"CREATE TABLE mlink(\n"
"  mid INTEGER REFERENCES blob,\n"
"  pid INTEGER REFERENCES blob,\n"
"  fid INTEGER REFERENCES blob,\n"
"  fnid INTEGER REFERENCES filename,\n"
"  pfnid INTEGER REFERENCES filename\n"
");\n"
"CREATE INDEX mlink_i1 ON mlink(mid);\n"
"CREATE INDEX mlink_i2 ON mlink(fnid);\n"
"CREATE INDEX mlink_i3 ON mlink(fid);\n"
"CREATE INDEX mlink_i4 ON mlink(pid);\n"
"\n"


"CREATE TABLE plink(\n"
"  pid INTEGER REFERENCES blob,\n"
"  cid INTEGER REFERENCES blob,\n"
"  isprim BOOLEAN,\n"
"  mtime DATETIME,\n"
"  UNIQUE(pid, cid)\n"
");\n"
"CREATE INDEX plink_i2 ON plink(cid);\n"
"\n"


"CREATE TABLE event(\n"
"  type TEXT,\n"
"  mtime DATETIME,\n"
"  objid INTEGER PRIMARY KEY,\n"
"  tagid INTEGER,\n"
"  uid INTEGER REFERENCES user,\n"
"  bgcolor TEXT,\n"
"  euser TEXT,\n"
"  user TEXT,\n"
"  ecomment TEXT,\n"
"  comment TEXT,\n"
"  brief TEXT\n"
");\n"
"CREATE INDEX event_i1 ON event(mtime);\n"
"\n"



"CREATE TABLE phantom(\n"
"  rid INTEGER PRIMARY KEY\n"
");\n"
"\n"









"CREATE TABLE unclustered(\n"
"  rid INTEGER PRIMARY KEY\n"
");\n"
"\n"




"CREATE TABLE unsent(\n"
"  rid INTEGER PRIMARY KEY\n"
");\n"
"\n"









"CREATE TABLE tag(\n"
"  tagid INTEGER PRIMARY KEY,\n"
"  tagname TEXT UNIQUE\n"
");\n"
"INSERT INTO tag VALUES(1, 'bgcolor');\n"
"INSERT INTO tag VALUES(2, 'comment');\n"
"INSERT INTO tag VALUES(3, 'user');\n"
"INSERT INTO tag VALUES(4, 'date');\n"
"INSERT INTO tag VALUES(5, 'hidden');\n"
"INSERT INTO tag VALUES(6, 'private');\n"
"INSERT INTO tag VALUES(7, 'cluster');\n"
"INSERT INTO tag VALUES(8, 'branch');\n"
"INSERT INTO tag VALUES(9, 'closed');\n"
"\n"





"CREATE TABLE tagxref(\n"
"  tagid INTEGER REFERENCES tag,\n"
"  tagtype INTEGER,\n"
"  srcid INTEGER REFERENCES blob,\n"
"  origid INTEGER REFERENCES blob,\n"
"  value TEXT,\n"
"  mtime TIMESTAMP,\n"
"  rid INTEGER REFERENCE blob,\n"
"  UNIQUE(rid, tagid)\n"
");\n"
"CREATE INDEX tagxref_i1 ON tagxref(tagid, mtime);\n"
"\n"





"CREATE TABLE backlink(\n"
"  target TEXT,\n"
"  srctype INT,\n"
"  srcid INT,\n"
"  mtime TIMESTAMP,\n"
"  UNIQUE(target, srctype, srcid)\n"
");\n"
"CREATE INDEX backlink_src ON backlink(srcid, srctype);\n"
"\n"





"CREATE TABLE ticket(\n"

"  tkt_id INTEGER PRIMARY KEY,\n"
"  tkt_uuid TEXT UNIQUE,\n"
"  tkt_mtime DATE,\n"

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
** Predefined tagid values
*/
#if INTERFACE
# define TAG_BGCOLOR    1     /* Set the background color for display */
# define TAG_COMMENT    2     /* The check-in comment */
# define TAG_USER       3     /* User who made a checking */
# define TAG_DATE       4     /* The date of a check-in */
# define TAG_HIDDEN     5     /* Do not display or sync */
# define TAG_PRIVATE    6     /* Display but do not sync */
# define TAG_CLUSTER    7     /* A cluster */
# define TAG_BRANCH     8     /* Value is name of the current branch */
# define TAG_CLOSED     9     /* Do not display this check-in as a leaf */
#endif
#if EXPORT_INTERFACE
# define MAX_INT_TAG    9     /* The largest pre-assigned tag id */
#endif

/*
** The schema for the locate FOSSIL database file found at the root
** of very check-out.  This database contains the complete state of
** the checkout.
*/
const char zLocalSchema[] =










"CREATE TABLE vvar(\n"
"  name TEXT PRIMARY KEY NOT NULL,\n"
"  value CLOB,\n"
"  CHECK( typeof(name)='text' AND length(name)>=1 )\n"
");\n"
"\n"















"CREATE TABLE vfile(\n"
"  id INTEGER PRIMARY KEY,\n"
"  vid INTEGER REFERENCES blob,\n"
"  chnged INT DEFAULT 0,\n"
"  deleted BOOLEAN DEFAULT 0,\n"
"  isexe BOOLEAN,\n"
"  rid INTEGER,\n"
"  mrid INTEGER,\n"
"  mtime INTEGER,\n"
"  pathname TEXT,\n"
"  origname TEXT,\n"
"  UNIQUE(pathname,vid)\n"
");\n"
"\n"





"\n"
"CREATE TABLE vmerge(\n"
"  id INTEGER REFERENCES vfile,\n"
"  merge INTEGER,\n"
"  UNIQUE(id, merge)\n"
");\n"
"\n"
;
