import random
import time
import sys
import os,errno,time
from multiprocessing import Process, Manager, Value, Array
from ctypes import c_char_p, c_int
import warnings


warnings.filterwarnings("ignore")
sys.tracebacklimit=0

def showerrormessage(messagetext):
	idproc=os.fork()
	if(idproc==0):
		import signal
		def message(messagetext,winclosed):
			import gi
			if not hasattr(sys, 'argv'):
				sys.argv  = ['']
			gi.require_version('Gtk', '3.0')
			from gi.repository import Gtk, Gdk, GObject
			def init():
				class Dialog(Gtk.Dialog):
					def __init__(self):
						Gtk.Window.__init__(self,title="Error")
						self.set_default_icon_from_file("resources/icon.svg")
						self.set_default_size(150, 100)
						self.add_button("_OK", Gtk.ResponseType.OK)
						self.connect("response", self.on_response)
						self.set_keep_above(True)
						hboxdialogerror = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=3)
						imageerror=(Gtk.Image.new_from_icon_name("dialog-error",Gtk.IconSize.DIALOG))
						label = Gtk.Label(""+str(messagetext.value))
						hboxdialogerror.pack_start(imageerror, False, False, 0)
						hboxdialogerror.pack_start(label, True, True, 0)
						box = self.get_content_area()
						box.add(hboxdialogerror)
						self.show_all()
					def on_response(self, dialog,response):
						winclosed.value="True"
						dialog.close()
						dialog.destroy()
				win = Dialog()
				win.show_all()
				win.connect("delete-event", Gtk.main_quit)
				Gtk.main()
				win.close()
				win.destroy()
			init()
			quit()
		managerc = Manager()
		internaltextvalue = managerc.Value(c_char_p, ""+str(messagetext))
		winclosed = managerc.Value(c_char_p, "")
		p2=Process(target=message,args=(internaltextvalue,winclosed))
		p2.start()
		print "Error opened"
		while(str(winclosed.value)!="True"):
			time.sleep(0.1)
		print "Error closed"
		p2.terminate()
		os.kill(os.getpid(),signal.SIGKILL)
	print "return"
