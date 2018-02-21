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

print(os.getgroups())
#Here we no longer need to be in the group allowed to access the
#spectrometer, since this is done in another thread. We should drop the group
#here, to avoid code injection using the unsafe user set env vars we are about to restore
#from /dev/shm/spectrareadenv-shm-1823019506 or the config file (shelve module is vulnerable to code injection)
#but this is aparently not possible (a process can't drop its own privileges without being root).
#Anyway this group can access only some devices
#so the setuid solution is still better than allowing direct user access using udev rules, although it won't stop
#a decided user to directly access the spectrometer.

#The ideal solution would be to have a privileged daemon acessing the devices and talking using sockets to de main program...
#but this would probably limit access speed to the device.


#Restore the unsafe variables...
with open('/dev/shm/spectrareadenv-shm-1823019506') as f:
	for line in f:
		try:
			key, value = line.replace('export ', '', 1).strip().split('=', 1)
			os.environ[key] = value
		except:
			print("Could not export enviroment variable")
import gi
import sys
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

def mainui(ui_values,gtkcolorsfromnode,spectrum,savecommand,infotext,windowclose,uncheck):
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
						self.set_title("Save single spectra and peaks")
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
						labelfilenamed = Gtk.Label("\nSpectra file name:    ")
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
							save_multiple_a(self,"Save multiple spectra and peaks::..Select folder")
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
						def updategtkviewb(aaa,bbb,ccc):
							viewb.execute_script("var systemcolors='"+gtkcolorsfromnode.value+"'")
							if(viewb.get_uri()+""!="file://"+os.path.dirname(os.path.abspath(__file__))+"/resources/help/help.html"):
								buttonback.show()
							else:
								buttonback.hide()
						def goback(aaaa):
							viewb.go_back()
						viewb.connect('title-changed', updategtkviewb)
						buttonback.connect('clicked', goback)
						viewb.execute_script("var systemcolors='"+gtkcolorsfromnode.value+"'")
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
						adjustment_botton_b_dialog = Gtk.Adjustment(value=1,lower=1,upper=100000,step_increment=1,page_increment=1)
						entry_botton_b_dialog = Gtk.SpinButton(adjustment=adjustment_botton_b_dialog, digits=0)
						entry_botton_b_dialog.set_numeric(True)
						entry_botton_b_dialog.set_width_chars(7)
						label_save_int_a = Gtk.Label("spectra acquired")
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
						fileentry.connect("changed", updatesavemultipleinternalvalues)
						entry_botton_b_dialog.connect("changed", updatesavemultipleinternalvalues)
						self.show_all()
					def on_response(self, dialog, response):
						if response == Gtk.ResponseType.OK:
							dialog.destroy()
							save_multiple_a(self,"Save multiple spectra::..Select folder")
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
						adjustment_botton_b_dialog = Gtk.Adjustment(value=1,lower=1,upper=100000,step_increment=1,page_increment=1)
						entry_botton_b_dialog = Gtk.SpinButton(adjustment=adjustment_botton_b_dialog, digits=0)
						entry_botton_b_dialog.set_numeric(True)
						entry_botton_b_dialog.set_width_chars(7)
						label_save_int_a = Gtk.Label("spectra acquired")
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
						fileentry.connect("changed", updatesavemultipleinternalvalues)
						entry_botton_b_dialog.connect("changed", updatesavemultipleinternalvalues)
						self.show_all()
					def on_response(self, dialog, response):
						if response == Gtk.ResponseType.OK:
							dialog.destroy()
							save_multiple_a(self,"Save multiple peaks::..Select folder")
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
						label_save_a = Gtk.Label("Save every:         ")
						adjustment_botton_b_dialog = Gtk.Adjustment(value=1,lower=1,upper=100000,step_increment=1,page_increment=1)
						entry_botton_b_dialog = Gtk.SpinButton(adjustment=adjustment_botton_b_dialog, digits=0)#
						entry_botton_b_dialog.set_numeric(True)
						entry_botton_b_dialog.set_width_chars(7)
						label_save_int_a = Gtk.Label("spectra acquired")
						label_save_int_b = Gtk.Label("s")
						hboxdialoginta = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=3)
						label_save_b = Gtk.Label("Save for:              ")
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
						fileentry.connect("changed", updatesavemultipleinternalvalues)
						fileentryd.connect("changed", updatesavemultipleinternalvalues)
						entry_botton_b_dialog.connect("changed", updatesavemultipleinternalvalues)
						self.show_all()
					def on_response(self, dialog, response):
						if response == Gtk.ResponseType.OK:
							dialog.destroy()
							save_multiple_a(self,"Save multiple spectra and peaks::..Select folder")
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
						def updategtkviewb(aaa,bbb,ccc):
							viewb.execute_script("var systemcolors='"+gtkcolorsfromnode.value+"'")
							if(viewb.get_uri()+""!="file://"+os.path.dirname(os.path.abspath(__file__))+"/resources/about/about.html"):
								buttonback.show()
							else:
								buttonback.hide()
						def goback(aaaa):
							viewb.go_back()
						viewb.connect('title-changed', updategtkviewb)
						buttonback.connect('clicked', goback)
						viewb.execute_script("var systemcolors='"+gtkcolorsfromnode.value+"'")
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
			menuitemb = Gtk.MenuItem(label='Save single')
			menuitemc = Gtk.MenuItem(label='Save multiple')
			menuitemd = Gtk.MenuItem(label='Quit')
			menuitemc.set_sensitive(False)
			submenubar.append(menuitemb)
			submenubar.append(menuitemc)
			submenubar.append(menuitemd)
			menuitem.set_submenu(submenubar)
			menubar.append(menuitem)
			menubar.append(menuitemhelp)
			submenubarhelp = Gtk.Menu()
			submenuitemhelpa = Gtk.MenuItem(label='Help')
			submenuitemhelpb = Gtk.MenuItem(label='About')
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
			currentdevice.additems(toolbar)
			hbox.pack_start(menubar, False, False, 0)
			hbox.pack_start(toolbar, False, False, 0)
			hboxb = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=6)
			hboxb.get_style_context().add_class(Gtk.STYLE_CLASS_LIST)
			hbox.pack_start(hboxb, True, True, 0)
			scrolled_window = Gtk.ScrolledWindow()
			view = WebKit.WebView()
			hboxb.pack_start(scrolled_window, True, True, 0)
			scrolled_window.add(view)
			view.open("file://"+os.path.dirname(os.path.abspath(__file__))+"/resources/graph/graph.html")
			view.show()
			#------------------------------------------------------
			#Fix the white borders on webview when resizing window.
			#------------------------------------------------------
			def installTransparency(self, component):
				component.set_visual(component.get_screen().get_rgba_visual())
			view.set_transparent(True)
			installTransparency(self,view)
			#------------------------------------------------------
			toolbarb = Gtk.Toolbar()
			context = toolbarb.get_style_context()
			context.add_class(Gtk.STYLE_CLASS_PRIMARY_TOOLBAR)
			settings = view.get_settings()
			settings.set_property('enable-default-context-menu', False)
			view.set_settings(settings)
			tool_item_botton_a = Gtk.ToolItem()
			adjustment_botton_a = Gtk.Adjustment(value=2,lower=1,upper=10,step_increment=1,page_increment=1)
			entry_botton_a = Gtk.SpinButton(adjustment=adjustment_botton_a, digits=0)
			#Fix a bug in some GTK themes (For example Mint-Y) that causes the buttons to disappear (white background and text in white text entry) when the spin button has focus.
			entry_botton_a.get_style_context().remove_class(Gtk.STYLE_CLASS_SPINBUTTON)
			entry_botton_a.set_numeric(True)
			entry_botton_a.set_width_chars(3)
			tool_item_botton_a.add(entry_botton_a)
			entry_botton_peaks = Gtk.Label(infotext.value)
			def updategtk(aaa,bbb,ccc):
				view.execute_script("var systemcolors='"+gtkcolorsfromnode.value+"'")
				view.execute_script("var spectrumdata='"+spectrum.value+"'")
				entry_botton_peaks.set_text(infotext.value)
				if(uncheck.value=="True"):
					uncheck.value="False"
					checkbox.set_active(False)
			view.connect('title-changed', updategtk)
			tool_item_botton_b = Gtk.ToolItem()
			adjustment_botton_b = Gtk.Adjustment(value=1,lower=0,upper=10,step_increment=1,page_increment=1)
			entry_botton_b = Gtk.SpinButton(adjustment=adjustment_botton_b, digits=0)
			#Fix a bug in some GTK themes (For example Mint-Y) that causes the buttons to disappear (white background and text in white text entry) when the spin button has focus.
			entry_botton_b.get_style_context().remove_class(Gtk.STYLE_CLASS_SPINBUTTON)
			entry_botton_b.set_numeric(True)
			entry_botton_b.set_width_chars(3)
			tool_item_botton_b.add(entry_botton_b)
			tool_item_botton_h = Gtk.ToolItem()
			adjustment_botton_h = Gtk.Adjustment(value=0,lower=0,upper=1,step_increment=0.1,page_increment=0.1)
			entry_botton_h = Gtk.SpinButton(adjustment=adjustment_botton_h, digits=1)
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
				v=str(entry_botton_b.get_value())+";"
				v=v+str(entry_botton_h.get_value())+";"
				v=v+str(dev_combo.get_model()[dev_combo.get_active_iter()][:2][0])+";"
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
					buttont.set_sensitive(True)
				currentdevice.changedelement()
			update(True)
			def devicechanged(hdfghgd):
				global currentdevice
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
					dev.remove(selectdeviceitem)
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
				dev.remove(selectdeviceitem)
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
			entry_botton_b.connect("changed", update)
			entry_botton_h.connect("changed", update)
			dev_combo.connect("changed", devicechanged)
			entry_botton_a.connect("changed", update)
			checkbox.connect("toggled", update)
			toolbarelementsevents=currentdevice.getchangedeventelements()
			addelementscounter=0
			while (addelementscounter<len(toolbarelementsevents)):
				toolbarelementsevents[addelementscounter].connect("changed", update)
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
	while(gtkcolorsfromnode.value+""==""):
		time.sleep(1)
	time.sleep(1.5)
	win.show_all()
	win.connect("delete-event", Gtk.main_quit)
	Gtk.main()
	win.destroy()
	windowclose.value="closed"
