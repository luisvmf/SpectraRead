import gi
import sys
import gtk
import time
import os
from multiprocessing import Process, Manager, Value, Array
from ctypes import c_char_p

def getgtkcolors(gtkcolors):
	if not hasattr(sys, 'argv'):
		sys.argv  = ['']
	gi.require_version('Gtk', '3.0')
	from gi.repository import Gtk, Gdk
	global returnvalue
	returnvalue=""
	class MyWIndowb(Gtk.Window):
		global returnvalue
		def __init__(self):
			global returnvalue
			Gtk.Window.__init__(self)
			hbox = Gtk.Box(spacing=6)
			self.add(hbox)
			menubar = Gtk.MenuBar()
			menuitem = Gtk.MenuItem(label='test\nline2')
			submenubar = Gtk.Menu()
			menuitemb = Gtk.MenuItem(label='test\nline2')
			submenubar.append(menuitemb)
			menuitem.set_submenu(submenubar)
			menubar.append(menuitem)
			toolbar = Gtk.Toolbar()
			context = toolbar.get_style_context()
			context.add_class(Gtk.STYLE_CLASS_PRIMARY_TOOLBAR)
			buttont = Gtk.Button(label="Click")
			tool_item = Gtk.ToolItem()
			tool_item.add(buttont)
			toolbar.insert(tool_item,0)
			hbox.pack_start(toolbar, True, True, 0)
			hbox.pack_start(menubar, True, True, 0)
			button = Gtk.Button(label="Click")
			hbox.pack_start(button, True, True, 0)
			#self.add(self.button)
			entry = Gtk.Entry()
			entry.set_text("Hello World")
			hbox.pack_start(entry, True, True, 0)
			#self.add(self.entry)
			style = button.get_style_context()
			styleb = entry.get_style_context()
			stylec = MyWIndowb.get_style_context(self)
			styled = buttont.get_style_context()
			stylee = toolbar.get_style_context()
			#color = style.get_background_color(Gtk.StateFlags.NORMAL)
			color = style.lookup_color("theme_bg_color").color
			colorb =	style.get_background_color(Gtk.StateFlags.ACTIVE)
			colorc =	styleb.get_color(Gtk.StateFlags.NORMAL)
			#colord =	styleb.get_background_color(Gtk.StateFlags.NORMAL)
			colord = styleb.lookup_color("theme_bg_color").color
			colore =	styleb.get_background_color(Gtk.StateFlags.SELECTED)
			colorf =	styleb.get_color(Gtk.StateFlags.SELECTED)
			#colorg =	stylec.get_background_color(Gtk.StateFlags.NORMAL)
			colorg = stylec.lookup_color("theme_bg_color").color
			#colorh =	styled.get_background_color(Gtk.StateFlags.NORMAL)
			colorh = styled.lookup_color("theme_bg_color").color
			colori =	styled.get_background_color(Gtk.StateFlags.ACTIVE)
			#colorj =	stylee.get_background_color(Gtk.StateFlags.NORMAL)
			colorj = stylee.lookup_color("theme_bg_color").color
			returnvalue=""
			returnvalue=returnvalue+str("button-normal:"+str('#'+''.join(map(chr, (int(color.red*255),int(color.green*255),int(color.blue*255)))).encode('hex')))+";"
			returnvalue=returnvalue+str("button-active-unsafe:"+str('#'+''.join(map(chr, (int(colorb.red*255),int(colorb.green*255),int(colorb.blue*255)))).encode('hex')))+";"
			returnvalue=returnvalue+str("text-input-normal:"+str('#'+''.join(map(chr, (int(colord.red*255),int(colord.green*255),int(colord.blue*255)))).encode('hex')))+";"
			returnvalue=returnvalue+str("text-input-selected-unsafe:"+str('#'+''.join(map(chr, (int(colore.red*255),int(colore.green*255),int(colore.blue*255)))).encode('hex')))+";"
			returnvalue=returnvalue+str("text-normal-color:"+str('#'+''.join(map(chr, (int(colorc.red*255),int(colorc.green*255),int(colorc.blue*255)))).encode('hex')))+";"
			returnvalue=returnvalue+str("text-selected-color:"+str('#'+''.join(map(chr, (int(colorf.red*255),int(colorf.green*255),int(colorf.blue*255)))).encode('hex')))+";"
			returnvalue=returnvalue+str("window-normal:"+str('#'+''.join(map(chr, (int(colorg.red*255),int(colorg.green*255),int(colorg.blue*255)))).encode('hex')))+";"
			returnvalue=returnvalue+str("menu-normal:"+str('#'+''.join(map(chr, (int(colorj.red*255),int(colorj.green*255),int(colorj.blue*255)))).encode('hex')))+";"
			returnvalue=returnvalue+str("menu-item-normal:"+str('#'+''.join(map(chr, (int(colorh.red*255),int(colorh.green*255),int(colorh.blue*255)))).encode('hex')))+";"
			returnvalue=returnvalue+str("menu-item-active-unsafe:"+str('#'+''.join(map(chr, (int(colori.red*255),int(colori.green*255),int(colori.blue*255)))).encode('hex')))+";"
			#returnvalue=returnvalue+str(styled.lookup_color("theme_bg_primary_color"))+"\n"
			gtkcolors.value=returnvalue
	winb = MyWIndowb()
	winb.close()
	winb.destroy()


def getcolors():
	managere = Manager()
	gtkcolors = managere.Value(c_char_p, "")
	p1b=Process(target=getgtkcolors, args=(gtkcolors,))
	p1b.start()
	time.sleep(0.5)
	p1b.terminate()
	time.sleep(0.1)
	return(gtkcolors.value)
