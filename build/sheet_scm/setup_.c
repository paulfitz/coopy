/*
** Copyright (c) 2007 D. Richard Hipp
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
** Implementation of the Setup page
*/
#include <assert.h>
#include "config.h"
#include "setup.h"


/*
** Output a single entry for a menu generated using an HTML table.
** If zLink is not NULL or an empty string, then it is the page that
** the menu entry will hyperlink to.  If zLink is NULL or "", then
** the menu entry has no hyperlink - it is disabled.
*/
void setup_menu_entry(
  const char *zTitle,
  const char *zLink,
  const char *zDesc
){
  cgi_printf("<tr><td valign=\"top\" align=\"right\">\n");
  if( zLink && zLink[0] ){
    cgi_printf("<a href=\"%s\">%h</a>\n",zLink,zTitle);
  }else{
    cgi_printf("%h\n",zTitle);
  }
  cgi_printf("</td><td valign=\"top\">%h</td></tr>\n",zDesc);
}

/*
** WEBPAGE: /setup
*/
void setup_page(void){
  login_check_credentials();
  if( !g.okSetup ){
    login_needed();
  }

  style_header("Server Administration");
  cgi_printf("<table border=\"0\" cellspacing=\"20\">\n");
  setup_menu_entry("Users", "setup_ulist",
    "Grant privileges to individual users.");
  setup_menu_entry("Access", "setup_access",
    "Control access settings.");
  setup_menu_entry("Configuration", "setup_config",
    "Configure the WWW components of the repository");
  setup_menu_entry("Behavior", "setup_behavior",
    "Configure the SCM behavior of the repository");
  setup_menu_entry("Timeline", "setup_timeline",
    "Timeline display preferences");
  setup_menu_entry("Tickets", "tktsetup",
    "Configure the trouble-ticketing system for this repository");
  setup_menu_entry("Skins", "setup_skin",
    "Select from a menu of prepackaged \"skins\" for the web interface");
  setup_menu_entry("CSS", "setup_editcss",
    "Edit the Cascading Style Sheet used by all pages of this repository");
  setup_menu_entry("Header", "setup_header",
    "Edit HTML text inserted at the top of every page");
  setup_menu_entry("Footer", "setup_footer",
    "Edit HTML text inserted at the bottom of every page");
  setup_menu_entry("Logo", "setup_logo",
    "Change the logo image for the server");
  setup_menu_entry("Shunned", "shun",
    "Show artifacts that are shunned by this repository");
  setup_menu_entry("Log", "rcvfromlist",
    "A record of received artifacts and their sources");
  setup_menu_entry("Stats", "stat",
    "Display repository statistics");
  setup_menu_entry("Sync now", "setup_sync",
    "Sync this repository with the 'remote-url' it was set up with");
  cgi_printf("</table>\n");

  style_footer();
}

/*
** WEBPAGE: setup_ulist
**
** Show a list of users.  Clicking on any user jumps to the edit
** screen for that user.
*/
void setup_ulist(void){
  Stmt s;

  login_check_credentials();
  if( !g.okAdmin ){
    login_needed();
    return;
  }

  style_submenu_element("Add", "Add User", "setup_uedit");
  style_header("User List");
  cgi_printf("<table border=\"0\" cellpadding=\"0\" cellspacing=\"25\">\n"
         "<tr><td valign=\"top\">\n"
         "<b>Users:</b>\n"
         "<table border=\"1\" cellpadding=\"10\"><tr><td>\n"
         "<table cellspacing=0 cellpadding=0 border=0>\n"
         "<tr>\n"
         "  <th align=\"right\">User&nbsp;ID</th><td width=\"20\">&nbsp;</td>\n"
         "  <th>Capabilities</th><td width=\"15\">&nbsp;</td>\n"
         "  <th>Contact&nbsp;Info</th>\n"
         "</tr>\n");
  db_prepare(&s, "SELECT uid, login, cap, info FROM user ORDER BY login");
  while( db_step(&s)==SQLITE_ROW ){
    const char *zCap = db_column_text(&s, 2);
    if( strstr(zCap, "s") ) zCap = "s";
    cgi_printf("<tr>\n"
           "<td align=\"right\">\n");
    if( g.okAdmin && (zCap[0]!='s' || g.okSetup) ){
      cgi_printf("<a href=\"setup_uedit?id=%d\">\n",db_column_int(&s,0));
    }
    cgi_printf("<nobr>%h</nobr>\n",db_column_text(&s,1));
    if( g.okAdmin ){
      cgi_printf("</a>\n");
    }
    cgi_printf("</td><td>&nbsp;&nbsp;&nbsp;</td>\n"
           "<td align=\"center\">%s</td>\n"
           "<td>&nbsp;&nbsp;&nbsp;</td>\n"
           "<td align=\"left\">%s</td>\n"
           "</tr>\n",zCap,db_column_text(&s,3));
  }
  cgi_printf("</table></td></tr></table>\n"
         "<td valign=\"top\">\n"
         "<b>Notes:</b>\n"
         "<ol>\n"
         "<li><p>The permission flags are as follows:</p>\n"
         "<table>\n"
            "<tr><td valign=\"top\"><b>a</b></td>\n"
            "  <td><i>Admin:</i> Create and delete users</td></tr>\n"
            "<tr><td valign=\"top\"><b>c</b></td>\n"
            "  <td><i>Append-Tkt:</i> Append to tickets</td></tr>\n"
            "<tr><td valign=\"top\"><b>d</b></td>\n"
            "  <td><i>Delete:</i> Delete wiki and tickets</td></tr>\n"
            "<tr><td valign=\"top\"><b>e</b></td>\n"
            "  <td><i>Email:</i> View sensitive data such as EMail addresses</td></tr>\n"
            "<tr><td valign=\"top\"><b>f</b></td>\n"
            "  <td><i>New-Wiki:</i> Create new wiki pages</td></tr>\n"
            "<tr><td valign=\"top\"><b>g</b></td>\n"
            "  <td><i>Clone:</i> Clone the repository</td></tr>\n"
            "<tr><td valign=\"top\"><b>h</b></td>\n"
            "  <td><i>Hyperlinks:</i> Show hyperlinks to detailed\n"
            "  repository history</td></tr>\n"
            "<tr><td valign=\"top\"><b>i</b></td>\n"
            "  <td><i>Check-In:</i> Commit new versions in the repository</td></tr>\n"
            "<tr><td valign=\"top\"><b>j</b></td>\n"
            "  <td><i>Read-Wiki:</i> View wiki pages</td></tr>\n"
            "<tr><td valign=\"top\"><b>k</b></td>\n"
            "  <td><i>Write-Wiki:</i> Edit wiki pages</td></tr>\n"
            "<tr><td valign=\"top\"><b>m</b></td>\n"
            "  <td><i>Append-Wiki:</i> Append to wiki pages</td></tr>\n"
            "<tr><td valign=\"top\"><b>n</b></td>\n"
            "  <td><i>New-Tkt:</i> Create new tickets</td></tr>\n"
            "<tr><td valign=\"top\"><b>o</b></td>\n"
            "  <td><i>Check-Out:</i> Check out versions</td></tr>\n"
            "<tr><td valign=\"top\"><b>p</b></td>\n"
            "  <td><i>Password:</i> Change your own password</td></tr>\n"
            "<tr><td valign=\"top\"><b>r</b></td>\n"
            "  <td><i>Read-Tkt:</i> View tickets</td></tr>\n"
            "<tr><td valign=\"top\"><b>s</b></td>\n"
            "  <td><i>Setup/Super-user:</i> Setup and configure this website</td></tr>\n"
            "<tr><td valign=\"top\"><b>t</b></td>\n"
            "  <td><i>Tkt-Report:</i> Create new bug summary reports</td></tr>\n"
            "<tr><td valign=\"top\"><b>u</b></td>\n"
            "  <td><i>Reader:</i> Inherit privileges of \n"
            "  user <tt>reader</tt></td></tr>\n"
            "<tr><td valign=\"top\"><b>v</b></td>\n"
            "  <td><i>Developer:</i> Inherit privileges of \n"
            "  user <tt>developer</tt></td></tr>\n"
            "<tr><td valign=\"top\"><b>w</b></td>\n"
            "  <td><i>Write-Tkt:</i> Edit tickets</td></tr>\n"
            "<tr><td valign=\"top\"><b>z</b></td>\n"
            "  <td><i>Zip download:</i> Download a baseline via the\n"
            "  <tt>/zip</tt> URL even without check<b>o</b>ut\n"
            "   and <b>h</b>istory permissions</td></tr>\n"
         "</table>\n"
         "</li>\n"
         "\n"
         "<li><p>\n"
         "Every user, logged in or not, inherits the privileges of <b>nobody</b>.\n"
         "</p></li>\n"
         "\n"
         "<li><p>\n"
         "Any human can login as <b>anonymous</b> since the password is\n"
         "clearly displayed on the login page for them to type.  The purpose\n"
         "of requiring anonymous to log in is to prevent access by spiders.\n"
         "Every logged-in user inherits the combined privileges of\n"
         "<b>anonymous</b> and\n"
         "<b>nobody</b>.\n"
         "</p></li>\n"
         "\n"
         "<li><p>\n"
         "Users with privilege <b>v</b> inherit the combined privileges of\n"
         "<b>developer</b>, <b>anonymous</b>, and <b>nobody</b>.\n"
         "</p></li>\n"
         "\n"
         "</ol>\n"
         "</td></tr></table>\n");
  style_footer();
}

