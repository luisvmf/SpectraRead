#MIT License
#
#Copyright (c) 2018 Luis Victor Muller Fabris
#
#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:
#
#The above copyright notice and this permission notice shall be included in all
#copies or substantial portions of the Software.
#
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#SOFTWARE.

import os
try:
	os.mkdir(os.path.expanduser("~")+"/.spectraread/")
except:
	null=None
try:
	os.mkdir(os.path.expanduser("~")+"/.spectraread/spectrareadcfg/")
except:
	null=None
try:
	os.mkdir(os.path.expanduser("~")+"/.spectraread/spectrareadegg/")
except:
	null=None
os.environ['MPLCONFIGDIR'] = os.path.expanduser("~")+"/.spectraread/spectrareadcfg/"
os.environ['PYTHON_EGG_CACHE'] = os.path.expanduser("~")+"/.spectraread/spectrareadegg/"
#print(os.getgroups())

import fastmmap
import gi
import sys
reload(sys)
sys.setdefaultencoding('utf-8')
import gtk
import time
#import os
import threading
import warnings
import shelve
warnings.filterwarnings("ignore")
sys.tracebacklimit=0
from multiprocessing import Process, Manager, Value, Array
from ctypes import c_char_p
import blank_module.blank_module_toolbar_options as blank_module
if not hasattr(sys, 'argv'):
	sys.argv  = ['']
gi.require_version('Gtk', '3.0')
gi.require_version('WebKit', '3.0')
from gi.repository import Gtk, Gdk, WebKit, GObject


import numpy as np
import matplotlib
#print matplotlib.get_configdir()
#print matplotlib.get_cachedir()
matplotlib.use('GTK3Agg')
import matplotlib.pyplot as plt
import matplotlib.patches as patches
#from mpl_toolkits.mplot3d import Axes3D
from matplotlib.figure import Figure
from numpy import arange, pi, random, linspace
#Possibly this rendering backend is broken currently
#from gi.repository import GObject
#from gi.repository import Gtk
matplotlib.use('GTK3Agg')
from matplotlib.backends.backend_gtk3agg import FigureCanvasGTK3Agg as FigureCanvas
#from matplotlib.backends.backend_gtk3cairo import FigureCanvasGTK3Cairo as FigureCanvas
from matplotlib.colors import ColorConverter
from matplotlib.backends.backend_gtk3 import NavigationToolbar2GTK3 as NavigationToolbar
from matplotlib.backend_tools import ToolBase, ToolToggleBase



