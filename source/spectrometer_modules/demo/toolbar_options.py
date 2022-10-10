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

#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#In this file we declare the toolbar itens that are specific to the spectrometer.
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#Import necessary modules here.



#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#Always import the folowing module.
from ui_headers import *
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#We will use configfile to save the values in the widgets defined below to disk.
#The argument passed to initconfig must not be used in other module and can not contain "/"
#or any other character that can't be in a file name (for exemple the null character)
configfile=initconfig("demo")
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#Below we add the widgets that we want in the toolbar that are device specific
label = Gtk.Label("Integration time:")
labelb = Gtk.Label(u"\u03BCs")
labelc = Gtk.Label("  Sleep time:")
labeld = Gtk.Label("  Averages:")
tool_itemc = Gtk.ToolItem()
tool_itemd = Gtk.ToolItem()
tool_itemc.add(label)
tool_itemd.add(labelb)
tool_itemb = Gtk.ToolItem()
adjustment = Gtk.Adjustment(value=5702,lower=3,upper=9854,step_increment=1,page_increment=1,page_size=10)
adjustmentb = Gtk.Adjustment(value=1,lower=1,upper=30,step_increment=1,page_increment=1,page_size=10)
entry = Gtk.SpinButton(adjustment=adjustment, digits=0)
#Fix a bug in some GTK themes (For example Mint-Y) that causes the buttons to disappear (white background and text in white text entry) when the spin button has focus.
entry.get_style_context().remove_class(Gtk.STYLE_CLASS_SPINBUTTON)
entry.set_numeric(True)
entryb = Gtk.SpinButton(adjustment=adjustmentb, digits=0)
#Fix a bug in some GTK themes (For example Mint-Y) that causes the buttons to disappear (white background and text in white text entry) when the spin button has focus.
entryb.get_style_context().remove_class(Gtk.STYLE_CLASS_SPINBUTTON)
entryb.set_numeric(True)
entry.set_width_chars(5)
entryb.set_width_chars(2)
tool_itementryaverages = Gtk.ToolItem()
tool_itementryaverages.add(entryb)
tool_itemb.add(entry)
freq = Gtk.ListStore(int, str)
tool_itemcombo = Gtk.ToolItem()
tool_itemlabelcombo = Gtk.ToolItem()
tool_itemlabelaverages = Gtk.ToolItem()
tool_itemlabelaverages.add(labeld)
freq_combo = Gtk.ComboBox.new_with_model(freq)
renderer_textb = Gtk.CellRendererText()
freq_combo.pack_start(renderer_textb, True)
freq_combo.add_attribute(renderer_textb, "text", 1)
tool_itemcombo.add(freq_combo)
tool_itemlabelcombo.add(labelc)
freq_combo.set_active(0)
oldtint=-1
def changeavailtinits(tint,vsettings):
	global oldtint
	global lastactive
	lastactive=int(vsettings)
	try:
		lastactive=freq_combo.get_model()[freq_combo.get_active_iter()][:2][0]
	except:
		null=None
	if(tint!=oldtint):
		for introw in freq:
			del freq[0]
		i=0
		if(((1000000/10)-126-20-tint)>=0):
			freq.append([1, str((1000000/30)-126-20-tint)+u" \u03BCs + 126 \u03BCs + 20 \u03BCs = 30 hz"])
			i=i+1
		if(((1000000/100)-126-20-tint)>=0):
			freq.append([2, str((1000000/100)-126-20-tint)+u" \u03BCs + 126 \u03BCs + 20 \u03BCs = 100 hz"])
			i=i+1
		if(((1000000/500)-126-20-tint)>=0):
			freq.append([3, str((1000000/500)-126-20-tint)+u" \u03BCs + 126 \u03BCs + 20 \u03BCs = 500 hz"])
			i=i+1
		#if(((1000000/1000)-126-20-tint)>=0):
		#	freq.append([4, str((1000000/1000)-126-20-tint)+u" \u03BCs + 126 \u03BCs + 20 \u03BCs = 1 khz"])
		#	i=i+1
		#if(((1000000/2000)-126-20-tint)>=0):
		#	freq.append([5, str((1000000/2000)-126-20-tint)+u" \u03BCs + 126 \u03BCs + 20 \u03BCs = 2 khz"])
		#	i=i+1
		#if(((1000000/3000)-126-20-tint)>=0):
		#	freq.append([6, str((1000000/3000)-126-20-tint)+u" \u03BCs + 126 \u03BCs + 20 \u03BCs = 3 khz"])
		#	i=i+1
		#if(((1000000/4000)-126-20-tint)>=0):
		#	freq.append([7, str((1000000/4000)-126-20-tint)+u" \u03BCs + 126 \u03BCs + 20 \u03BCs = 4 khz"])
		#	i=i+1
		#if(((1000000/5000)-126-20-tint)>=0):
		#	freq.append([8, str((1000000/5000)-126-20-tint)+u" \u03BCs + 126 \u03BCs + 20 \u03BCs = 5 khz"])
		#	i=i+1
		#if(((1000000/6000)-126-20-tint)>=0):
		#	freq.append([9, str((1000000/6000)-126-20-tint)+u" \u03BCs + 126 \u03BCs + 20 \u03BCs = 6 khz"])
		#	i=i+1
		if((int(lastactive))>=i):
			lastactive=i
		freq_combo.set_active(int(lastactive)-1)
		oldtint=tint