/*
** Return true if zPw is a valid password string.  A valid
** password string is:
**
**  (1)  A zero-length string, or
**  (2)  a string that contains a character other than '*'.
*/
static int isValidPwString(const char *zPw){
  if( zPw==0 ) return 0;
  if( zPw[0]==0 ) return 1;
  while( zPw[0]=='*' ){ zPw++; }
  return zPw[0]!=0;
}

/*
** WEBPAGE: /setup_uedit
*/
void user_edit(void){
  const char *zId, *zLogin, *zInfo, *zCap, *zPw;
  char *oaa, *oas, *oar, *oaw, *oan, *oai, *oaj, *oao, *oap;
  char *oak, *oad, *oac, *oaf, *oam, *oah, *oag, *oae;
  char *oat, *oau, *oav, *oaz;
  const char *inherit[128];
  int doWrite;
  int uid;
  int higherUser = 0;  /* True if user being edited is SETUP and the */
                       /* user doing the editing is ADMIN.  Disallow editing */

  /* Must have ADMIN privleges to access this page
  */
  login_check_credentials();
  if( !g.okAdmin ){ login_needed(); return; }

  /* Check to see if an ADMIN user is trying to edit a SETUP account.
  ** Don't allow that.
  */
  zId = PD("id", "0");
  uid = atoi(zId);
  if( zId && !g.okSetup && uid>0 ){
    char *zOldCaps;
    zOldCaps = db_text(0, "SELECT cap FROM user WHERE uid=%d",uid);
    higherUser = zOldCaps && strchr(zOldCaps,'s');
  }

  if( P("can") ){
    cgi_redirect("setup_ulist");
    return;
  }

  /* If we have all the necessary information, write the new or
  ** modified user record.  After writing the user record, redirect
  ** to the page that displays a list of users.
  */
  doWrite = cgi_all("login","info","pw") && !higherUser;
  if( doWrite ){
    char zCap[50];
    int i = 0;
    int aa = P("aa")!=0;
    int ad = P("ad")!=0;
    int ae = P("ae")!=0;
    int ai = P("ai")!=0;
    int aj = P("aj")!=0;
    int ak = P("ak")!=0;
    int an = P("an")!=0;
    int ao = P("ao")!=0;
    int ap = P("ap")!=0;
    int ar = P("ar")!=0;
    int as = g.okSetup && P("as")!=0;
    int aw = P("aw")!=0;
    int ac = P("ac")!=0;
    int af = P("af")!=0;
    int am = P("am")!=0;
    int ah = P("ah")!=0;
    int ag = P("ag")!=0;
    int at = P("at")!=0;
    int au = P("au")!=0;
    int av = P("av")!=0;
    int az = P("az")!=0;
    if( aa ){ zCap[i++] = 'a'; }
    if( ac ){ zCap[i++] = 'c'; }
    if( ad ){ zCap[i++] = 'd'; }
    if( ae ){ zCap[i++] = 'e'; }
    if( af ){ zCap[i++] = 'f'; }
    if( ah ){ zCap[i++] = 'h'; }
    if( ag ){ zCap[i++] = 'g'; }
    if( ai ){ zCap[i++] = 'i'; }
    if( aj ){ zCap[i++] = 'j'; }
    if( ak ){ zCap[i++] = 'k'; }
    if( am ){ zCap[i++] = 'm'; }
    if( an ){ zCap[i++] = 'n'; }
    if( ao ){ zCap[i++] = 'o'; }
    if( ap ){ zCap[i++] = 'p'; }
    if( ar ){ zCap[i++] = 'r'; }
    if( as ){ zCap[i++] = 's'; }
    if( at ){ zCap[i++] = 't'; }
    if( au ){ zCap[i++] = 'u'; }
    if( av ){ zCap[i++] = 'v'; }
    if( aw ){ zCap[i++] = 'w'; }
    if( az ){ zCap[i++] = 'z'; }

    zCap[i] = 0;
    zPw = P("pw");
    zLogin = P("login");
    if( isValidPwString(zPw) ){
      zPw = sha1_shared_secret(zPw, zLogin);
    }else{
      zPw = db_text(0, "SELECT pw FROM user WHERE uid=%d", uid);
    }
    if( uid>0 &&
        db_exists("SELECT 1 FROM user WHERE login=%Q AND uid!=%d", zLogin, uid)
    ){
      style_header("User Creation Error");
      cgi_printf("<font color=\"red\">Login \"%h\" is already used by a different\n"
             "user.</font>\n"
             "\n"
             "<p><a href=\"setup_uedit?id=%d\">[Bummer]</a></p>\n",zLogin,uid);
      style_footer();
      return;
    }
    login_verify_csrf_secret();
    db_multi_exec(
       "REPLACE INTO user(uid,login,info,pw,cap) "
       "VALUES(nullif(%d,0),%Q,%Q,%Q,'%s')",
      uid, P("login"), P("info"), zPw, zCap
    );
    cgi_redirect("setup_ulist");
    return;
  }

  /* Load the existing information about the user, if any
  */
  zLogin = "";
  zInfo = "";
  zCap = "";
  zPw = "";
  oaa = oac = oad = oae = oaf = oag = oah = oai = oaj = oak = oam =
        oan = oao = oap = oar = oas = oat = oau = oav = oaw = oaz = "";
  if( uid ){
    zLogin = db_text("", "SELECT login FROM user WHERE uid=%d", uid);
    zInfo = db_text("", "SELECT info FROM user WHERE uid=%d", uid);
    zCap = db_text("", "SELECT cap FROM user WHERE uid=%d", uid);
    zPw = db_text("", "SELECT pw FROM user WHERE uid=%d", uid);
    if( strchr(zCap, 'a') ) oaa = " checked";
    if( strchr(zCap, 'c') ) oac = " checked";
    if( strchr(zCap, 'd') ) oad = " checked";
    if( strchr(zCap, 'e') ) oae = " checked";
    if( strchr(zCap, 'f') ) oaf = " checked";
    if( strchr(zCap, 'g') ) oag = " checked";
    if( strchr(zCap, 'h') ) oah = " checked";
    if( strchr(zCap, 'i') ) oai = " checked";
    if( strchr(zCap, 'j') ) oaj = " checked";
    if( strchr(zCap, 'k') ) oak = " checked";
    if( strchr(zCap, 'm') ) oam = " checked";
    if( strchr(zCap, 'n') ) oan = " checked";
    if( strchr(zCap, 'o') ) oao = " checked";
    if( strchr(zCap, 'p') ) oap = " checked";
    if( strchr(zCap, 'r') ) oar = " checked";
    if( strchr(zCap, 's') ) oas = " checked";
    if( strchr(zCap, 't') ) oat = " checked";
    if( strchr(zCap, 'u') ) oau = " checked";
    if( strchr(zCap, 'v') ) oav = " checked";
    if( strchr(zCap, 'w') ) oaw = " checked";
    if( strchr(zCap, 'z') ) oaz = " checked";
  }

  /* figure out inherited permissions */
  memset(inherit, 0, sizeof(inherit));
  if( strcmp(zLogin, "developer") ){
    char *z1, *z2;
    z1 = z2 = db_text(0,"SELECT cap FROM user WHERE login='developer'");
    while( z1 && *z1 ){
      inherit[0x7f & *(z1++)] = "<font color=\"red\">&bull;</font>";
    }
    free(z2);
  }
  if( strcmp(zLogin, "reader") ){
    char *z1, *z2;
    z1 = z2 = db_text(0,"SELECT cap FROM user WHERE login='reader'");
    while( z1 && *z1 ){
      inherit[0x7f & *(z1++)] = "<font color=\"black\">&bull;</font>";
    }
    free(z2);
  }
  if( strcmp(zLogin, "anonymous") ){
    char *z1, *z2;
    z1 = z2 = db_text(0,"SELECT cap FROM user WHERE login='anonymous'");
    while( z1 && *z1 ){
      inherit[0x7f & *(z1++)] = "<font color=\"blue\">&bull;</font>";
    }
    free(z2);
  }
  if( strcmp(zLogin, "nobody") ){
    char *z1, *z2;
    z1 = z2 = db_text(0,"SELECT cap FROM user WHERE login='nobody'");
    while( z1 && *z1 ){
      inherit[0x7f & *(z1++)] = "<font color=\"green\">&bull;</font>";
    }
    free(z2);
  }

  /* Begin generating the page
  */
  style_submenu_element("Cancel", "Cancel", "setup_ulist");
  if( uid ){
    style_header(mprintf("Edit User %h", zLogin));
  }else{
    style_header("Add A New User");
  }
  cgi_printf("<table align=\"left\" hspace=\"20\" vspace=\"10\"><tr><td>\n"
         "<form action=\"%s\" method=\"POST\">\n",g.zPath);
  login_insert_csrf_secret();
  cgi_printf("<table>\n"
         "<tr>\n"
         "  <td align=\"right\"><nobr>User ID:</nobr></td>\n");
  if( uid ){
    cgi_printf("  <td>%d <input type=\"hidden\" name=\"id\" value=\"%d\"></td>\n",uid,uid);
  }else{
    cgi_printf("  <td>(new user)<input type=\"hidden\" name=\"id\" value=0></td>\n");
  }
  cgi_printf("</tr>\n"
         "<tr>\n"
         "  <td align=\"right\"><nobr>Login:</nobr></td>\n"
         "  <td><input type=\"text\" name=\"login\" value=\"%h\"></td>\n"
         "</tr>\n"
         "<tr>\n"
         "  <td align=\"right\"><nobr>Contact&nbsp;Info:</nobr></td>\n"
         "  <td><input type=\"text\" name=\"info\" size=40 value=\"%h\"></td>\n"
         "</tr>\n"
         "<tr>\n"
         "  <td align=\"right\" valign=\"top\">Capabilities:</td>\n"
         "  <td>\n",zLogin,zInfo);
#define B(x) inherit[x]
  if( g.okSetup ){
    cgi_printf("   <input type=\"checkbox\" name=\"as\"%s/>%sSetup<br>\n",oas,B('s'));
  }
  cgi_printf("   <input type=\"checkbox\" name=\"aa\"%s/>%sAdmin<br>\n"
         "   <input type=\"checkbox\" name=\"ad\"%s/>%sDelete<br>\n"
         "   <input type=\"checkbox\" name=\"ae\"%s/>%sEmail<br>\n"
         "   <input type=\"checkbox\" name=\"ap\"%s/>%sPassword<br>\n"
         "   <input type=\"checkbox\" name=\"ai\"%s/>%sCheck-In<br>\n"
         "   <input type=\"checkbox\" name=\"ao\"%s/>%sCheck-Out<br>\n"
         "   <input type=\"checkbox\" name=\"ah\"%s/>%sHistory<br>\n"
         "   <input type=\"checkbox\" name=\"au\"%s/>%sReader<br>\n"
         "   <input type=\"checkbox\" name=\"av\"%s/>%sDeveloper<br>\n"
         "   <input type=\"checkbox\" name=\"ag\"%s/>%sClone<br>\n"
         "   <input type=\"checkbox\" name=\"aj\"%s/>%sRead Wiki<br>\n"
         "   <input type=\"checkbox\" name=\"af\"%s/>%sNew Wiki<br>\n"
         "   <input type=\"checkbox\" name=\"am\"%s/>%sAppend Wiki<br>\n"
         "   <input type=\"checkbox\" name=\"ak\"%s/>%sWrite Wiki<br>\n"
         "   <input type=\"checkbox\" name=\"ar\"%s/>%sRead Tkt<br>\n"
         "   <input type=\"checkbox\" name=\"an\"%s/>%sNew Tkt<br>\n"
         "   <input type=\"checkbox\" name=\"ac\"%s/>%sAppend Tkt<br>\n"
         "   <input type=\"checkbox\" name=\"aw\"%s/>%sWrite Tkt<br>\n"
         "   <input type=\"checkbox\" name=\"at\"%s/>%sTkt Report<br>\n"
         "   <input type=\"checkbox\" name=\"az\"%s/>%sDownload Zip\n"
         "  </td>\n"
         "</tr>\n"
         "<tr>\n"
         "  <td align=\"right\">Password:</td>\n",oaa,B('a'),oad,B('d'),oae,B('e'),oap,B('p'),oai,B('i'),oao,B('o'),oah,B('h'),oau,B('u'),oav,B('v'),oag,B('g'),oaj,B('j'),oaf,B('f'),oam,B('m'),oak,B('k'),oar,B('r'),oan,B('n'),oac,B('c'),oaw,B('w'),oat,B('t'),oaz,B('z'));
  if( zPw[0] ){
    /* Obscure the password for all users */
    cgi_printf("  <td><input type=\"password\" name=\"pw\" value=\"**********\"></td>\n");
  }else{
    /* Show an empty password as an empty input field */
    cgi_printf("  <td><input type=\"password\" name=\"pw\" value=\"\"></td>\n");
  }
  cgi_printf("</tr>\n");
  if( !higherUser ){
    cgi_printf("<tr>\n"
           "  <td>&nbsp</td>\n"
           "  <td><input type=\"submit\" name=\"submit\" value=\"Apply Changes\">\n"
           "</tr>\n");
  }
  cgi_printf("</table></td></tr></table>\n"
         "<h2>Privileges And Capabilities:</h2>\n"
         "<ul>\n");
  if( higherUser ){
    cgi_printf("<li><p><font color=\"blue\"><b>\n"
           "User %h has Setup privileges and you only have Admin privileges\n"
           "so you are not permitted to make changes to %h.\n"
           "</b></font></p></li>\n"
           "\n",zLogin,zLogin);
  }
  cgi_printf("<li><p>\n"
         "The <b>Setup</b> user can make arbitrary configuration changes.\n"
         "An <b>Admin</b> user can add other users and change user privileges\n"
         "and reset user passwords.  Both automatically get all other privileges\n"
         "listed below.  Use these two settings with discretion.\n"
         "</p></li>\n"
         "\n"
         "<li><p>\n"
         "The \"<font color=\"green\"><big>&bull;</big></font>\" mark indicates\n"
         "the privileges of \"nobody\" that are available to all users\n"
         "regardless of whether or not they are logged in.\n"
         "</p></li>\n"
         "\n"
         "<li><p>\n"
         "The \"<font color=\"blue\"><big>&bull;</big></font>\" mark indicates\n"
         "the privileges of \"anonymous\" that are inherited by all logged-in users.\n"
         "</p></li>\n"
         "\n"
         "<li><p>\n"
         "The \"<font color=\"red\"><big>&bull;</big></font>\" mark indicates\n"
         "the privileges of \"developer\" that are inherited by all users with\n"
         "the <b>Developer</b> privilege.\n"
         "</p></li>\n"
         "\n"
         "<li><p>\n"
         "The \"<font color=\"black\"><big>&bull;</big></font>\" mark indicates\n"
         "the privileges of \"reader\" that are inherited by all users with\n"
         "the <b>Reader</b> privilege.\n"
         "</p></li>\n"
         "\n"
         "<li><p>\n"
         "The <b>Delete</b> privilege give the user the ability to erase\n"
         "wiki, tickets, and attachments that have been added by anonymous\n"
         "users.  This capability is intended for deletion of spam.  The\n"
         "delete capability is only in effect for 24 hours after the item\n"
         "is first posted.  The Setup user can delete anything at any time.\n"
         "</p></li>\n"
         "\n"
         "<li><p>\n"
         "The <b>History</b> privilege allows a user to see most hyperlinks.\n"
         "This is recommended ON for most logged-in users but OFF for\n"
         "user \"nobody\" to avoid problems with spiders trying to walk every\n"
         "historical version of every baseline and file.\n"
         "</p></li>\n"
         "\n"
         "<li><p>\n"
         "The <b>Zip</b> privilege allows a user to see the \"download as ZIP\"\n"
         "hyperlink and permits access to the <tt>/zip</tt> page.  This allows\n"
         "users to download ZIP archives without granting other rights like\n"
         "<b>Read</b> or <b>History</b>.  This privilege is recommended for\n"
         "user <b>nobody</b> so that automatic package downloaders can obtain\n"
         "the sources without going through the login procedure.\n"
         "</p></li>\n"
         "\n"
         "<li><p>\n"
         "The <b>Check-in</b> privilege allows remote users to \"push\".\n"
         "The <b>Check-out</b> privilege allows remote users to \"pull\".\n"
         "The <b>Clone</b> privilege allows remote users to \"clone\".\n"
         "</li><p>\n"
         "\n"
         "<li><p>\n"
         "The <b>Read Wiki</b>, <b>New Wiki</b>, <b>Append Wiki</b>, and\n"
         "<b>Write Wiki</b> privileges control access to wiki pages.  The\n"
         "<b>Read Tkt</b>, <b>New Tkt</b>, <b>Append Tkt</b>, and\n"
         "<b>Write Tkt</b> privileges control access to trouble tickets.\n"
         "The <b>Tkt Report</b> privilege allows the user to create or edit\n"
         "ticket report formats.\n"
         "</p></li>\n"
         "\n"
         "<li><p>\n"
         "Users with the <b>Password</b> privilege are allowed to change their\n"
         "own password.  Recommended ON for most users but OFF for special\n"
         "users \"developer\", \"anonymous\", and \"nobody\".\n"
         "</p></li>\n"
         "\n"
         "<li><p>\n"
         "The <b>EMail</b> privilege allows the display of sensitive information\n"
         "such as the email address of users and contact information on tickets.\n"
         "Recommended OFF for \"anonymous\" and for \"nobody\" but ON for\n"
         "\"developer\".\n"
         "</p></li>\n"
         "\n"
         "<li><p>\n"
         "Login is prohibited if the password is an empty string.\n"
         "</p></li>\n"
         "</ul>\n"
         "\n"
         "<h2>Special Logins</h2>\n"
         "\n"
         "<ul>\n"
         "<li><p>\n"
         "No login is required for user \"<b>nobody</b>\".  The capabilities\n"
         "of the <b>nobody</b> user are inherited by all users, regardless of\n"
         "whether or not they are logged in.  To disable universal access\n"
         "to the repository, make sure no user named \"<b>nobody</b>\" exists or\n"
         "that the <b>nobody</b> user has no capabilities enabled.\n"
         "The password for <b>nobody</b> is ignore.  To avoid problems with\n"
         "spiders overloading the server, it is recommended\n"
         "that the 'h' (History) capability be turned off for the <b>nobody</b>\n"
         "user.\n"
         "</p></li>\n"
         "\n"
         "<li><p>\n"
         "Login is required for user \"<b>anonymous</b>\" but the password\n"
         "is displayed on the login screen beside the password entry box\n"
         "so anybody who can read should be able to login as anonymous.\n"
         "On the other hand, spiders and web-crawlers will typically not\n"
         "be able to login.  Set the capabilities of the anonymous user\n"
         "to things that you want any human to be able to do, but not any\n"
         "spider.  Every other logged-in user inherits the privileges of\n"
         "<b>anonymous</b>.\n"
         "</p></li>\n"
         "\n"
         "<li><p>\n"
         "The \"<b>developer</b>\" user is intended as a template for trusted users\n"
         "with check-in privileges.  When adding new trusted users, simply\n"
         "select the <b>Developer</b> privilege to cause the new user to inherit\n"
         "all privileges of the \"developer\" user.  Similarly, the \"<b>reader</b>\"\n"
         "user is a template for users who are allowed more access than anonymous,\n"
         "but less than a developer.\n"
         "</p></li>\n"
         "</ul>\n"
         "</form>\n");
  style_footer();
}