managerb = Manager()
ui_values = managerb.Value(c_char_p, "")
uncheck = managerb.Value(c_char_p, "False")
gtkcolorsfromnode = managerb.Value(c_char_p, "")
infotext = managerb.Value(c_char_p, "")
savecommand = managerb.Value(c_char_p, "")
windowclose = managerb.Value(c_char_p, "opened")
savecommand.value="False"
spectrum = managerb.Value(c_char_p, "")
infotext.value="Peaks:[]          Frequency:0hz             Temperature:--"+u'\xb0'+"C"
p2=Process(target=mainui, args=(ui_values,gtkcolorsfromnode,spectrum,savecommand,infotext,windowclose,uncheck))
p2.start()
def stopacquisition():
	uncheck.value="True"
def getselectedvalues():
	return ui_values.value
def iswindowclosed():
	return windowclose.value+" "+str(p2.is_alive())
def getsavecommand():
	return savecommand.value
def clearsavecommand():
	savecommand.value="False"
def updatecolorvalues(fghddhsdgfhfg):
	gtkcolorsfromnode.value=fghddhsdgfhfg
def updateinfotext(peaks,freq,temp,extra):
	infotext.value="Peaks:["+str(peaks)+"]          Frequency:"+str(freq)+"hz             Temperature:"+str(temp)+""+u'\xb0'+"C"+str(extra)+""
def updategraphspectrumvariable(fghddhsdgfhfg):
	spectrum.value=fghddhsdgfhfg