mmap=-1
mmapbutton=-1
def mainui(ui_values,gtkcolorsfromnode,spectrum,savecommand,infotext,windowclose,uncheck,spectrumx,readyshoww,readshowwin):
	global currentdevice
	currentdevice=blank_module
	global returna
	returna=''
	global multiplefilesavedialoginternal
	multiplefilesavedialoginternal=""
	class MyWIndow(Gtk.Window):
		global currentdevice
		global returna
		def __init__(self):
			time.sleep(0.5)
			fastmmap.write(mmap,",20loading dialogs")
			def save_single_spectrum(dgggf):
				class Dialog(Gtk.Dialog):
					def __init__(self):
						Gtk.Dialog.__init__(self)
						dialogb = Gtk.FileChooserDialog("Save spectrum", self,Gtk.FileChooserAction.SAVE,(Gtk.STOCK_CANCEL, Gtk.ResponseType.CANCEL,Gtk.STOCK_SAVE, Gtk.ResponseType.OK))
						#self.add_filters(dialog)
						hboxsavedialog = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=3)
						f2 = Gtk.Label("Line Ending:")
						f3 = Gtk.Label("Extension:")
						fileext = Gtk.ListStore(int, str)
						fileext.append([1, ".dat"])
						fileext.append([2, ".txt"])
						fileextcombo = Gtk.ComboBox.new_with_model(fileext)
						renderer_textb = Gtk.CellRendererText()
						fileextcombo.pack_start(renderer_textb, True)
						fileextcombo.add_attribute(renderer_textb, "text", 1)
						hboxsavedialog.pack_start(f2, True, True,0)
						hboxsavedialog.pack_end(fileextcombo, False, True, 0)
						fileextcombo.set_active(0)
						f2.show()
						fileextcombo.show()
						fileextb = Gtk.ListStore(int, str)
						fileextb.append([1, "Unix / Linux"])
						fileextcombob = Gtk.ComboBox.new_with_model(fileextb)
						renderer_textbb = Gtk.CellRendererText()
						fileextcombob.pack_start(renderer_textbb, True)
						fileextcombob.add_attribute(renderer_textbb, "text", 1)
						hboxsavedialog.pack_start(f2, True, True,0)
						hboxsavedialog.pack_start(fileextcombo, True, True, 0)
						hboxsavedialog.pack_end(f3, True, True,0)
						hboxsavedialog.pack_end(fileextcombob, True, True,0)
						fileextcombo.set_active(0)
						fileextcombob.set_active(0)
						f2.show()
						fileextcombo.show()
						f3.show()
						fileextcombob.show()
						dialogb.set_extra_widget(hboxsavedialog)
						response = dialogb.run()
						if response == Gtk.ResponseType.OK:
							if(str(fileextcombo.get_model()[fileextcombo.get_active_iter()][:2][0])+""=="1"):
								savecommand.value="/ssghhgdjhdfsdgihidghiughdfghdihdighidgihhigoighihgghidehidfs:::..."+dialogb.get_filename()+".dat"
							if(str(fileextcombo.get_model()[fileextcombo.get_active_iter()][:2][0])+""=="2"):
								savecommand.value="/ssghhgdjhdfsdgihidghiughdfghdihdighidgihhigoighihgghidehidfs:::..."+dialogb.get_filename()+".txt"
						dialogb.destroy()
						dialog.destroy()
				dialog = Dialog()
				dialog.run()
			def save_single_peaks(dgggf):
				class Dialog(Gtk.Dialog):
					def __init__(self):
						Gtk.Dialog.__init__(self)
						dialogb = Gtk.FileChooserDialog("Save peaks", self,Gtk.FileChooserAction.SAVE,(Gtk.STOCK_CANCEL, Gtk.ResponseType.CANCEL,Gtk.STOCK_SAVE, Gtk.ResponseType.OK))
						#self.add_filters(dialog)
						hboxsavedialog = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=3)
						f2 = Gtk.Label("Line Ending:")
						f3 = Gtk.Label("Extension:")
						fileext = Gtk.ListStore(int, str)
						fileext.append([1, ".dat"])
						fileext.append([2, ".txt"])
						fileextcombo = Gtk.ComboBox.new_with_model(fileext)
						renderer_textb = Gtk.CellRendererText()
						fileextcombo.pack_start(renderer_textb, True)
						fileextcombo.add_attribute(renderer_textb, "text", 1)
						hboxsavedialog.pack_start(f2, True, True,0)
						hboxsavedialog.pack_end(fileextcombo, False, True, 0)
						fileextcombo.set_active(0)
						f2.show()
						fileextcombo.show()
						fileextb = Gtk.ListStore(int, str)
						fileextb.append([1, "Unix / Linux"])
						fileextcombob = Gtk.ComboBox.new_with_model(fileextb)
						renderer_textbb = Gtk.CellRendererText()
						fileextcombob.pack_start(renderer_textbb, True)
						fileextcombob.add_attribute(renderer_textbb, "text", 1)
						hboxsavedialog.pack_start(f2, True, True,0)
						hboxsavedialog.pack_start(fileextcombo, True, True, 0)
						hboxsavedialog.pack_end(f3, True, True,0)
						hboxsavedialog.pack_end(fileextcombob, True, True,0)
						fileextcombo.set_active(0)
						fileextcombob.set_active(0)
						f2.show()
						fileextcombo.show()
						f3.show()
						fileextcombob.show()
						dialogb.set_extra_widget(hboxsavedialog)
						response = dialogb.run()
						if response == Gtk.ResponseType.OK:
							if(str(fileextcombo.get_model()[fileextcombo.get_active_iter()][:2][0])+""=="1"):
								savecommand.value="/spiagfjoigdjguhgirthrhhirgtvgfhyhoihgolhhgheihlaetgahtghuthgi:::..."+dialogb.get_filename()+".dat"
							if(str(fileextcombo.get_model()[fileextcombo.get_active_iter()][:2][0])+""=="2"):
								savecommand.value="/spiagfjoigdjguhgirthrhhirgtvgfhyhoihgolhhgheihlaetgahtghuthgi:::..."+dialogb.get_filename()+".txt"
						dialogb.destroy()
						dialog.destroy()
				dialog = Dialog()
				dialog.run()
			def save_single_spectrum_peaks(dgggf):
				global multiplefilesavedialoginternal
				class Dialog(Gtk.Dialog):
					global multiplefilesavedialoginternal
					def __init__(self):
						global multiplefilesavedialoginternal
						Gtk.Dialog.__init__(self)
						self.set_title("Save single spectrum and peaks")
						self.set_default_size(500, 150)
						self.set_resizable(False)
						self.add_button("Cancel", Gtk.ResponseType.CANCEL)
						buttondialognext=self.add_button("Next", Gtk.ResponseType.OK)
						buttondialognext.set_sensitive(False)
						self.connect("response", self.on_response)
						hboxdialog = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=3)
						hboxdialogb = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=3)
						imagea=(Gtk.Image.new_from_icon_name("document-save",Gtk.IconSize.DIALOG))
						hboxdialogintb = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=3)
						hboxdialoginta = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=3)
						hboxdialog.pack_start(imagea, False, False, 0)
						hboxdialog.pack_start(hboxdialogb, False, False, 0)
						hboxdialogb.pack_start(hboxdialogintb, False, False, 0)
						hboxdialogb.pack_start(hboxdialoginta, False, False, 0)
						hboxdialogintc = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=0)
						hboxdialogb.pack_start(hboxdialogintc, False, False, 0)
						labelfilename = Gtk.Label("\nPeaks file name:       ")
						fileentry=Gtk.Entry();
						fileentryb=Gtk.Entry();
						fileentryb.set_text(".dat")
						fileentry.set_progress_pulse_step(1)
						fileentry.progress_pulse()
						fileentryb.set_editable(False)
						fileentryb.set_state_flags(Gtk.StateFlags.INSENSITIVE, True)
						hboxdialogintd = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=0)
						hboxdialogb.pack_start(hboxdialogintd, False, False, 0)
						labelfilenamed = Gtk.Label("\nSpectrum file name:    ")
						fileentryd=Gtk.Entry();
						fileentrybd=Gtk.Entry();
						fileentrybd.set_text(".dat")
						fileentryd.set_progress_pulse_step(1)
						fileentryd.progress_pulse()
						fileentrybd.set_editable(False)
						fileentrybd.set_state_flags(Gtk.StateFlags.INSENSITIVE, True)
						hboxdialogintd.pack_start(labelfilenamed, False, False, 0)
						hboxdialogintd.pack_start(fileentryd, False, False, 0)
						hboxdialogintd.pack_start(fileentryb, False, False, 0)
						hboxdialogintc.pack_start(labelfilename, False, False, 0)
						hboxdialogintc.pack_start(fileentry, False, False, 0)
						hboxdialogintc.pack_start(fileentrybd, False, False, 0)
						self.vbox.add(hboxdialog)
						multiplefilesavedialoginternal="/ssspdfjhhvgfjkdbfyhzszsfygujfbhyeurrfgvuekfafrgruyrfruyraewolufgeyrofrlewygarfaulg:::..."
						def updatesavemultipleinternalvalues(bhgdsgydsygybgyuhrghsjhf):
							global multiplefilesavedialoginternal
							if (fileentry.get_text()+""!=""):
								fileentry.set_progress_pulse_step(0)
								fileentry.progress_pulse()
							if (fileentry.get_text()+""==""):
								fileentry.set_progress_pulse_step(1)
								fileentry.progress_pulse()
								buttondialognext.set_sensitive(False)
							if (fileentryd.get_text()+""!=""):
								fileentryd.set_progress_pulse_step(0)
								fileentryd.progress_pulse()
							if (fileentryd.get_text()+""==""):
								fileentryd.set_progress_pulse_step(1)
								fileentryd.progress_pulse()
								buttondialognext.set_sensitive(False)
							if (fileentryd.get_text()+""!=""):
								if (fileentry.get_text()+""!=""):
									if (fileentry.get_text()+""!=fileentryd.get_text()+""):
										buttondialognext.set_sensitive(True)
							if (fileentry.get_text()+""==fileentryd.get_text()+""):
								fileentryd.set_progress_pulse_step(1)
								fileentryd.progress_pulse()
								buttondialognext.set_sensitive(False)
							if (str(fileentry.get_text().find("/"))+""!="-1"):
								fileentry.set_progress_pulse_step(1)
								fileentry.progress_pulse()
								buttondialognext.set_sensitive(False)
							if (str(fileentryd.get_text().find("/"))+""!="-1"):
								fileentryd.set_progress_pulse_step(1)
								fileentryd.progress_pulse()
								buttondialognext.set_sensitive(False)
							multiplefilesavedialoginternal="/ssspdfjhhvgfjkdbfyhzszsfygujfbhyeurrfgvuekfafrgruyrfruyraewolufgeyrofrlewygarfaulg:::..."+str(fileentry.get_text())+"/---internal-python-variable-separator---gghjfvhvfdvbdfhhbvdbhfdbvfhbdfvhbvdfjhfvdhdf---"+str(fileentryd.get_text())
						fileentry.connect("changed", updatesavemultipleinternalvalues)
						fileentryd.connect("changed", updatesavemultipleinternalvalues)
						self.show_all()
					def on_response(self, dialog, response):
						if response == Gtk.ResponseType.OK:
							dialog.destroy()
							save_multiple_a(self,"Save single spectrum and peaks - Select folder")
						if response == Gtk.ResponseType.CANCEL:
							dialog.destroy()
				dialog = Dialog()
				dialog.run()
			def save_multiple_a(dgggf,title_save_dialog):
				global multiplefilesavedialoginternal
				class Dialog(Gtk.Dialog):
					global multiplefilesavedialoginternal
					def __init__(self):
						global multiplefilesavedialoginternal
						Gtk.Dialog.__init__(self)
						dialogb = Gtk.FileChooserDialog(""+title_save_dialog+"", self,Gtk.FileChooserAction.SELECT_FOLDER,(Gtk.STOCK_CANCEL, Gtk.ResponseType.CANCEL,Gtk.STOCK_SAVE, Gtk.ResponseType.OK))
						response = dialogb.run()
						if response == Gtk.ResponseType.OK:
							savecommand.value=multiplefilesavedialoginternal+"/mbghjdghdhvbsbhdbhdgfbhdhdghbvhdvadflbvdfhbhgf:::..."+dialogb.get_filename()
						dialogb.destroy()
						dialog.destroy()
				dialog = Dialog()
				dialog.run()
			def opendialog(dhhgfhdkgfg):
				class Dialog(Gtk.Dialog):
					def __init__(self):
						Gtk.Dialog.__init__(self)
						self.set_title("Help")
						self.set_default_size(655, 300)
						self.set_resizable(False)
						self.add_button("_OK", Gtk.ResponseType.OK)
						self.connect("response", self.on_response)
						hboxdialog = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=3)
						hboxdialogb = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=3)
						imagea=(Gtk.Image.new_from_icon_name("help-contents",Gtk.IconSize.DIALOG))
						imageb=(Gtk.Image.new_from_icon_name("go-previous",Gtk.IconSize.BUTTON))
						label= Gtk.Label("")
						labelb= Gtk.Label("")
						label.set_size_request(15,205)
						labelb.set_size_request(645,15)
						listbox = Gtk.ListBox()
						listbox.set_selection_mode(Gtk.SelectionMode.NONE)
						row = Gtk.ListBoxRow()
						row.get_style_context().remove_class(Gtk.STYLE_CLASS_LIST_ROW)
						row.set_activatable(False)
						hboxdialog.pack_start(imagea, False, False, 0)
						hboxdialog.pack_start(label, False, False, 0)
						hboxdialogb.pack_start(hboxdialog, False, False, 0)
						hboxdialogb.pack_start(labelb, False, False, 0)
						buttonback=Gtk.Button(label="Back")
						buttonback.set_always_show_image(True)
						buttonback.set_image(imageb)
						buttonback.get_style_context().add_class(Gtk.STYLE_CLASS_SUGGESTED_ACTION)
						buttonback.hide()
						hboxdialog.pack_start(buttonback, False, False, 0)
						scrolled_windowb = Gtk.ScrolledWindow()
						viewb = WebKit.WebView()
						hboxdialog.pack_start(scrolled_windowb, True, True, 0)
						scrolled_windowb.add(viewb)
						viewb.open("file://"+os.path.dirname(os.path.abspath(__file__))+"/resources/help/help.html")
						viewb.show()
						settingsb = viewb.get_settings()
						settingsb.set_property('enable-default-context-menu', False)
						viewb.set_settings(settingsb)
						buttongfgfs = Gtk.Entry()
						styledgsdgs = buttongfgfs.get_style_context()
						hsdhdjdg = styledgsdgs.get_color(Gtk.StateFlags.NORMAL)
						def updategtkviewb(aaa,bbb,ccc):
							styledgsdgs = buttongfgfs.get_style_context()
							hsdhdjdg = styledgsdgs.get_color(Gtk.StateFlags.NORMAL)
							viewb.execute_script("var systemcolors='"+str("text-normal-color:"+str('#'+''.join(map(chr, (int(hsdhdjdg.red*255),int(hsdhdjdg.green*255),int(hsdhdjdg.blue*255)))).encode('hex')))+";"+"'")
							if(viewb.get_uri()+""!="file://"+os.path.dirname(os.path.abspath(__file__))+"/resources/help/help.html"):
								buttonback.show()
							else:
								buttonback.hide()
						def goback(aaaa):
							viewb.go_back()
						viewb.connect('title-changed', updategtkviewb)
						buttonback.connect('clicked', goback)
						viewb.execute_script("var systemcolors='"+str("text-normal-color:"+str('#'+''.join(map(chr, (int(hsdhdjdg.red*255),int(hsdhdjdg.green*255),int(hsdhdjdg.blue*255)))).encode('hex')))+";"+"'")
						def installTransparency(self, component):
							component.set_visual(component.get_screen().get_rgba_visual())
						viewb.set_transparent(True)
						installTransparency(self,viewb)
						self.vbox.add(listbox)
						row.add(hboxdialogb)
						listbox.add(row)
						self.show_all()
						buttonback.hide()
					def on_response(self, dialog, response):
						dialog.destroy()
				dialog = Dialog()
				dialog.run()
			def save_multiple_spectra(dhhgfhdkgfg):
				global multiplefilesavedialoginternal
				class Dialog(Gtk.Dialog):
					global multiplefilesavedialoginternal
					def __init__(self):
						global multiplefilesavedialoginternal
						Gtk.Dialog.__init__(self)
						directory = os.path.dirname(os.path.expanduser("~")+"/.spectraread/")
						if not os.path.exists(directory):
							os.makedirs(directory)
						try:
							configfile = shelve.open(os.path.expanduser("~")+"/.spectraread/spectrareaduisettings-multiplespectra")
						except:
							try:
								os.remove(os.path.expanduser("~")+"/.spectraread/spectrareaduisettings-multiplespectra.dat")
							except:
								print("")
							try:
								os.remove(os.path.expanduser("~")+"/.spectraread/spectrareaduisettings-multiplespectra.dir")
							except:
								print("")
							try:
								os.remove(os.path.expanduser("~")+"/.spectraread/spectrareaduisettings-multiplespectra.bak")
							except:
								print("")
							configfile = shelve.open(os.path.expanduser("~")+"/.spectraread/spectrareaduisettings-multiplespectra")
						self.set_title("Save multiple spectra")
						self.set_default_size(500, 150)
						self.set_resizable(False)
						self.add_button("Cancel", Gtk.ResponseType.CANCEL)
						buttondialognext=self.add_button("Next", Gtk.ResponseType.OK)
						buttondialognext.set_sensitive(False)
						self.connect("response", self.on_response)
						hboxdialog = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=3)
						hboxdialogb = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=3)
						imagea=(Gtk.Image.new_from_icon_name("document-save",Gtk.IconSize.DIALOG))
						hboxdialogintb = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=3)
						label_save_a = Gtk.Label("Save spectrum every:")
						adjustment_botton_b_dialog = Gtk.Adjustment(value=50,lower=50,upper=1000000,step_increment=1,page_increment=1)
						entry_botton_b_dialog = Gtk.SpinButton(adjustment=adjustment_botton_b_dialog, digits=0)
						entry_botton_b_dialog.set_numeric(True)
						entry_botton_b_dialog.set_width_chars(7)
						label_save_int_a = Gtk.Label("ms")
						label_save_int_b = Gtk.Label("s")
						hboxdialoginta = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=3)
						label_save_b = Gtk.Label("Save spectrum for:     ")
						label_save_b2 = Gtk.Label("(0 means until continuous mode is disabled)                                                                                       ")
						adjustment_botton_a_dialog = Gtk.Adjustment(value=5,lower=0,upper=10000,step_increment=0.1,page_increment=0.1)
						entry_botton_a_dialog = Gtk.SpinButton(adjustment=adjustment_botton_a_dialog, digits=1)
						entry_botton_a_dialog.set_numeric(True)
						entry_botton_a_dialog.set_width_chars(7)
						hboxdialog.pack_start(imagea, False, False, 0)
						hboxdialog.pack_start(hboxdialogb, False, False, 0)
						hboxdialogintb.pack_start(label_save_a, False, False, 0)
						hboxdialogintb.pack_start(entry_botton_b_dialog, False, False, 0)
						hboxdialogintb.pack_start(label_save_int_a, False, False, 0)
						hboxdialoginta.pack_start(label_save_b, False, False, 0)
						hboxdialoginta.pack_start(entry_botton_a_dialog, False, False, 0)
						hboxdialoginta.pack_start(label_save_int_b, False, False, 0)
						hboxdialogb.pack_start(hboxdialogintb, False, False, 0)
						hboxdialogb.pack_start(hboxdialoginta, False, False, 0)
						hboxdialogb.pack_start(label_save_b2, False, False, 0)
						hboxdialogintc = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=0)
						hboxdialogb.pack_start(hboxdialogintc, False, False, 0)
						labelfilename = Gtk.Label("\nSpectrum file name:    ")
						fileentry=Gtk.Entry();
						fileentryb=Gtk.Entry();
						fileentryb.set_text("-0000.dat")
						fileentry.set_progress_pulse_step(1)
						fileentry.progress_pulse()
						fileentryb.set_editable(False)
						vsettings=""
						try:
							vsettings=float(configfile['1'])
						except:
							configfile['1']=""
						if(str(vsettings)+""!=""):
							entry_botton_a_dialog.set_value(float(vsettings))
						vsettings=""
						try:
							vsettings=float(configfile['2'])
						except:
							configfile['2']=""
						if(str(vsettings)+""!=""):
							entry_botton_b_dialog.set_value(float(vsettings))
						fileentryb.set_state_flags(Gtk.StateFlags.INSENSITIVE, True)
						hboxdialogintc.pack_start(labelfilename, False, False, 0)
						hboxdialogintc.pack_start(fileentry, False, False, 0)
						hboxdialogintc.pack_start(fileentryb, False, False, 0)
						self.vbox.add(hboxdialog)
						multiplefilesavedialoginternal="/smsbgjbvgfdsubsgbhjgsbbgdvhbvdgfsvbhgfdg:::..."+str(entry_botton_a_dialog.get_value())+"/---internal-python-variable-separator---gghjfvhvfdvbdfhhbvdbhfdbvfhbdfvhbvdfjhfvdhdf---"+str(entry_botton_b_dialog.get_value())+"/---internal-python-variable-separator---gghjfvhvfdvbdfhhbvdbhfdbvfhbdfvhbvdfjhfvdhdf---"+str(fileentry.get_text())
						def updatesavemultipleinternalvalues(bhgdsgydsygybgyuhrghsjhf):
							global multiplefilesavedialoginternal
							configfile['1']=float(entry_botton_a_dialog.get_value())
							configfile['2']=float(entry_botton_b_dialog.get_value())
							if (fileentry.get_text()+""!=""):
								fileentry.set_progress_pulse_step(0)
								fileentry.progress_pulse()
								buttondialognext.set_sensitive(True)
							if (fileentry.get_text()+""==""):
								fileentry.set_progress_pulse_step(1)
								fileentry.progress_pulse()
								buttondialognext.set_sensitive(False)
							if (str(fileentry.get_text().find("/"))+""!="-1"):
								fileentry.set_progress_pulse_step(1)
								fileentry.progress_pulse()
								buttondialognext.set_sensitive(False)
							multiplefilesavedialoginternal="/smsbgjbvgfdsubsgbhjgsbbgdvhbvdgfsvbhgfdg:::..."+str(entry_botton_a_dialog.get_value())+"/---internal-python-variable-separator---gghjfvhvfdvbdfhhbvdbhfdbvfhbdfvhbvdfjhfvdhdf---"+str(entry_botton_b_dialog.get_value())+"/---internal-python-variable-separator---gghjfvhvfdvbdfhhbvdbhfdbvfhbdfvhbvdfjhfvdhdf---"+str(fileentry.get_text())
						entry_botton_a_dialog.connect("changed", updatesavemultipleinternalvalues)
						entry_botton_a_dialog.connect("value-changed", updatesavemultipleinternalvalues)
						fileentry.connect("changed", updatesavemultipleinternalvalues)
						entry_botton_b_dialog.connect("changed", updatesavemultipleinternalvalues)
						entry_botton_b_dialog.connect("value-changed", updatesavemultipleinternalvalues)
						self.show_all()
					def on_response(self, dialog, response):
						if response == Gtk.ResponseType.OK:
							dialog.destroy()
							save_multiple_a(self,"Save multiple spectra - Select folder")
						if response == Gtk.ResponseType.CANCEL:
							dialog.destroy()
				dialog = Dialog()
				dialog.run()
			def save_multiple_peaks(dhhgfhdkgfg):
				global multiplefilesavedialoginternal
				class Dialog(Gtk.Dialog):
					global multiplefilesavedialoginternal
					def __init__(self):
						global multiplefilesavedialoginternal
						directory = os.path.dirname(os.path.expanduser("~")+"/.spectraread/")
						if not os.path.exists(directory):
							os.makedirs(directory)
						try:
							configfile = shelve.open(os.path.expanduser("~")+"/.spectraread/spectrareaduisettings-multiplepeaks")
						except:
							try:
								os.remove(os.path.expanduser("~")+"/.spectraread/spectrareaduisettings-multiplepeaks.dat")
							except:
								print("")
							try:
								os.remove(os.path.expanduser("~")+"/.spectraread/spectrareaduisettings-multiplepeaks.dir")
							except:
								print("")
							try:
								os.remove(os.path.expanduser("~")+"/.spectraread/spectrareaduisettings-multiplepeaks.bak")
							except:
								print("")
							configfile = shelve.open(os.path.expanduser("~")+"/.spectraread/spectrareaduisettings-multiplepeaks")
						Gtk.Dialog.__init__(self)
						self.set_title("Save multiple peaks")
						self.set_default_size(500, 150)
						self.set_resizable(False)
						self.add_button("Cancel", Gtk.ResponseType.CANCEL)
						buttondialognext=self.add_button("Next", Gtk.ResponseType.OK)
						buttondialognext.set_sensitive(False)
						self.connect("response", self.on_response)
						hboxdialog = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=3)
						hboxdialogb = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=3)
						imagea=(Gtk.Image.new_from_icon_name("document-save",Gtk.IconSize.DIALOG))
						hboxdialogintb = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=3)
						label_save_a = Gtk.Label("Save peaks every:   ")
						adjustment_botton_b_dialog = Gtk.Adjustment(value=50,lower=50,upper=1000000,step_increment=1,page_increment=1)
						entry_botton_b_dialog = Gtk.SpinButton(adjustment=adjustment_botton_b_dialog, digits=0)
						entry_botton_b_dialog.set_numeric(True)
						entry_botton_b_dialog.set_width_chars(7)
						label_save_int_a = Gtk.Label("ms")
						label_save_int_b = Gtk.Label("s")
						hboxdialoginta = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=3)
						label_save_b = Gtk.Label("Save peaks for:        ")
						label_save_b2 = Gtk.Label("(0 means until continuous mode is disabled)                                                                                       ")
						adjustment_botton_a_dialog = Gtk.Adjustment(value=5,lower=0,upper=10000,step_increment=0.1,page_increment=0.1)
						entry_botton_a_dialog = Gtk.SpinButton(adjustment=adjustment_botton_a_dialog, digits=1)
						entry_botton_a_dialog.set_numeric(True)
						entry_botton_a_dialog.set_width_chars(7)
						hboxdialog.pack_start(imagea, False, False, 0)
						hboxdialog.pack_start(hboxdialogb, False, False, 0)
						#hboxdialogintb.pack_start(label_save_a, False, False, 0)
						#hboxdialogintb.pack_start(entry_botton_b_dialog, False, False, 0)
						#hboxdialogintb.pack_start(label_save_int_a, False, False, 0)
						hboxdialoginta.pack_start(label_save_b, False, False, 0)
						hboxdialoginta.pack_start(entry_botton_a_dialog, False, False, 0)
						hboxdialoginta.pack_start(label_save_int_b, False, False, 0)
						hboxdialogb.pack_start(hboxdialogintb, False, False, 0)
						hboxdialogb.pack_start(hboxdialoginta, False, False, 0)
						hboxdialogb.pack_start(label_save_b2, False, False, 0)
						hboxdialogintc = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=0)
						hboxdialogb.pack_start(hboxdialogintc, False, False, 0)
						labelfilename = Gtk.Label("\nPeaks file name:       ")
						fileentry=Gtk.Entry();
						fileentryb=Gtk.Entry();
						fileentryb.set_text(".dat")
						fileentry.set_progress_pulse_step(1)
						fileentry.progress_pulse()
						fileentryb.set_editable(False)
						fileentryb.set_state_flags(Gtk.StateFlags.INSENSITIVE, True)
						hboxdialogintc.pack_start(labelfilename, False, False, 0)
						hboxdialogintc.pack_start(fileentry, False, False, 0)
						hboxdialogintc.pack_start(fileentryb, False, False, 0)
						self.vbox.add(hboxdialog)
						vsettings=""
						try:
							vsettings=float(configfile['1'])
						except:
							configfile['1']=""
						if(str(vsettings)+""!=""):
							entry_botton_a_dialog.set_value(float(vsettings))
						vsettings=""
						try:
							vsettings=float(configfile['2'])
						except:
							configfile['2']=""
						if(str(vsettings)+""!=""):
							entry_botton_b_dialog.set_value(float(vsettings))
						multiplefilesavedialoginternal="/smpgdkhgdfkhgfkjhkjuhkghdghjgvjfbgjfsfsbhbhfjkaogirhgdshgfgjasdgffagh:::..."+str(entry_botton_a_dialog.get_value())+"/---internal-python-variable-separator---gghjfvhvfdvbdfhhbvdbhfdbvfhbdfvhbvdfjhfvdhdf---"+str(entry_botton_b_dialog.get_value())+"/---internal-python-variable-separator---gghjfvhvfdvbdfhhbvdbhfdbvfhbdfvhbvdfjhfvdhdf---"+str(fileentry.get_text())
						def updatesavemultipleinternalvalues(bhgdsgydsygybgyuhrghsjhf):
							global multiplefilesavedialoginternal
							configfile['1']=float(entry_botton_a_dialog.get_value())
							configfile['2']=float(entry_botton_b_dialog.get_value())
							if (fileentry.get_text()+""!=""):
								fileentry.set_progress_pulse_step(0)
								fileentry.progress_pulse()
								buttondialognext.set_sensitive(True)
							if (fileentry.get_text()+""==""):
								fileentry.set_progress_pulse_step(1)
								fileentry.progress_pulse()
								buttondialognext.set_sensitive(False)
							if (str(fileentry.get_text().find("/"))+""!="-1"):
								fileentry.set_progress_pulse_step(1)
								fileentry.progress_pulse()
								buttondialognext.set_sensitive(False)
							multiplefilesavedialoginternal="/smpgdkhgdfkhgfkjhkjuhkghdghjgvjfbgjfsfsbhbhfjkaogirhgdshgfgjasdgffagh:::..."+str(entry_botton_a_dialog.get_value())+"/---internal-python-variable-separator---gghjfvhvfdvbdfhhbvdbhfdbvfhbdfvhbvdfjhfvdhdf---"+str(entry_botton_b_dialog.get_value())+"/---internal-python-variable-separator---gghjfvhvfdvbdfhhbvdbhfdbvfhbdfvhbvdfjhfvdhdf---"+str(fileentry.get_text())
						entry_botton_a_dialog.connect("changed", updatesavemultipleinternalvalues)
						entry_botton_a_dialog.connect("value-changed", updatesavemultipleinternalvalues)
						fileentry.connect("changed", updatesavemultipleinternalvalues)
						entry_botton_b_dialog.connect("changed", updatesavemultipleinternalvalues)
						entry_botton_b_dialog.connect("value-changed", updatesavemultipleinternalvalues)
						self.show_all()
					def on_response(self, dialog, response):
						if response == Gtk.ResponseType.OK:
							dialog.destroy()
							save_multiple_a(self,"Save multiple peaks - Select folder")
						if response == Gtk.ResponseType.CANCEL:
							dialog.destroy()
				dialog = Dialog()
				dialog.run()
			def save_multiple_spectra_peaks(dhhgfhdkgfg):
				global multiplefilesavedialoginternal
				class Dialog(Gtk.Dialog):
					global multiplefilesavedialoginternal
					def __init__(self):
						global multiplefilesavedialoginternal
						directory = os.path.dirname(os.path.expanduser("~")+"/.spectraread/")
						if not os.path.exists(directory):
							os.makedirs(directory)
						try:
							configfile = shelve.open(os.path.expanduser("~")+"/.spectraread/spectrareaduisettings-multiplespectrapeaks")
						except:
							try:
								os.remove(os.path.expanduser("~")+"/.spectraread/spectrareaduisettings-multiplespectrapeaks.dat")
							except:
								print("")
							try:
								os.remove(os.path.expanduser("~")+"/.spectraread/spectrareaduisettings-multiplespectrapeaks.dir")
							except:
								print("")
							try:
								os.remove(os.path.expanduser("~")+"/.spectraread/spectrareaduisettings-multiplespectrapeaks.bak")
							except:
								print("")
							configfile = shelve.open(os.path.expanduser("~")+"/.spectraread/spectrareaduisettings-multiplespectrapeaks")
						Gtk.Dialog.__init__(self)
						self.set_title("Save multiple spectra and peaks")
						self.set_default_size(500, 150)
						self.set_resizable(False)
						self.add_button("Cancel", Gtk.ResponseType.CANCEL)
						buttondialognext=self.add_button("Next", Gtk.ResponseType.OK)
						buttondialognext.set_sensitive(False)
						self.connect("response", self.on_response)
						hboxdialog = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=3)
						hboxdialogb = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=3)
						imagea=(Gtk.Image.new_from_icon_name("document-save",Gtk.IconSize.DIALOG))
						hboxdialogintb = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=3)
						label_save_a = Gtk.Label("Save spectra every: ")
						adjustment_botton_b_dialog = Gtk.Adjustment(value=50,lower=50,upper=1000000,step_increment=1,page_increment=1)
						entry_botton_b_dialog = Gtk.SpinButton(adjustment=adjustment_botton_b_dialog, digits=0)#
						entry_botton_b_dialog.set_numeric(True)
						entry_botton_b_dialog.set_width_chars(7)
						label_save_int_a = Gtk.Label("ms")
						label_save_int_b = Gtk.Label("s")
						hboxdialoginta = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=3)
						label_save_b = Gtk.Label("Save for:                      ")
						label_save_b2 = Gtk.Label("(0 means until continuous mode is disabled)                                                                                       ")
						adjustment_botton_a_dialog = Gtk.Adjustment(value=5,lower=0,upper=10000,step_increment=0.1,page_increment=0.1)
						entry_botton_a_dialog = Gtk.SpinButton(adjustment=adjustment_botton_a_dialog, digits=1)#
						entry_botton_a_dialog.set_numeric(True)
						entry_botton_a_dialog.set_width_chars(7)
						hboxdialog.pack_start(imagea, False, False, 0)
						hboxdialog.pack_start(hboxdialogb, False, False, 0)
						hboxdialogintb.pack_start(label_save_a, False, False, 0)
						hboxdialogintb.pack_start(entry_botton_b_dialog, False, False, 0)
						hboxdialogintb.pack_start(label_save_int_a, False, False, 0)
						hboxdialoginta.pack_start(label_save_b, False, False, 0)
						hboxdialoginta.pack_start(entry_botton_a_dialog, False, False, 0)
						hboxdialoginta.pack_start(label_save_int_b, False, False, 0)
						hboxdialogb.pack_start(hboxdialogintb, False, False, 0)
						hboxdialogb.pack_start(hboxdialoginta, False, False, 0)
						hboxdialogb.pack_start(label_save_b2, False, False, 0)
						hboxdialogintc = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=0)
						hboxdialogb.pack_start(hboxdialogintc, False, False, 0)
						labelfilename = Gtk.Label("\nPeaks file name:       ")
						fileentry=Gtk.Entry();
						fileentryb=Gtk.Entry();
						fileentryb.set_text(".dat")
						fileentry.set_progress_pulse_step(1)
						fileentry.progress_pulse()
						fileentryb.set_editable(False)
						fileentryb.set_state_flags(Gtk.StateFlags.INSENSITIVE, True)
						hboxdialogintd = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=0)
						hboxdialogb.pack_start(hboxdialogintd, False, False, 0)
						labelfilenamed = Gtk.Label("\nSpectra file name:    ")
						fileentryd=Gtk.Entry();
						fileentrybd=Gtk.Entry();
						fileentrybd.set_text("-0000.dat")
						fileentryd.set_progress_pulse_step(1)
						fileentryd.progress_pulse()
						fileentrybd.set_editable(False)
						fileentrybd.set_state_flags(Gtk.StateFlags.INSENSITIVE, True)
						hboxdialogintd.pack_start(labelfilenamed, False, False, 0)
						hboxdialogintd.pack_start(fileentryd, False, False, 0)
						hboxdialogintd.pack_start(fileentrybd, False, False, 0)
						hboxdialogintc.pack_start(labelfilename, False, False, 0)
						hboxdialogintc.pack_start(fileentry, False, False, 0)
						hboxdialogintc.pack_start(fileentryb, False, False, 0)
						self.vbox.add(hboxdialog)
						vsettings=""
						try:
							vsettings=float(configfile['1'])
						except:
							configfile['1']=""
						if(str(vsettings)+""!=""):
							entry_botton_a_dialog.set_value(float(vsettings))
						vsettings=""
						try:
							vsettings=float(configfile['2'])
						except:
							configfile['2']=""
						if(str(vsettings)+""!=""):
							entry_botton_b_dialog.set_value(float(vsettings))
						multiplefilesavedialoginternal="/smsphjdfghfjgshhfuygbhguuygyhguybhgyuygduygsyugrfvuyrkauesgfkuaegyuegarhfaelgjaewgfrgyg:::..."+str(entry_botton_a_dialog.get_value())+"/---internal-python-variable-separator---gghjfvhvfdvbdfhhbvdbhfdbvfhbdfvhbvdfjhfvdhdf---"+str(entry_botton_b_dialog.get_value())+"/---internal-python-variable-separator---gghjfvhvfdvbdfhhbvdbhfdbvfhbdfvhbvdfjhfvdhdf---"+str(fileentry.get_text())+"/---internal-python-variable-separator---gghjfvhvfdvbdfhhbvdbhfdbvfhbdfvhbvdfjhfvdhdf---"+str(fileentryd.get_text())
						def updatesavemultipleinternalvalues(bhgdsgydsygybgyuhrghsjhf):
							global multiplefilesavedialoginternal
							configfile['1']=float(entry_botton_a_dialog.get_value())
							configfile['2']=float(entry_botton_b_dialog.get_value())
							if (fileentry.get_text()+""!=""):
								fileentry.set_progress_pulse_step(0)
								fileentry.progress_pulse()
							if (fileentry.get_text()+""==""):
								fileentry.set_progress_pulse_step(1)
								fileentry.progress_pulse()
								buttondialognext.set_sensitive(False)
							if (fileentryd.get_text()+""!=""):
								fileentryd.set_progress_pulse_step(0)
								fileentryd.progress_pulse()
							if (fileentryd.get_text()+""==""):
								fileentryd.set_progress_pulse_step(1)
								fileentryd.progress_pulse()
								buttondialognext.set_sensitive(False)
							if (fileentryd.get_text()+""!=""):
								if (fileentry.get_text()+""!=""):
									if (fileentry.get_text()+""!=fileentryd.get_text()+""):
										buttondialognext.set_sensitive(True)
							if (fileentry.get_text()+""==fileentryd.get_text()+""):
								fileentryd.set_progress_pulse_step(1)
								fileentryd.progress_pulse()
								buttondialognext.set_sensitive(False)
							if (str(fileentry.get_text().find("/"))+""!="-1"):
								fileentry.set_progress_pulse_step(1)
								fileentry.progress_pulse()
								buttondialognext.set_sensitive(False)
							if (str(fileentryd.get_text().find("/"))+""!="-1"):
								fileentryd.set_progress_pulse_step(1)
								fileentryd.progress_pulse()
								buttondialognext.set_sensitive(False)
							multiplefilesavedialoginternal="/smsphjdfghfjgshhfuygbhguuygyhguybhgyuygduygsyugrfvuyrkauesgfkuaegyuegarhfaelgjaewgfrgyg:::..."+str(entry_botton_a_dialog.get_value())+"/---internal-python-variable-separator---gghjfvhvfdvbdfhhbvdbhfdbvfhbdfvhbvdfjhfvdhdf---"+str(entry_botton_b_dialog.get_value())+"/---internal-python-variable-separator---gghjfvhvfdvbdfhhbvdbhfdbvfhbdfvhbvdfjhfvdhdf---"+str(fileentry.get_text())+"/---internal-python-variable-separator---gghjfvhvfdvbdfhhbvdbhfdbvfhbdfvhbvdfjhfvdhdf---"+str(fileentryd.get_text())
						entry_botton_a_dialog.connect("changed", updatesavemultipleinternalvalues)
						entry_botton_a_dialog.connect("value-changed", updatesavemultipleinternalvalues)
						fileentry.connect("changed", updatesavemultipleinternalvalues)
						fileentryd.connect("changed", updatesavemultipleinternalvalues)
						entry_botton_b_dialog.connect("changed", updatesavemultipleinternalvalues)
						entry_botton_b_dialog.connect("value-changed", updatesavemultipleinternalvalues)
						self.show_all()
					def on_response(self, dialog, response):
						if response == Gtk.ResponseType.OK:
							dialog.destroy()
							save_multiple_a(self,"Save multiple spectra and peaks - Select folder")
						if response == Gtk.ResponseType.CANCEL:
							dialog.destroy()
				dialog = Dialog()
				dialog.run()
			def opendialogb(dhhgfhdkgfg):
				class Dialog(Gtk.Dialog):
					def __init__(self):
						Gtk.Dialog.__init__(self)
						self.set_title("About")
						self.set_default_size(655, 300)
						self.set_resizable(False)
						self.add_button("_OK", Gtk.ResponseType.OK)
						self.connect("response", self.on_response)
						hboxdialog = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=3)
						hboxdialogb = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=3)
						imagea=(Gtk.Image.new_from_icon_name("dialog-information",Gtk.IconSize.DIALOG))
						imageb=(Gtk.Image.new_from_icon_name("go-previous",Gtk.IconSize.BUTTON))
						label= Gtk.Label("")
						labelb= Gtk.Label("")
						label.set_size_request(15,205)
						labelb.set_size_request(645,15)
						listbox = Gtk.ListBox()
						listbox.set_selection_mode(Gtk.SelectionMode.NONE)
						row = Gtk.ListBoxRow()
						row.get_style_context().remove_class(Gtk.STYLE_CLASS_LIST_ROW)
						row.set_activatable(False)
						hboxdialog.pack_start(imagea, False, False, 0)
						hboxdialog.pack_start(label, False, False, 0)
						hboxdialogb.pack_start(hboxdialog, False, False, 0)
						hboxdialogb.pack_start(labelb, False, False, 0)
						buttonback=Gtk.Button(label="Back")
						buttonback.set_always_show_image(True)
						buttonback.set_image(imageb)
						buttonback.get_style_context().add_class(Gtk.STYLE_CLASS_SUGGESTED_ACTION)
						buttonback.hide()
						hboxdialog.pack_start(buttonback, False, False, 0)
						scrolled_windowb = Gtk.ScrolledWindow()
						viewb = WebKit.WebView()
						hboxdialog.pack_start(scrolled_windowb, True, True, 0)
						scrolled_windowb.add(viewb)
						viewb.open("file://"+os.path.dirname(os.path.abspath(__file__))+"/resources/about/about.html")
						viewb.show()
						settingsb = viewb.get_settings()
						settingsb.set_property('enable-default-context-menu', False)
						viewb.set_settings(settingsb)
						buttongfgfs = Gtk.Entry()
						styledgsdgs = buttongfgfs.get_style_context()
						hsdhdjdg = styledgsdgs.get_color(Gtk.StateFlags.NORMAL)
						def updategtkviewb(aaa,bbb,ccc):
							styledgsdgs = buttongfgfs.get_style_context()
							hsdhdjdg = styledgsdgs.get_color(Gtk.StateFlags.NORMAL)
							viewb.execute_script("var systemcolors='"+str("text-normal-color:"+str('#'+''.join(map(chr, (int(hsdhdjdg.red*255),int(hsdhdjdg.green*255),int(hsdhdjdg.blue*255)))).encode('hex')))+";"+"'")
							if(viewb.get_uri()+""!="file://"+os.path.dirname(os.path.abspath(__file__))+"/resources/about/about.html"):
								buttonback.show()
							else:
								buttonback.hide()
						def goback(aaaa):
							viewb.go_back()
						viewb.connect('title-changed', updategtkviewb)
						buttonback.connect('clicked', goback)
						viewb.execute_script("var systemcolors='"+str("text-normal-color:"+str('#'+''.join(map(chr, (int(hsdhdjdg.red*255),int(hsdhdjdg.green*255),int(hsdhdjdg.blue*255)))).encode('hex')))+";"+"'")
						def installTransparency(self, component):
							component.set_visual(component.get_screen().get_rgba_visual())
						viewb.set_transparent(True)
						installTransparency(self,viewb)
						self.vbox.add(listbox)
						row.add(hboxdialogb)
						listbox.add(row)
						self.show_all()
						buttonback.hide()
					def on_response(self, dialog, response):
						dialog.destroy()
				dialog = Dialog()
				dialog.run()
			global returna
			global currentdevice
			global removedselectitem
			removedselectitem=0
			loaded_spectrometer_modules=[None] * 1000000
			loaded_spectrometer_names=[None] * 1000000
			import_counter=0
			spectrometer_modules_dir = "spectrometer_modules"
			for child in os.listdir(spectrometer_modules_dir):
				current_spectrometer_module = os.path.join(spectrometer_modules_dir, child)
				if os.path.isdir(current_spectrometer_module):
					loaded_spectrometer_modules[import_counter] = __import__(""+current_spectrometer_module.split("/")[1]+".toolbar_options", fromlist=[''])
					loaded_spectrometer_names[import_counter] = ""+current_spectrometer_module.split("/")[1]
					import_counter=import_counter+1
			directory = os.path.dirname(os.path.expanduser("~")+"/.spectraread/")
			if not os.path.exists(directory):
				os.makedirs(directory)
			try:
				configfile = shelve.open(os.path.expanduser("~")+"/.spectraread/spectrareaduisettings")
			except:
				try:
					os.remove(os.path.expanduser("~")+"/.spectraread/spectrareaduisettings.dat")
				except:
					print("")
				try:
					os.remove(os.path.expanduser("~")+"/.spectraread/spectrareaduisettings.dir")
				except:
					print("")
				try:
					os.remove(os.path.expanduser("~")+"/.spectraread/spectrareaduisettings.bak")
				except:
					print("")
				configfile = shelve.open(os.path.expanduser("~")+"/.spectraread/spectrareaduisettings")
			dev = Gtk.ListStore(int, str)
			selectdeviceitem=dev.append([1, "---Select---"])
			internalcounterloadedmodulesincombobox=2
			deviceavaliableonloadcount=0
			for spectrometername in loaded_spectrometer_names:
				if (str(spectrometername)+""!="None"):
					dev.append([internalcounterloadedmodulesincombobox, ""+spectrometername])
					if(loaded_spectrometer_modules[internalcounterloadedmodulesincombobox-2].checkavaliable()==True):
						deviceavaliableonloadcount=deviceavaliableonloadcount+1
					internalcounterloadedmodulesincombobox=internalcounterloadedmodulesincombobox+1
			Gtk.Window.__init__(self,title="SpectraRead v1.3.5")
			self.set_size_request(900, 500)
			self.set_default_icon_from_file("resources/icon.svg")
			hbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=0)
			self.add(hbox)
			menubar = Gtk.MenuBar()
			menuitem = Gtk.MenuItem(label='File')
			menuitemhelp = Gtk.MenuItem(label='Help')
			submenubar = Gtk.Menu()
			menuitemb = Gtk.ImageMenuItem(Gtk.STOCK_SAVE,label='Save single')
			menuitemb.set_image(Gtk.Image.new_from_icon_name("document-save-as",Gtk.IconSize.BUTTON))
			menuitemb.set_always_show_image(True)
			menuitemc = Gtk.ImageMenuItem(Gtk.STOCK_SAVE,label='Save multiple')
			menuitemc.set_image(Gtk.Image.new_from_icon_name("document-save-as",Gtk.IconSize.BUTTON))
			menuitemc.set_always_show_image(True)
			menuitemd = Gtk.ImageMenuItem(Gtk.STOCK_QUIT,label='Quit')
			menuitemd.set_image(Gtk.Image.new_from_icon_name("application-exit",Gtk.IconSize.BUTTON))
			menuitemd.set_always_show_image(True)
			menuitemc.set_sensitive(False)
			submenubar.append(menuitemb)
			submenubar.append(menuitemc)
			submenubar.append(menuitemd)
			menuitem.set_submenu(submenubar)
			menubar.append(menuitem)
			menubar.append(menuitemhelp)
			submenubarhelp = Gtk.Menu()
			submenuitemhelpa = Gtk.ImageMenuItem(Gtk.STOCK_HELP,label='Help')
			submenuitemhelpa.set_image(Gtk.Image.new_from_icon_name("help-contents",Gtk.IconSize.BUTTON))
			submenuitemhelpa.set_always_show_image(True)
			submenuitemhelpb = Gtk.ImageMenuItem(Gtk.STOCK_ABOUT,label='About')
			submenuitemhelpb.set_image(Gtk.Image.new_from_icon_name("help-about",Gtk.IconSize.BUTTON))
			submenuitemhelpb.set_always_show_image(True)
			submenubarhelp.append(submenuitemhelpa)
			submenubarhelp.append(submenuitemhelpb)
			menuitemhelp.set_submenu(submenubarhelp)
			submenubarb = Gtk.Menu()
			submenuitemhelpa.connect("activate", opendialog)
			submenuitemhelpb.connect("activate", opendialogb)
			submenuitema = Gtk.MenuItem(label='Spectrum')
			submenuitemb = Gtk.MenuItem(label='Peaks')
			submenuitemc = Gtk.MenuItem(label='Spectrum and peaks')
			submenuitema.connect("activate", save_single_spectrum)
			submenuitemb.connect("activate", save_single_peaks)
			submenuitemc.connect("activate", save_single_spectrum_peaks)
			def closewindow(jyrryjryjeyjyj):
				windowclose.value="closed"
				self.destroy()
			menuitemd.connect("activate", closewindow)
			submenubarb.append(submenuitema)
			submenubarb.append(submenuitemb)
			submenubarb.append(submenuitemc)
			menuitemb.set_submenu(submenubarb)
			submenubarc = Gtk.Menu()
			submenuitemab = Gtk.MenuItem(label='Spectra')
			submenuitembb = Gtk.MenuItem(label='Peaks')
			submenuitemcb = Gtk.MenuItem(label='Spectra and peaks')
			submenuitemab.connect("activate", save_multiple_spectra)
			submenuitembb.connect("activate", save_multiple_peaks)
			submenuitemcb.connect("activate", save_multiple_spectra_peaks)
			submenubarc.append(submenuitemab)
			submenubarc.append(submenuitembb)
			submenubarc.append(submenuitemcb)
			menuitemc.set_submenu(submenubarc)
			toolbar = Gtk.Toolbar()
			context = toolbar.get_style_context()
			context.add_class(Gtk.STYLE_CLASS_PRIMARY_TOOLBAR)
			tool_item = Gtk.ToolItem()
			buttont = Gtk.Button(label="Run once")
			tool_item.add(buttont)
			toolbar.insert(tool_item,0)
			tool_item_checkbox = Gtk.ToolItem()
			checkbox = Gtk.ToggleButton("Continuous", use_underline=False)
			tool_item_checkbox.add(checkbox)
			toolbar.insert(tool_item_checkbox,0)
			viewb = WebKit.WebView()
			currentdevice.additems(toolbar)
			hbox.pack_start(menubar, False, False, 0)
			hbox.pack_start(toolbar, False, False, 0)
			hboxb = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=6)
			hboxb.get_style_context().add_class(Gtk.STYLE_CLASS_LIST)
			#---
			listbox = Gtk.ListBox()
			listbox.set_selection_mode(Gtk.SelectionMode.NONE)
			#---
			hbox.pack_start(hboxb, True, True, 0)
			class CustomToolbar(NavigationToolbar):
				toolitems=((u'Pan', u'Pan axes with left mouse, zoom with right', u'move', u'pan'), (u'Zoom', u'Zoom to rectangle', u'zoom_to_rect', u'zoom'),  )
			def toogle(aaa,self,b1toogle,b2toogle,b3toogle,autoscaleval):
				if b1toogle.get_active()==True:
					if aaa==b1toogle:
						self.pan('off')
						self.zoom('off')
						autoscaleval[0]=1
						b2toogle.set_active(False)
						b3toogle.set_active(False)
				if b2toogle.get_active()==True:
					if aaa==b2toogle:
						self.zoom('off')
						self.pan('on')
						autoscaleval[0]=0
						b1toogle.set_active(False)
						b3toogle.set_active(False)
				if b3toogle.get_active()==True:
					if aaa==b3toogle:
						self.pan('off')
						self.zoom('on')
						autoscaleval[0]=0
						b1toogle.set_active(False)
						b2toogle.set_active(False)
				if b1toogle.get_active()==False:
					if b2toogle.get_active()==False:
						if b3toogle.get_active()==False:
							aaa.set_active(True)
			self.inmatplotlibbox = Gtk.Box(spacing=0, orientation=Gtk.Orientation.VERTICAL)
			self.inmatplotlibboxb = Gtk.Box(spacing=0, orientation=Gtk.Orientation.VERTICAL)
			self.inmatplotlibboxc = Gtk.Box(spacing=0, orientation=Gtk.Orientation.HORIZONTAL)
			self.inmatplotlibboxd = Gtk.Box(spacing=0, orientation=Gtk.Orientation.HORIZONTAL)
			self.inmatplotlibboxe = Gtk.Box(spacing=0, orientation=Gtk.Orientation.HORIZONTAL)
			self.inmatplotlibboxf = Gtk.Box(spacing=0, orientation=Gtk.Orientation.HORIZONTAL)
			self.inmatplotlibbox.get_style_context().add_class(Gtk.STYLE_CLASS_LIST)
			self.inmatplotlibboxb.get_style_context().add_class(Gtk.STYLE_CLASS_LIST)
			self.inmatplotlibboxc.get_style_context().add_class(Gtk.STYLE_CLASS_LIST)
			self.inmatplotlibboxd.get_style_context().add_class(Gtk.STYLE_CLASS_LIST)
			self.inmatplotlibboxe.get_style_context().add_class(Gtk.STYLE_CLASS_LIST)
			self.inmatplotlibboxf.get_style_context().add_class(Gtk.STYLE_CLASS_LIST)
			plt.rcParams['axes.linewidth'] = 2.1
			plt.rcParams['font.size'] = 55
			plt.rcParams['grid.linestyle'] = 'None'
			plt.rcParams['agg.path.chunksize'] = 10000000
			fig = Figure(figsize=(0.0000001,0.0000001), dpi=15)
			plt.locator_params(nbins=4)
			ax = fig.add_subplot(111)
			entrytextselectstyle = Gtk.ToggleButton()
			styletextselect = entrytextselectstyle.get_style_context()
			selectcolor=styletextselect.get_background_color(Gtk.StateFlags.ACTIVE).to_color().to_floats()
			ccb = ColorConverter()
			ccb.to_rgba(selectcolor)
			line,=ax.plot(np.arange(100),np.asarray([0]*100),label='',marker='o', markersize=15,linewidth=3,color=(selectcolor[0]/1.2,selectcolor[1]/1.2,selectcolor[2]/1.2))
			lineb,=ax.plot(np.arange(1),np.asarray([0]*1),label='',marker='o', markersize=25,linewidth=0,color=(selectcolor[0],selectcolor[1]/5.2,selectcolor[2]/5.2))
			ax.tick_params(axis='both', which='major', pad=15)
			mousepos=[0]*2
			mousepostext=[0]*2
			def format_coord(x, y):
				return ''
			def aaa(a):
				if("button=None" in str(a)):
					return False
				else:
					return True
			inside=[0]*2
			val=[0]*2
			def mousemoveevent(event):
				intpos=[0]*2
				valb=(str(event).split("xy=(")[1].split(")")[0].split(","))
				val[0]=valb[0]
				val[1]=valb[1]
				intpos[0]=float(ax.transData.inverted().transform([float(val[0]), float(val[1])])[0])
				intpos[1]=float(ax.transData.inverted().transform([float(val[0]), float(val[1])])[1])
				inside[0]=0
				mousepostext[0]=intpos[0]
				mousepostext[1]=intpos[1]
				if(intpos[0]-(((ax.axis())[1]-(ax.axis())[0])*9.3/100.0)>(ax.axis())[0]):
					if(intpos[0]+(9.0/100.0)*intpos[0]<(ax.axis())[1]):
						if(intpos[1]-(9.0/100.0)*intpos[1]>(ax.axis())[2]):
							if(intpos[1]+(((ax.axis())[3]-(ax.axis())[2])*9.3/100.0)<(ax.axis())[3]):
								mousepos[0]=float(ax.transData.inverted().transform([float(val[0]), float(val[1])])[0])
								mousepos[1]=float(ax.transData.inverted().transform([float(val[0]), float(val[1])])[1])
				if(intpos[0]>(ax.axis())[0]):
					if(intpos[0]<(ax.axis())[1]):
						if(intpos[1]>(ax.axis())[2]):
							if(intpos[1]<(ax.axis())[3]):
								mousepos[0]=float(ax.transData.inverted().transform([float(val[0]), float(val[1])])[0])
								mousepos[1]=float(ax.transData.inverted().transform([float(val[0]), float(val[1])])[1])
								inside[0]=1
			ax.format_coord = format_coord
			ax.in_axes=aaa
			ax.grid(False)
			style = Gtk.ListBoxRow().get_style_context()
			bg_color = style.lookup_color("theme_base_color").color.to_color().to_floats()
			cc = ColorConverter()
			cc.to_rgba(bg_color)
			fg_color = style.get_color(Gtk.StateType.NORMAL).to_color().to_floats()
			ccb = ColorConverter()
			ccb.to_rgba(fg_color)
			fig.patch.set_facecolor(bg_color)
			ax.patch.set_facecolor(bg_color)
			font = {'family' : 'normal',
					'weight' : 'bold',
					'size'   : 29}
			ax.spines['bottom'].set_color(fg_color)
			ax.spines['top'].set_color(fg_color) 
			ax.spines['right'].set_color(fg_color)
			ax.spines['left'].set_color(fg_color)
			ax.spines['right'].set_visible(False)
			ax.spines['top'].set_visible(False)
			ax.xaxis.set_ticks_position('bottom')
			ax.yaxis.set_ticks_position('left')
			ax.tick_params(axis='x', colors=fg_color)
			ax.tick_params(axis='y', colors=fg_color)
			ax.yaxis.label.set_color(fg_color)
			ax.xaxis.label.set_color(fg_color)
			ax.tick_params(
				axis='x',
				which='both',
				bottom=False,
				top=False,
				labelbottom=True)
			ax.tick_params(
				axis='y',
				which='both',
				left=False,
				right=False,
				labelleft=True)
			matplotlib.rc('font', **font)
			plt.rc('font', **font)
			canvas = FigureCanvas(fig)
			canvas.mpl_connect('motion_notify_event', mousemoveevent)
			canvas.set_size_request(200,200)
			inmatplotlibtoolbarb = CustomToolbar(canvas, self)
			matplotlibshowtoolbar=Gtk.Toolbar()
			btnresetmatplotlib=Gtk.ToggleButton()
			btnresetmatplotlib.set_image(Gtk.Image.new_from_icon_name("zoom-original",Gtk.IconSize.BUTTON))
			btnresetmatplotlib.set_property("tooltip-text", "Autoscale")
			btnresetitn=Gtk.ToolItem()
			btnresetitn.add(btnresetmatplotlib)
			btnpanmatplotlib=Gtk.ToggleButton()
			btnpanmatplotlib.set_image(Gtk.Image.new_from_icon_name("object-flip-horizontal",Gtk.IconSize.BUTTON))
			btnpanmatplotlib.set_property("tooltip-text", "Drag")
			btnresetitnb=Gtk.ToolItem()
			btnresetitnb.add(btnpanmatplotlib)
			btnzoommatplotlib=Gtk.ToggleButton()
			btnzoommatplotlib.set_image(Gtk.Image.new_from_icon_name("zoom-in",Gtk.IconSize.BUTTON))
			btnzoommatplotlib.set_property("tooltip-text", "Zoom")
			btnresetitnc=Gtk.ToolItem()
			btnresetitnc.add(btnzoommatplotlib)
			btnresetmatplotlib.set_active(True)
			autoscaleval=[1]*3
			btnresetmatplotlib.connect("toggled", toogle,inmatplotlibtoolbarb,btnresetmatplotlib,btnpanmatplotlib,btnzoommatplotlib,autoscaleval)
			btnpanmatplotlib.connect("toggled", toogle,inmatplotlibtoolbarb,btnresetmatplotlib,btnpanmatplotlib,btnzoommatplotlib,autoscaleval)
			btnzoommatplotlib.connect("toggled", toogle,inmatplotlibtoolbarb,btnresetmatplotlib,btnpanmatplotlib,btnzoommatplotlib,autoscaleval)
			matplotlibshowtoolbar.insert(btnresetitn,0)
			matplotlibshowtoolbar.insert(btnresetitnb,1)
			matplotlibshowtoolbar.insert(btnresetitnc,2)
			inmatplotlibsw = Gtk.ScrolledWindow()
			inmatplotlibsw.add_with_viewport(canvas)
			matplotlibshowtoolbar.set_orientation(Gtk.Orientation.VERTICAL)
			inmatplotlibtoolbarb.set_orientation(Gtk.Orientation.VERTICAL)
			self.inmatplotlibbox.pack_start(self.inmatplotlibboxb, False, True, 0)
			self.inmatplotlibbox.pack_start(self.inmatplotlibboxc, True, True, 0)
			self.inmatplotlibboxc.pack_start(self.inmatplotlibboxd, False, False, 0)
			self.inmatplotlibboxc.pack_start(self.inmatplotlibboxe, True, True, 0)
			self.inmatplotlibboxc.pack_start(self.inmatplotlibboxf, False, False, 0)
			self.inmatplotlibboxe.pack_start(inmatplotlibsw, True, True, 0)
			self.inmatplotlibboxf.pack_start(matplotlibshowtoolbar, False, True, 0)
			i=[0]*5
			winsize=[0]*3
			timecounterb=[0]*3
			hboxb.pack_start(self.inmatplotlibbox, True, True, 0)
			rect = patches.Rectangle((0.3,0.2),((ax.axis())[1]-(ax.axis())[0])*2.3/100.0,((ax.axis())[3]-(ax.axis())[2])*2.3/100.0,linewidth=0,facecolor=(selectcolor[0]/1.2,selectcolor[1]/1.2,selectcolor[2]/1.2))
			recttext=ax.text(0.3,0.2, "x,y=(0,0)", bbox=dict(facecolor='gray', alpha=0.5),fontsize=59)
			ax.add_patch(rect)
			rect.set_visible(False)
			recttext.set_visible(False)
			def desenhagrafico(i,autoscaleval,entrytextselectstyle,rect,mousepos,inside,recttext,mousepostext,winsize,timecounterb,infotext,checkbox,returnedfalse,oldmouseposloopcontrol,val,ax):
				if(str(self.get_size())==winsize[0]):
					if(1==1):
						timecounterb[0]=0
						if(timecounterb[0]<=0):
							updateddatatryexcept=1
							try:
								igsighgjihkurgrgf=(spectrum.value.split(" "))
								intcounterdatanp=0
								intveclist=[0]*(len(igsighgjihkurgrgf))
								for v in igsighgjihkurgrgf:
									intveclist[intcounterdatanp]=float(v)
									intcounterdatanp=intcounterdatanp+1
								igsighgjihkurgrgf=(spectrumx.value.split(" "))
								intcounterdatanp=0
								intveclistx=[0]*len(igsighgjihkurgrgf)
								for v in igsighgjihkurgrgf:
									intveclistx[intcounterdatanp]=float(v)
									intcounterdatanp=intcounterdatanp+1
							except:
								updateddatatryexcept=0
							if(len(spectrumx.value.split(" "))!=len(spectrum.value.split(" "))):
								updateddatatryexcept=0
							if(updateddatatryexcept==1):
								try:
									vpeaks=infotext.value.split("Peaks:[")[1].split("]")[0].split(" ")
									ipeaks=0
									vpeaksnum=[]
									for v in vpeaks:
										try:
											vpeaksnum.append(float(v))
										except:
											null=None
									ipeaks=ipeaks+1
									line.set_data(np.asarray(intveclistx), np.asarray(intveclist))
									match_y=[0]*(len(vpeaksnum))
									match=0
									ipeaksb=0
									ipeaksc=0
									for v in vpeaksnum:
										match=abs(intveclistx[0]-v)
										ipeaksb=0
										for v2 in intveclistx:
											if(abs(v-v2)<match):
												match=abs(v-v2)
												match_y[ipeaksc]=intveclist[ipeaksb]
											ipeaksb=ipeaksb+1
										ipeaksc=ipeaksc+1
									lineb.set_data(vpeaksnum, match_y)
								except:
									null=None
						if(inside[0]==0):
							rect.set_width(0)
							rect.set_height(0)
							rect.set_visible(False)
							recttext.set_visible(False)
						else:
							intpos=[0]*2
							intpos[0]=float(ax.transData.inverted().transform([float(val[0]), float(val[1])])[0])
							intpos[1]=float(ax.transData.inverted().transform([float(val[0]), float(val[1])])[1])
							mousepostext[0]=intpos[0]
							mousepostext[1]=intpos[1]
							if(intpos[0]-(((ax.axis())[1]-(ax.axis())[0])*9.3/100.0)>(ax.axis())[0]):
								if(intpos[0]+(9.0/100.0)*intpos[0]<(ax.axis())[1]):
									if(intpos[1]-(9.0/100.0)*intpos[1]>(ax.axis())[2]):
										if(intpos[1]+(((ax.axis())[3]-(ax.axis())[2])*9.3/100.0)<(ax.axis())[3]):
											mousepos[0]=float(ax.transData.inverted().transform([float(val[0]), float(val[1])])[0])
											mousepos[1]=float(ax.transData.inverted().transform([float(val[0]), float(val[1])])[1])
							if(intpos[0]>(ax.axis())[0]):
								if(intpos[0]<(ax.axis())[1]):
									if(intpos[1]>(ax.axis())[2]):
										if(intpos[1]<(ax.axis())[3]):
											mousepos[0]=float(ax.transData.inverted().transform([float(val[0]), float(val[1])])[0])
											mousepos[1]=float(ax.transData.inverted().transform([float(val[0]), float(val[1])])[1])
							#rect.set_visible(True)
							recttext.set_visible(True)
							#rect.set_width(((ax.axis())[1]-(ax.axis())[0])*2.3/100.0)
							#rect.set_height(((ax.axis())[3]-(ax.axis())[2])*2.3/100.0)
							#rect.set_xy((mousepos[0],mousepos[1]-(((ax.axis())[3]-(ax.axis())[2])*0.4/100.0)),)
							recttext.set_position((mousepos[0],mousepos[1]))
							#recttext.set_text("x,y=("+str(round(mousepostext[0]*100)/100)+","+str(round(mousepostext[1]*100)/100)+")")
						fig.canvas.draw()
						if autoscaleval[0]==1:
							ax.autoscale_view()
							ax.autoscale(True)
						ax.relim()
					if(time.time()-i[0]>0.3):
						style = hboxb.get_style_context()
						if(timecounterb[0]>=0):
							fg_color = style.get_color(Gtk.StateType.NORMAL).to_color().to_floats()
							ccb = ColorConverter()
							ccb.to_rgba(fg_color)
							styletextselect = entrytextselectstyle.get_style_context()
							selectcolor=styletextselect.get_background_color(Gtk.StateFlags.ACTIVE).to_color().to_floats()
							ccb = ColorConverter()
							ccb.to_rgba(selectcolor)
							if(inside[0]==0):
								rect.set_width(0)
								rect.set_height(0)
								rect.set_visible(False)
								recttext.set_visible(False)
							else:
								#rect.set_visible(True)
								recttext.set_visible(True)
								#rect.set_width(((ax.axis())[1]-(ax.axis())[0])*2.3/100.0)
								#rect.set_height(((ax.axis())[3]-(ax.axis())[2])*2.3/100.0)
								#rect.set_xy((mousepos[0],mousepos[1]-(((ax.axis())[3]-(ax.axis())[2])*0.4/100.0)),)
								#recttext.set_position((mousepos[0],mousepos[1]))
								recttext.set_text("x,y=("+str(round(mousepostext[0]*100)/100)+","+str(round(mousepostext[1]*100)/100)+")")
								recttext.set_color(fg_color)
								rect.set_color(selectcolor)
							if autoscaleval[0]==1:
								ax.autoscale_view()
								ax.autoscale(True)
							stylelistbox = Gtk.ListBoxRow().get_style_context()
							bg_color = stylelistbox.lookup_color("theme_base_color").color.to_color().to_floats()
							cc = ColorConverter()
							cc.to_rgba(bg_color)
							if(str(bg_color)!=i[1]):
								fig.patch.set_facecolor(bg_color)
								ax.patch.set_facecolor(bg_color)
								i[1]=str(bg_color)
							if(str(selectcolor)!=i[3]):
								line.set_color((selectcolor[0]/1.2,selectcolor[1]/1.2,selectcolor[2]/1.2))
								addcolor=0
								if(selectcolor[1]>0):
									lineb.set_color((selectcolor[0],selectcolor[1]/5.2,selectcolor[2]/5.2))
									addcolor=1
								if(selectcolor[2]>0):
									lineb.set_color((selectcolor[0],selectcolor[1]/5.2,selectcolor[2]/5.2))
									addcolor=1
								if(addcolor==0):
									if(selectcolor[0]<0.7):
										addcolor=0.3
									else:
										addcolor=-0.3
									lineb.set_color((selectcolor[0]+addcolor,selectcolor[1]/5.2,selectcolor[2]/5.2))
								i[3]=str(selectcolor)
							if(str(fg_color)!=i[2]):
								ax.spines['bottom'].set_color(fg_color)
								ax.spines['top'].set_color(fg_color)
								ax.spines['right'].set_color(fg_color)
								ax.spines['left'].set_color(fg_color)
								ax.tick_params(axis='x', colors=fg_color)
								ax.tick_params(axis='y', colors=fg_color)
								ax.yaxis.label.set_color(fg_color)
								ax.xaxis.label.set_color(fg_color)
								i[2]=str(fg_color)
							timecounterb[0]=0
						else:
							timecounterb[0]=timecounterb[0]+1
				else:
					i[0]=time.time()
				winsize[0]=str(self.get_size())
				#i[0]=i[0]+1
				#ax.draw_artist(line)
				#fig.canvas.blit(ax.bbox)
				returnedfalse[0]=1
				if(checkbox.get_active()==True):
					returnedfalse[0]=0
					return True
				if(oldmouseposloopcontrol[0]!=mousepos[0]):
					oldmouseposloopcontrol[0]=mousepos[0]
					returnedfalse[0]=0
					return True
				if(oldmouseposloopcontrol[1]!=mousepos[1]):
					oldmouseposloopcontrol[1]=mousepos[1]
					returnedfalse[0]=0
					return True
				return False
			returnedfalse=[1]*3
			oldmouseposloopcontrol=[0]*2
			def loopcontrol(i,autoscaleval,entrytextselectstyle,rect,mousepos,inside,recttext,mousepostext,winsize,timecounterb,infotext,checkbox,returnedfalse,oldmouseposloopcontrol,val,ax):
				#GObject.idle_add(desenhagrafico,i,autoscaleval,entrytextselectstyle,rect,mousepos,inside,recttext,mousepostext,winsize,timecounterb)
				if(returnedfalse[0]==1):
					GObject.timeout_add(50, desenhagrafico,i,autoscaleval,entrytextselectstyle,rect,mousepos,inside,recttext,mousepostext,winsize,timecounterb,infotext,checkbox,returnedfalse,oldmouseposloopcontrol,val,ax,priority=GObject.PRIORITY_DEFAULT_IDLE)
				return True
			GObject.timeout_add(350, loopcontrol,i,autoscaleval,entrytextselectstyle,rect,mousepos,inside,recttext,mousepostext,winsize,timecounterb,infotext,checkbox,returnedfalse,oldmouseposloopcontrol,val,ax,priority=GObject.PRIORITY_DEFAULT_IDLE)
			toolbarb = Gtk.Toolbar()
			context = toolbarb.get_style_context()
			context.add_class(Gtk.STYLE_CLASS_PRIMARY_TOOLBAR)
			tool_item_botton_a = Gtk.ToolItem()
			adjustment_botton_a = Gtk.Adjustment(value=2,lower=0,upper=10,step_increment=1,page_increment=1)
			entry_botton_a = Gtk.SpinButton(adjustment=adjustment_botton_a, digits=0)
			#Fix a bug in some GTK themes (For example Mint-Y) that causes the buttons to disappear (white background and text in white text entry) when the spin button has focus.
			entry_botton_a.get_style_context().remove_class(Gtk.STYLE_CLASS_SPINBUTTON)
			entry_botton_a.set_numeric(True)
			entry_botton_a.set_width_chars(3)
			tool_item_botton_a.add(entry_botton_a)
			entry_botton_peaks = Gtk.Label(infotext.value)
			def updategtk():
				peaksupdate=infotext.value.split("Peaks:[")[1].split("]")[0].split(" ")
				peaksupdatestring=""
				peaksupdatetextcounter=0
				addeddotspeaksupdate=0
				for v in peaksupdate:
					if(peaksupdatetextcounter<5):
						peaksupdatestring=peaksupdatestring+v+"  "
					else:
						if(addeddotspeaksupdate==0):
							peaksupdatestring=peaksupdatestring+"..."
						addeddotspeaksupdate=1
					peaksupdatetextcounter=peaksupdatetextcounter+1
				peaksupdatestring=peaksupdatestring+"]"
				entry_botton_peaks.set_text("F"+(infotext.value.split("Peaks:[")[1].split("]")[1]).split("          F")[1]+" Peaks:["+peaksupdatestring+"")
				if(uncheck.value=="True"):
					uncheck.value="False"
					checkbox.set_active(False)
				return True
			GObject.timeout_add(100, updategtk)
			tool_item_botton_b = Gtk.ToolItem()
			adjustment_botton_b = Gtk.Adjustment(value=1,lower=0,upper=10,step_increment=1,page_increment=1)
			entry_botton_b = Gtk.SpinButton(adjustment=adjustment_botton_b, digits=0)
			#Fix a bug in some GTK themes (For example Mint-Y) that causes the buttons to disappear (white background and text in white text entry) when the spin button has focus.
			entry_botton_b.get_style_context().remove_class(Gtk.STYLE_CLASS_SPINBUTTON)
			entry_botton_b.set_numeric(True)
			entry_botton_b.set_width_chars(3)
			tool_item_botton_b.add(entry_botton_b)
			tool_item_botton_h = Gtk.ToolItem()
			adjustment_botton_h = Gtk.Adjustment(value=0,lower=0,upper=1,step_increment=0.01,page_increment=0.01)
			entry_botton_h = Gtk.SpinButton(adjustment=adjustment_botton_h, digits=2)
			#Fix a bug in some GTK themes (For example Mint-Y) that causes the buttons to disappear (white background and text in white text entry) when the spin button has focus.
			entry_botton_h.get_style_context().remove_class(Gtk.STYLE_CLASS_SPINBUTTON)
			entry_botton_h.set_numeric(True)
			entry_botton_h.set_width_chars(3)
			tool_item_botton_h.add(entry_botton_h)
			tool_item_botton_g = Gtk.ToolItem()
			dev_combo = Gtk.ComboBox.new_with_model(dev)
			renderer_textc = Gtk.CellRendererText()
			dev_combo.pack_start(renderer_textc, True)
			dev_combo.add_attribute(renderer_textc, "text", 1)
			dev_combo.set_active(0)
			tool_item_botton_g.add(dev_combo)
			tool_item_botton_c = Gtk.ToolItem()
			entry_botton_c = Gtk.Label("   Baseline:")
			tool_item_botton_c.add(entry_botton_c)
			tool_item_botton_d = Gtk.ToolItem()
			entry_botton_d = Gtk.Label("   Boxcar:")
			tool_item_botton_d.add(entry_botton_d)
			tool_item_botton_e = Gtk.ToolItem()
			entry_botton_e = Gtk.Label("   Threshold:")
			tool_item_botton_e.add(entry_botton_e)
			vsettings=""
			try:
				vsettings=float(configfile['1'])
			except:
				configfile['1']=""
			if(str(vsettings)+""!=""):
				entry_botton_b.set_value(float(vsettings))
			vsettings=""
			try:
				vsettings=float(configfile['2'])
			except:
				configfile['2']=""
			if(str(vsettings)+""!=""):
				entry_botton_h.set_value(int(vsettings)/10.0)
			vsettings=""
			try:
				vsettings=float(configfile['3'])
			except:
				configfile['3']=""
			if(str(vsettings)+""!=""):
				entry_botton_a.set_value(float(vsettings))
			def update(hfff):
				global returna
				if (checkbox.get_active()==True):
					menuitemc.set_sensitive(True)
				if (checkbox.get_active()==False):
					menuitemc.set_sensitive(False)
					savecommand.value="False"
				devselectfoldername="None"
				if(int(dev_combo.get_model()[dev_combo.get_active_iter()][:2][0])!=1):
					devselectfoldername=str(dev[(dev_combo.get_model()[dev_combo.get_active_iter()][:2][0])-2][:][1])
				v=str(entry_botton_b.get_value())+";"
				v=v+str(entry_botton_h.get_value())+";"
				v=v+str(devselectfoldername)+";"
				v=v+str(entry_botton_a.get_value())+";"
				v=v+str(checkbox.get_active())
				v=v+"----------"+str(currentdevice.gettoolbarvalues())
				configfile['1']=float(entry_botton_b.get_value())
				configfile['2']=int(float(entry_botton_h.get_value())*10)
				configfile['3']=float(entry_botton_a.get_value())
				returna=''+str(v)
				ui_values.value=''+returna
				if(str(checkbox.get_active())=="True"):
					buttont.set_sensitive(False)
				else:
					if(str(checkbox.get_sensitive())=="True"):
						buttont.set_sensitive(True)
				currentdevice.changedelement()
			update(True)
			def devicechanged(hdfghgd):
				global currentdevice
				global removedselectitem
				savecommand.value="False"
				currentdevice.removeallwidgets(toolbar)
				if((int(dev_combo.get_model()[dev_combo.get_active_iter()][:2][0]))==1):
					currentdevice=blank_module
					checkbox.set_sensitive(False)
					checkbox.set_active(False)
					buttont.set_sensitive(False)
					entry_botton_b.set_sensitive(False)
					entry_botton_h.set_sensitive(False)
					entry_botton_a.set_sensitive(False)
				else:
					if(removedselectitem==0):
						dev.remove(selectdeviceitem)
						removedselectitem=1
					currentdevice=loaded_spectrometer_modules[(int(dev_combo.get_model()[dev_combo.get_active_iter()][:2][0]))-2]
					checkbox.set_sensitive(True)
					buttont.set_sensitive(True)
					entry_botton_b.set_sensitive(True)
					entry_botton_h.set_sensitive(True)
					entry_botton_a.set_sensitive(True)
				currentdevice.additems(toolbar)
				self.show_all()
				toolbarelementsevents=currentdevice.getchangedeventelements()
				addelementscounter=0
				while (addelementscounter<len(toolbarelementsevents)):
					toolbarelementsevents[addelementscounter].connect("changed", update)
					addelementscounter=addelementscounter+1
				update(hdfghgd)
			if(deviceavaliableonloadcount==1):
				dev_combo.set_active(1)
				currentdevice.removeallwidgets(toolbar)
				if(removedselectitem==0):
					dev.remove(selectdeviceitem)
					removedselectitem=1
				currentdevice=loaded_spectrometer_modules[0]
				checkbox.set_sensitive(True)
				buttont.set_sensitive(True)
				entry_botton_b.set_sensitive(True)
				entry_botton_h.set_sensitive(True)
				entry_botton_a.set_sensitive(True)
				currentdevice.additems(toolbar)
			checkbox.set_sensitive(False)
			checkbox.set_active(False)
			buttont.set_sensitive(False)
			entry_botton_b.set_sensitive(False)
			entry_botton_h.set_sensitive(False)
			entry_botton_a.set_sensitive(False)
			tool_item_botton_f = Gtk.ToolItem()
			entry_botton_f = Gtk.Label("   Device:")
			tool_item_botton_f.add(entry_botton_f)
			toolbarb.insert(tool_item_botton_a,0)
			toolbarb.insert(tool_item_botton_d,0)
			toolbarb.insert(tool_item_botton_b,0)
			toolbarb.insert(tool_item_botton_c,0)
			toolbarb.insert(tool_item_botton_h,0)
			toolbarb.insert(tool_item_botton_e,0)
			toolbarb.insert(tool_item_botton_g,0)
			toolbarb.insert(tool_item_botton_f,0)
			policy = Gtk.SpinButtonUpdatePolicy.IF_VALID
			entry_botton_b.set_update_policy(policy)
			entry_botton_h.set_update_policy(policy)
			entry_botton_a.set_update_policy(policy)
			entry_botton_b.connect("output", update)
			entry_botton_h.connect("output", update)
			dev_combo.connect("changed", devicechanged)
			entry_botton_a.connect("output", update)
			checkbox.connect("toggled", update)
			def singlerequestedbuttont(self):
				fastmmap.write(mmapbutton,"c")
			buttont.connect("clicked", singlerequestedbuttont)
			toolbarelementsevents=currentdevice.getchangedeventelements()
			addelementscounter=0
			while (addelementscounter<len(toolbarelementsevents)):
				try:
					toolbarelementsevents[addelementscounter].connect("changed", update)
				except:
					print("")
				try:
					toolbarelementsevents[addelementscounter].connect("output", update)
				except:
					print("")
				addelementscounter=addelementscounter+1
			toolbarc = Gtk.Toolbar()
			contextpeaks = toolbarc.get_style_context()
			contextpeaks.add_class(Gtk.STYLE_CLASS_PRIMARY_TOOLBAR)
			tool_item_botton_peaks = Gtk.ToolItem()
			tool_item_botton_peaks.add(entry_botton_peaks)
			tool_item_botton_info = Gtk.ToolItem()
			toolbarc.insert(tool_item_botton_peaks,0)
			if(deviceavaliableonloadcount==1):
				checkbox.set_sensitive(True)
				buttont.set_sensitive(True)
				entry_botton_b.set_sensitive(True)
				entry_botton_h.set_sensitive(True)
				entry_botton_a.set_sensitive(True)
				update(self)
			hbox.pack_start(toolbarb, False, False, 0)
			hbox.pack_start(toolbarc, False, False, 0)
	win = MyWIndow()
	time.sleep(0.1)
	fastmmap.write(mmap,",11gui loaded")
	time.sleep(0.2)
	fastmmap.write(mmap,",10main window load...")
	time.sleep(0.5)
	while(readyshoww.value=="False"):
		time.sleep(0.1)
	while(readyshowwin.value=="-1"):
		time.sleep(0.1)
	while(fastmmap.getsharedstring(int(readyshowwin.value))!="ok"):
		time.sleep(0.1)
	fastmmap.write(mmap,",\2")
	win.show_all()
	win.connect("delete-event", Gtk.main_quit)
	Gtk.main()
	win.destroy()
	windowclose.value="closed"