/*
** Generate a checkbox for an attribute.
*/
static void onoff_attribute(
  const char *zLabel,   /* The text label on the checkbox */
  const char *zVar,     /* The corresponding row in the VAR table */
  const char *zQParm,   /* The query parameter */
  int dfltVal           /* Default value if VAR table entry does not exist */
){
  const char *zQ = P(zQParm);
  int iVal = db_get_boolean(zVar, dfltVal);
  if( zQ==0 && P("submit") ){
    zQ = "off";
  }
  if( zQ ){
    int iQ = strcmp(zQ,"on")==0 || atoi(zQ);
    if( iQ!=iVal ){
      login_verify_csrf_secret();
      db_set(zVar, iQ ? "1" : "0", 0);
      iVal = iQ;
    }
  }
  if( iVal ){
    cgi_printf("<input type=\"checkbox\" name=\"%s\" checked><b>%s</b></input>\n",zQParm,zLabel);
  }else{
    cgi_printf("<input type=\"checkbox\" name=\"%s\"><b>%s</b></input>\n",zQParm,zLabel);
  }
}

/*
** Generate an entry box for an attribute.
*/
void entry_attribute(
  const char *zLabel,   /* The text label on the entry box */
  int width,            /* Width of the entry box */
  const char *zVar,     /* The corresponding row in the VAR table */
  const char *zQParm,   /* The query parameter */
  char *zDflt     /* Default value if VAR table entry does not exist */
){
  const char *zVal = db_get(zVar, zDflt);
  const char *zQ = P(zQParm);
  if( zQ && strcmp(zQ,zVal)!=0 ){
    login_verify_csrf_secret();
    db_set(zVar, zQ, 0);
    zVal = zQ;
  }
  cgi_printf("<input type=\"text\" name=\"%s\" value=\"%h\" size=\"%d\">\n"
         "<b>%s</b>\n",zQParm,zVal,width,zLabel);
}

