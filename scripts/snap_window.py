#!/usr/bin/python

from sys import argv
import gtk

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
    print("Took snapshot %s"%fname)

if len(argv)<3:
    print "Call with base and name"
    exit(1)

base = argv[1]
name = argv[2]
fmt = "png"
fname = base + name + "." + fmt

if not(take_snapshot(fname,fmt)):
    exit(1)