managerb = Manager()
ui_values = managerb.Value(c_char_p, "")
uncheck = managerb.Value(c_char_p, "False")
readyshoww = managerb.Value(c_char_p, "False")
gtkcolorsfromnode = managerb.Value(c_char_p, "")
infotext = managerb.Value(c_char_p, "")
savecommand = managerb.Value(c_char_p, "")
windowclose = managerb.Value(c_char_p, "opened")
savecommand.value="False"
spectrum = managerb.Value(c_char_p, "")
spectrumx = managerb.Value(c_char_p, "")
readshowwin = managerb.Value(c_char_p, "-1")
infotext.value="Peaks:[]          Frequency:0hz             Temperature:--"+u'\xb0'+"C"
startedp2final=0
p2=Process(target=mainui, args=(ui_values,gtkcolorsfromnode,spectrum,savecommand,infotext,windowclose,uncheck,spectrumx,readyshoww,readshowwin))
globalpeaks=""
def mainload(processid):
	global mmap
	global startedp2final
	global mmapbutton
	mmap=fastmmap.connectmmap("spectrareads",""+str(processid))
	while mmap==-1:
		mmap=fastmmap.connectmmap("spectrareads",""+str(processid))
		print("Connection failed on main-ui 1...Reconnecting")
		time.sleep(0.1)
	mmapbutton=fastmmap.connectmmap("spectrareadd","L"+str(processid))
	while mmapbutton==-1:
		mmapbutton=fastmmap.connectmmap("spectrareadd","L"+str(processid))
		print("Connection failed on main-ui 2...Reconnecting")
		time.sleep(0.1)
	readyshowwin.value=str(mmap)
	fastmmap.write(mmap,",03starting gui")
	p2.start()
	startedp2final=1