/*
** Generate a text box for an attribute.
*/
static void textarea_attribute(
  const char *zLabel,   /* The text label on the textarea */
  int rows,             /* Rows in the textarea */
  int cols,             /* Columns in the textarea */
  const char *zVar,     /* The corresponding row in the VAR table */
  const char *zQP,      /* The query parameter */
  const char *zDflt     /* Default value if VAR table entry does not exist */
){
  const char *z = db_get(zVar, (char*)zDflt);
  const char *zQ = P(zQP);
  if( zQ && strcmp(zQ,z)!=0 ){
    login_verify_csrf_secret();
    db_set(zVar, zQ, 0);
    z = zQ;
  }
  if( rows>0 && cols>0 ){
    cgi_printf("<textarea name=\"%s\" rows=\"%d\" cols=\"%d\">%h</textarea>\n"
           "<b>%s</b>\n",zQP,rows,cols,z,zLabel);
  }
}


/*
** WEBPAGE: setup_access
*/
void setup_access(void){
  login_check_credentials();
  if( !g.okSetup ){
    login_needed();
  }

  style_header("Access Control Settings");
  db_begin_transaction();
  cgi_printf("<form action=\"%s/setup_access\" method=\"POST\">\n",g.zBaseURL);
  login_insert_csrf_secret();
  cgi_printf("<hr>\n");
  onoff_attribute("Require password for local access",
     "localauth", "localauth", 0);
  cgi_printf("<p>When enabled, the password sign-in is required for\n"
         "web access coming from 127.0.0.1.  When disabled, web access\n"
         "from 127.0.0.1 is allows without any login - the user id is selected\n"
         "from the ~/.fossil database. Password login is always required\n"
         "for incoming web connections on internet addresses other than\n"
         "127.0.0.1.</p></li>\n");

  cgi_printf("<hr>\n");
  onoff_attribute("Show javascript button to fill in CAPTCHA",
                  "auto-captcha", "autocaptcha", 0);
  cgi_printf("<p>When enabled, a button appears on the login screen for user\n"
         "\"anonymous\" that will automatically fill in the CAPTCHA password.\n"
         "This is less secure that forcing the user to do it manually, but is\n"
         "probably secure enough and it is certainly more convenient for\n"
         "anonymous users.</p>\n");

  cgi_printf("<hr>\n");
  entry_attribute("Login expiration time", 6, "cookie-expire", "cex", "8766");
  cgi_printf("<p>The number of hours for which a login is valid.  This must be a\n"
         "positive number.  The default is 8760 hours which is approximately equal\n"
         "to a year.</p>\n");

  cgi_printf("<hr>\n");
  entry_attribute("Download packet limit", 10, "max-download", "mxdwn",
                  "5000000");
  cgi_printf("<p>Fossil tries to limit out-bound sync, clone, and pull packets\n"
         "to this many bytes, uncompressed.  If the client requires more data\n"
         "than this, then the client will issue multiple HTTP requests.\n"
         "Values below 1 million are not recommended.  5 million is a \n"
         "reasonable number.</p>\n");

  cgi_printf("<hr>\n"
         "<p><input type=\"submit\"  name=\"submit\" value=\"Apply Changes\"></p>\n"
         "</form>\n");
  db_end_transaction(0);
  style_footer();
}