def changetinta(self):
	changeavailtinits(float(entry.get_value()),2)
entry.connect('value-changed', changetinta)
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#Here we will load the values from the configuration file into the widgets.
#We use the try/except do avoid errors if the value stored is not an float/int or if there is no value stored yet.
vsettings="1"
try:
	vsettings=float(configfile['1'])
except:
	configfile['1']=""
if(str(vsettings)+""!=""):
	entry.set_value(float(vsettings))
vsettings="1"
try:
	vsettings=float(configfile['2'])
except:
	configfile['2']=""
if(str(vsettings)+""!=""):
	entryb.set_value(float(vsettings))
vsettings="1"
try:
	vsettings=float(configfile['3'])
except:
	configfile['3']=""
changeavailtinits(float(entry.get_value()),vsettings)
if(str(vsettings)+""!=""):
	freq_combo.set_active(int(vsettings)-1)
configfile.close()
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#The folowing function receives the toolbar object and insert the widgets declared before in order.
#This function must always be declared here.
def additems(toolbar):
	#toolbar.insert(tool_itementryaverages,0)
	#toolbar.insert(tool_itemlabelaverages,0)
	toolbar.insert(tool_itemcombo,0)
	toolbar.insert(tool_itemlabelcombo,0)
	toolbar.insert(tool_itemd,0)
	toolbar.insert(tool_itemb,0)
	toolbar.insert(tool_itemc,0)
#The folowing function sends the values from the widgets to the main program. This values will be returned to the spectrometer data acquisition function.
#Please don't use any separator in the string other than ; because this string will be concatenated later.
#This function must always be declared here.
def gettoolbarvalues():
	gettoolbarvaluesreturnvalue=""
	gettoolbarvaluesreturnvalue=gettoolbarvaluesreturnvalue+str(freq_combo.get_model()[freq_combo.get_active_iter()][:2][0])+";"
	gettoolbarvaluesreturnvalue=gettoolbarvaluesreturnvalue+str(entry.get_value())+";"
	gettoolbarvaluesreturnvalue=gettoolbarvaluesreturnvalue+str(entryb.get_value())+";"
	return gettoolbarvaluesreturnvalue
#This function returns an array with the widgets that the user can change values. The "changed" event will be connected to this widgets.
#This function must always be declared here.
def getchangedeventelements():
	returnarray=[None] * 3
	returnarray[0]=freq_combo
	returnarray[1]=entry
	returnarray[2]=entryb
	return returnarray
#This function is called when some value on a widget changes.
#Here we will store the new values on the configuration file.
#This function must always be declared here.
def changedelement():
	configfile=initconfig("imon256")
	configfile['1']=float(entry.get_value())
	configfile['2']=float(entryb.get_value())
	configfile['3']=float((freq_combo.get_model()[freq_combo.get_active_iter()][:2][0]))
	configfile.close()
#This function removes all the toolbar buttons from the toolbar.
#This function is called when the user changes the spectrometer in the botton left combo box.
#This function must always be declared here.
def removeallwidgets(toolbar):
	toolbar.remove(tool_itementryaverages)
	toolbar.remove(tool_itemlabelaverages)
	toolbar.remove(tool_itemcombo)
	toolbar.remove(tool_itemlabelcombo)
	toolbar.remove(tool_itemd)
	toolbar.remove(tool_itemb)
	toolbar.remove(tool_itemc)
def getdevicelist():
	return ["01"]