aaafreq=""
aaatemp=""
aaaextra=""
def stopacquisition():
	uncheck.value="True"
def readyshowwin():
	readyshoww.value="True"
def getselectedvalues():
	return ui_values.value
def iswindowclosed():
	if startedp2final==0:
		return "open True"
	return windowclose.value+" "+str(p2.is_alive())
def getsavecommand():
	return savecommand.value
def clearsavecommand():
	savecommand.value="False"
def updateinfotext(freq,temp,extra):
	global globalpeaks
	global aaafreq
	global aaatemp
	global aaaextra
	infotext.value="Peaks:["+str(globalpeaks)+"]          Frequency:"+str(freq)+"hz             Temperature:"+str(temp)+""+u'\xb0'+"C"+str(extra)+""
	aaafreq=freq
	aaatemp=temp
	aaaextra=extra
def updateinfopeaks(peaksdata):
	global globalpeaks
	global aaafreq
	global aaatemp
	global aaaextra
	infotext.value="Peaks:["+str(peaksdata)+"]          Frequency:"+str(aaafreq)+"hz             Temperature:"+str(aaatemp)+""+u'\xb0'+"C"+str(aaaextra)+""
	globalpeaks=str(peaksdata)
def updategraphspectrumvariable(hgfhdjugfhlidfhgaudgf,fghddhsdgfhfg):
	spectrum.value=fghddhsdgfhfg
	spectrumx.value=hgfhdjugfhlidfhgaudgf