/*
** WEBPAGE: setup_timeline
*/
void setup_timeline(void){
  login_check_credentials();
  if( !g.okSetup ){
    login_needed();
  }

  style_header("Timeline Display Preferences");
  db_begin_transaction();
  cgi_printf("<form action=\"%s/setup_timeline\" method=\"POST\">\n",g.zBaseURL);
  login_insert_csrf_secret();

  cgi_printf("<hr>\n");
  onoff_attribute("Allow block-markup in timeline",
                  "timeline-block-markup", "tbm", 0);
  cgi_printf("<p>In timeline displays, check-in comments can be displayed with or\n"
         "without block markup (paragraphs, tables, etc.)</p>\n");

  cgi_printf("<hr>\n");
  onoff_attribute("Use Universal Coordinated Time (UTC)",
                  "timeline-utc", "utc", 1);
  cgi_printf("<p>Show times as UTC (also sometimes called Greenwich Mean Time (GMT) or\n"
         "Zulu) instead of in local time.</p>\n");

  cgi_printf("<hr>\n");
  onoff_attribute("Show version differences by default",
                  "show-version-diffs", "vdiff", 0);
  cgi_printf("<p>On the version-information pages linked from the timeline can either\n"
         "show complete diffs of all file changes, or can just list the names of\n"
         "the files that have changed.  Users can get to either page by\n"
         "clicking.  This setting selects the default.</p>\n");

  cgi_printf("<hr>\n");
  entry_attribute("Max timeline comment length", 6, 
                  "timeline-max-comment", "tmc", "0");
  cgi_printf("<p>The maximum length of a comment to be displayed in a timeline.\n"
         "\"0\" there is no length limit.</p>\n");

  cgi_printf("<hr>\n"
         "<p><input type=\"submit\"  name=\"submit\" value=\"Apply Changes\"></p>\n"
         "</form>\n");
  db_end_transaction(0);
  style_footer();
}

