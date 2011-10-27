#!/usr/bin/python

# requires dogtail

from dogtail import tree
from dogtail.utils import run
from dogtail.procedural import *
from time import sleep
from os import environ, path, remove
from dogtail.utils import screenshot
environ['LANG']='en_US.UTF-8'
import gtk
import os
import shutil
import time

def xdotool(cmd):
    cmd = "xdotool "+cmd
    print cmd
    f=os.popen(cmd)
    f.close()

default_size = "628 300"

def focus_class(name):
    xdotool("search --onlyvisible --classname \"%s\" windowfocus %%@"%(name))

def resize_class(name):
    xdotool("search --onlyvisible --classname \"%s\" windowsize %%@ %s"%(name,default_size))

def resize_name(name):
    xdotool("search --onlyvisible --name \"%s\" windowsize %%@ %s"%(name,default_size))

def take_snapshot(fname,fmt):
    screenw = gtk.gdk.screen_width()
    screenh = gtk.gdk.screen_height()
    root = gtk.gdk.screen_get_default()
    if root.supports_net_wm_hint("_NET_ACTIVE_WINDOW") and root.supports_net_wm_hint("_NET_WM_WINDOW_TYPE"):
        active = root.get_active_window()
        if active.property_get("_NET_WM_WINDOW_TYPE")[-1][0] == '_NET_WM_WINDOW_TYPE_DESKTOP':
            return False

        relativex, relativey, winw, winh, d = active.get_geometry() 
        w = winw + (relativex*2)
        h = winh + (relativey+relativex)

        screenposx, screenposy = active.get_root_origin()
    else:
        return False

    screenshot = gtk.gdk.Pixbuf.get_from_drawable(gtk.gdk.Pixbuf(gtk.gdk.COLORSPACE_RGB, True, 8, w, h),
                                                  gtk.gdk.get_default_root_window(),
                                                  gtk.gdk.colormap_get_system(),
                                                  screenposx, screenposy, 0, 0, w, h)

    screenshot.save(fname, fmt,  {})
    print("Snapshot %s"%fname)

bin = os.getcwd()
delay = 2

def snap(name):
    time.sleep(delay)
    # fname = bin + "/../doc/images/screenshot/coopy_" + name + ".png"
    fname = /tmp/coopy_snaps/coopy_" + name + ".png"
    take_snapshot(fname,"png")

do_over = True

os.chdir("/tmp")
if do_over:
    shutil.rmtree("coopy_demo",True)
    os.mkdir("coopy_demo")

shutil.rmtree("coopy_snaps",True)
os.mkdir("coopy_snaps")

os.chdir("coopy_demo")

run(os.path.join(os.path.join(bin,"bin"),"coopy"), appName = "coopy")
# resize_name("^Coopy")

app = tree.root.application("coopy")

if do_over:
    snap("main")

    app.button('Pull in').click()
    resize_name("Select directory to work in")
    snap("pull")

    app.button('Open').click()
    snap("open")

    lnk="https://chiselapp.com/user/coopy/repository/coopy_demo/index"
    
    dlg = app.window("Enter repository link",True)
    dlg.child(roleName = 'text').text = lnk
    
    snap("open2")
    
    app.button('OK').click()

    # time.sleep(5)


#dlg = app.dialog("Select directory to work in")
#app.button('Cancel').click()

print app.children

# app.child(roleName = 'table')
numbers = app.child(roleName = 'table cell', name='numbers')
# click('numbers', roleName = 'table cell', raw=True)
numbers.grabFocus()
snap("after_pull")
print numbers.actions
numbers.doAction('activate')
dlg = app.dialog("Save File As _?",True)
resize_name("Save File As _?")
shutil.rmtree("/tmp/numbers.xls",True)
dlg.child(roleName = 'text').text = "/tmp/numbers.xls"
snap("save_as")
dlg.button('Save').click()
# dlg.button('Cancel').click()

#app.menu('File').click()
#snap("file")

app.button('Exit').click()

resize_class("gnumeric")
focus_class("gnumeric")
app2 = tree.root.application("gnumeric")
snap("xls")
app2.menu('File').menuItem('Quit').click()