/*
** WEBPAGE: setup_behavior
*/
void setup_behavior(void){
  login_check_credentials();
  if( !g.okSetup ){
    login_needed();
  }

  style_header("Fossil SCM Behavior");
  db_begin_transaction();
  cgi_printf("<form action=\"%s/setup_behavior\" method=\"POST\">\n",g.zBaseURL);
  login_insert_csrf_secret();

  cgi_printf("<hr>\n");
  onoff_attribute("Automatically synchronize with repository",
                  "autosync", "autosync", 1);
  cgi_printf("<p>Automatically keeps your work in sync with a centralized server.</p>\n");

  cgi_printf("<hr>\n");
  onoff_attribute("Show javascript button to fill in CAPTCHA",
                  "auto-captcha", "autocaptcha", 0);
  cgi_printf("<p>When enabled, a button appears on the login screen for user\n"
         "\"anonymous\" that will automatically fill in the CAPTCHA password.\n"
         "This is less secure that forcing the user to do it manually, but is\n"
         "probably secure enough and it is certainly more convenient for\n"
         "anonymous users.</p>\n");

  cgi_printf("<hr>\n");
  onoff_attribute("Sign all commits with GPG",
                  "clearsign", "clearsign", 0);
  cgi_printf("<p>When enabled (the default), fossil will attempt to\n"
         "    sign all commits with GPG.  When disabled, commits will\n"
         "   be unsigned.</p>  \n");
  
  cgi_printf("<hr>\n");
  onoff_attribute("Require local authentication",
                  "localauth", "localauth", 0);
  cgi_printf("<p>If enabled, require that HTTP connections from\n"
         "        127.0.0.1 be authenticated by password.  If\n"
         "       false, all HTTP requests from localhost have\n"
         "       unrestricted access to the repository.</p>  \n");
  
  cgi_printf("<hr>\n");
  onoff_attribute("Modification times used to detect changes",
                  "mtime-changes", "mtime-changes", 0);
  cgi_printf("<p>Use file modification times (mtimes) to detect when files have been modified.</p>  \n");
  
  cgi_printf("<hr>\n");
  entry_attribute("Diff Command", 16, 
                  "diff-command", "diff-command", "diff");
  cgi_printf("<p>External command used to generate a textual diff</p>  \n");
  
  cgi_printf("<hr>\n");
  entry_attribute("Gdiff Command", 16, 
                  "gdiff-command", "gdiff-command", "gdiff");
  cgi_printf("<p>External command to run when performing a graphical diff. If undefined, text diff will be used.</p>  \n");
    
  cgi_printf("<hr>\n");
  entry_attribute("Editor", 16, 
                  "editor", "editor", "");
  cgi_printf("<p>Text editor command used for check-in comments.</p>  \n");
  
  cgi_printf("<hr>\n");
  entry_attribute("HTTP port", 16, 
                  "http-port", "http-port", "8080");
  cgi_printf("<p>The TCP/IP port number to use by the \"server\" and \"ui\" commands.  Default: 8080</p>  \n");

  cgi_printf("<hr>\n");
  entry_attribute("PGP Command", 32, 
                  "pgp-command", "pgp-command", "gpg --clearsign -o ");
  cgi_printf("<p>Command used to clear-sign manifests at check-in.The default is \"gpg --clearsign -o \".</p>  \n");
  
  cgi_printf("<hr>\n");
  entry_attribute("Proxy", 32, 
                  "proxy", "proxy", "off");
  cgi_printf("<p>URL of the HTTP proxy.</p>  \n");
  
  cgi_printf("<hr>\n");
  entry_attribute("Web browser", 32, 
                  "web-browser", "web-browser", "");
  cgi_printf("<p>Default web browser for \"fossil ui\".</p>  \n");

  cgi_printf("<hr>\n"
         "<p><input type=\"submit\"  name=\"submit\" value=\"Apply Changes\"></p>\n"
         "</form>\n");
  db_end_transaction(0);
  style_footer();
}

/*
** WEBPAGE: setup_config
*/
void setup_config(void){
  login_check_credentials();
  if( !g.okSetup ){
    login_needed();
  }

  style_header("WWW Configuration");
  db_begin_transaction();
  cgi_printf("<form action=\"%s/setup_config\" method=\"POST\">\n",g.zBaseURL);
  login_insert_csrf_secret();
  cgi_printf("<hr />\n");
  entry_attribute("Project Name", 60, "project-name", "pn", "");
  cgi_printf("<p>Give your project a name so visitors know what this site is about.\n"
         "The project name will also be used as the RSS feed title.</p>\n"
         "<hr />\n");
  textarea_attribute("Project Description", 5, 60,
                     "project-description", "pd", "");
  cgi_printf("<p>Describe your project. This will be used in page headers for search\n"
         "engines as well as a short RSS description.</p>\n"
         "<hr />\n");
  entry_attribute("Index Page", 60, "index-page", "idxpg", "/home");
  cgi_printf("<p>Enter the pathname of the page to display when the \"Home\" menu\n"
         "option is selected and when no pathname is\n"
         "specified in the URL.  For example, if you visit the url:</p>\n"
         "\n"
         "<blockquote>%h</blockquote>\n"
         "\n"
         "<p>And you have specified an index page of \"/home\" the above will\n"
         "automatically redirect to:</p>\n"
         "\n"
         "<blockquote>%h/home</blockquote>\n"
         "\n"
         "<p>The default \"/home\" page displays a Wiki page with the same name\n"
         "as the Project Name specified above.  Some sites prefer to redirect\n"
         "to a documentation page (ex: \"/doc/tip/index.wiki\") or to \"/timeline\".</p>\n"
         "<hr />\n",g.zBaseURL,g.zBaseURL);
  onoff_attribute("Use HTML as wiki markup language",
    "wiki-use-html", "wiki-use-html", 0);
  cgi_printf("<p>Use HTML as the wiki markup language. Wiki links will still be parsed but\n"
         "all other wiki formatting will be ignored. This option is helpful if you have\n"
         "chosen to use a rich HTML editor for wiki markup such as TinyMCE.</p>\n"
         "<p><strong>CAUTION:</strong> when\n"
         "enabling, <i>all</i> HTML tags and attributes are accepted in the wiki.\n"
         "No sanitization is done. This means that it is very possible for malicious\n"
         "users to inject dangerous HTML, CSS and JavaScript code into your wiki.</p>\n"
         "<p>This should <strong>only</strong> be enabled when wiki editing is limited\n"
         "to trusted users. It should <strong>not</strong> be used on a publically\n"
         "editable wiki.</p>\n"
         "<hr />\n"
         "<p><input type=\"submit\"  name=\"submit\" value=\"Apply Changes\"></p>\n"
         "</form>\n");
  db_end_transaction(0);
  style_footer();
}

/*
** WEBPAGE: setup_editcss
*/
void setup_editcss(void){
  login_check_credentials();
  if( !g.okSetup ){
    login_needed();
  }
  db_begin_transaction();
  if( P("clear")!=0 ){
    db_multi_exec("DELETE FROM config WHERE name='css'");
    cgi_replace_parameter("css", zDefaultCSS);
    db_end_transaction(0);
    cgi_redirect("setup_editcss");
  }else{
    textarea_attribute(0, 0, 0, "css", "css", zDefaultCSS);
  }
  if( P("submit")!=0 ){
    db_end_transaction(0);
    cgi_redirect("setup_editcss");
  }
  style_header("Edit CSS");
  cgi_printf("<form action=\"%s/setup_editcss\" method=\"POST\">\n",g.zBaseURL);
  login_insert_csrf_secret();
  cgi_printf("Edit the CSS below:<br />\n");
  textarea_attribute("", 40, 80, "css", "css", zDefaultCSS);
  cgi_printf("<br />\n"
         "<input type=\"submit\" name=\"submit\" value=\"Apply Changes\">\n"
         "<input type=\"submit\" name=\"clear\" value=\"Revert To Default\">\n"
         "</form>\n"
         "<p><b>Note:</b> Press your browser Reload button after modifying the\n"
         "CSS in order to pull in the modified CSS file.</p>\n"
         "<hr>\n"
         "The default CSS is shown below for reference.  Other examples\n"
         "of CSS files can be seen on the <a href=\"setup_skin\">skins page</a>.\n"
         "See also the <a href=\"setup_header\">header</a> and \n"
         "<a href=\"setup_footer\">footer</a> editing screens.\n"
         "<blockquote><pre>\n"
         "%h\n"
         "</pre></blockquote>\n",zDefaultCSS);
  style_footer();
  db_end_transaction(0);
}

/*
** WEBPAGE: setup_header
*/
void setup_header(void){
  login_check_credentials();
  if( !g.okSetup ){
    login_needed();
  }
  db_begin_transaction();
  if( P("clear")!=0 ){
    db_multi_exec("DELETE FROM config WHERE name='header'");
    cgi_replace_parameter("header", zDefaultHeader);
  }else{
    textarea_attribute(0, 0, 0, "header", "header", zDefaultHeader);
  }
  style_header("Edit Page Header");
  cgi_printf("<form action=\"%s/setup_header\" method=\"POST\">\n",g.zBaseURL);
  login_insert_csrf_secret();
  cgi_printf("<p>Edit HTML text with embedded TH1 (a TCL dialect) that will be used to\n"
         "generate the beginning of every page through start of the main\n"
         "menu.</p>\n");
  textarea_attribute("", 40, 80, "header", "header", zDefaultHeader);
  cgi_printf("<br />\n"
         "<input type=\"submit\" name=\"submit\" value=\"Apply Changes\">\n"
         "<input type=\"submit\" name=\"clear\" value=\"Revert To Default\">\n"
         "</form>\n"
         "<hr>\n"
         "The default header is shown below for reference.  Other examples\n"
         "of headers can be seen on the <a href=\"setup_skin\">skins page</a>.\n"
         "See also the <a href=\"setup_editcss\">CSS</a> and\n"
         "<a href=\"setup_footer\">footer</a> editing screeens.\n"
         "<blockquote><pre>\n"
         "%h\n"
         "</pre></blockquote>\n",zDefaultHeader);
  style_footer();
  db_end_transaction(0);
}

/*
** WEBPAGE: setup_footer
*/
void setup_footer(void){
  login_check_credentials();
  if( !g.okSetup ){
    login_needed();
  }
  db_begin_transaction();
  if( P("clear")!=0 ){
    db_multi_exec("DELETE FROM config WHERE name='footer'");
    cgi_replace_parameter("footer", zDefaultFooter);
  }else{
    textarea_attribute(0, 0, 0, "footer", "footer", zDefaultFooter);
  }
  style_header("Edit Page Footer");
  cgi_printf("<form action=\"%s/setup_footer\" method=\"POST\">\n",g.zBaseURL);
  login_insert_csrf_secret();
  cgi_printf("<p>Edit HTML text with embedded TH1 (a TCL dialect) that will be used to\n"
         "generate the end of every page.</p>\n");
  textarea_attribute("", 20, 80, "footer", "footer", zDefaultFooter);
  cgi_printf("<br />\n"
         "<input type=\"submit\" name=\"submit\" value=\"Apply Changes\">\n"
         "<input type=\"submit\" name=\"clear\" value=\"Revert To Default\">\n"
         "</form>\n"
         "<hr>\n"
         "The default footer is shown below for reference.  Other examples\n"
         "of footers can be seen on the <a href=\"setup_skin\">skins page</a>.\n"
         "See also the <a href=\"setup_editcss\">CSS</a> and\n"
         "<a href=\"setup_header\">header</a> editing screens.\n"
         "<blockquote><pre>\n"
         "%h\n"
         "</pre></blockquote>\n",zDefaultFooter);
  style_footer();
  db_end_transaction(0);
}

/*
** WEBPAGE: setup_logo
*/
void setup_logo(void){
  const char *zMime = "image/gif";
  const char *aImg = P("im");
  int szImg = atoi(PD("im:bytes","0"));
  if( szImg>0 ){
    zMime = PD("im:mimetype","image/gif");
  }
  login_check_credentials();
  if( !g.okSetup ){
    login_needed();
  }
  db_begin_transaction();
  if( P("set")!=0 && zMime && zMime[0] && szImg>0 ){
    Blob img;
    Stmt ins;
    blob_init(&img, aImg, szImg);
    db_prepare(&ins, 
        "REPLACE INTO config(name, value)"
        " VALUES('logo-image',:bytes)"
    );
    db_bind_blob(&ins, ":bytes", &img);
    db_step(&ins);
    db_finalize(&ins);
    db_multi_exec(
       "REPLACE INTO config(name, value) VALUES('logo-mimetype',%Q)",
       zMime
    );
    db_end_transaction(0);
    cgi_redirect("setup_logo");
  }else if( P("clr")!=0 ){
    db_multi_exec(
       "DELETE FROM config WHERE name GLOB 'logo-*'"
    );
    db_end_transaction(0);
    cgi_redirect("setup_logo");
  }
  style_header("Edit Project Logo");
  cgi_printf("<p>The current project logo has a MIME-Type of <b>%h</b> and looks\n"
         "like this:</p>\n"
         "<blockquote><img src=\"%s/logo\" alt=\"logo\"></blockquote>\n"
         "\n"
         "<p>The logo is accessible to all users at this URL:\n"
         "<a href=\"%s/logo\">%s/logo</a>.\n"
         "The logo may or may not appear on each\n"
         "page depending on the <a href=\"setup_editcss\">CSS</a> and\n"
         "<a href=\"setup_header\">header setup</a>.</p>\n"
         "\n"
         "<form action=\"%s/setup_logo\" method=\"POST\"\n"
         " enctype=\"multipart/form-data\">\n"
         "<p>To set a new logo image, select a file to use as the logo using\n"
         "the entry box below and then press the \"Change Logo\" button.</p>\n",zMime,g.zTop,g.zBaseURL,g.zBaseURL,g.zBaseURL);
  login_insert_csrf_secret();
  cgi_printf("Logo Image file:\n"
         "<input type=\"file\" name=\"im\" size=\"60\" accepts=\"image/*\"><br>\n"
         "<input type=\"submit\" name=\"set\" value=\"Change Logo\">\n"
         "<input type=\"submit\" name=\"clr\" value=\"Revert To Default\">\n"
         "</form>\n"
         "\n"
         "<p><b>Note:</b>  Your browser has probably cached the logo image, so\n"
         "you will probably need to press the Reload button on your browser after\n"
         "changing the logo to provoke your browser to reload the new logo image.\n"
         "</p>\n");
  style_footer();
  db_end_transaction(0);
}

/*
** WEBPAGE: setup_sync
*/
void setup_sync(void){
	sync_cmd();
	style_header("Synchronized");
 cgi_printf("<p>The project has been synchronized</p>\n");
	style_footer();
}
