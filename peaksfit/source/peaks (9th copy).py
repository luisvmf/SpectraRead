#coding: utf-8
import os, errno, time
os.environ['MPLCONFIGDIR'] = os.path.expanduser("~")+"/.spectraread/spectrareadcfg/"
os.environ['PYTHON_EGG_CACHE'] = os.path.expanduser("~")+"/.spectraread/spectrareadegg/"
import sys
if not hasattr(sys, 'argv'):
	sys.argv  = ['']
reload(sys)
sys.setdefaultencoding('utf-8')
if not hasattr(sys, 'argv'):
	sys.argv  = ['']
import fastmmap
import math
import random
from multiprocessing import Process, Value, Manager
from ctypes import c_char_p, c_int
import warnings
warnings.filterwarnings("ignore")
mmap=-1
def initgui(windowclose,datavar,datavaronlyone,currentshowlength):
	time.sleep(0.3)
	fastmmap.write(mmap,",10load np")
	import numpy as np
	time.sleep(0.3)
	fastmmap.write(mmap,",10load gi")
	import gi
	gi.require_version('Gtk', '3.0')
	time.sleep(0.3)
	fastmmap.write(mmap,",05load gi repository")
	from gi.repository import Gtk, Gdk, GObject ,Pango, Gio
	fastmmap.write(mmap,",10load plot")
	import matplotlib
	import matplotlib.pyplot as plt
	time.sleep(0.3)
	fastmmap.write(mmap,",05load backend 1")
	matplotlib.use('GTKAgg')
	time.sleep(0.3)
	fastmmap.write(mmap,",10load figure")
	from matplotlib.figure import Figure
	from numpy import arange, pi, random, linspace
	#Possibly this rendering backend is broken currently
	#from gi.repository import GObject
	#from gi.repository import Gtk
	time.sleep(0.3)
	fastmmap.write(mmap,",10load backend 2")
	from matplotlib.backends.backend_gtk3agg import FigureCanvasGTK3Agg as FigureCanvas
	#from matplotlib.backends.backend_gtk3cairo import FigureCanvasGTK3Cairo as FigureCanvas
	from matplotlib.colors import ColorConverter
	from matplotlib.backends.backend_gtk3 import NavigationToolbar2GTK3 as NavigationToolbar
	import matplotlib.patches as patches
	#--------------------------------------------------------------------
	#--------------------------------------------------------------------
	#--------------------------------------------------------------------
	#maxnlines=5
	bufferlength=900
	currentshowlength.value=200
	#continuous data stream mode
	contarray=[]*0
	contimearr=[]*0
	datamodeselected=[0]*1
	totaldataarr=[]*0
	startstopupdategraph=[1]*1
	#--------------------------------------------------------------------
	#--------------------------------------------------------------------
	#--------------------------------------------------------------------
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
	def format_coord(x, y):
		return ''
	def aaa(a):
		if("button=None" in str(a)):
			return False
		else:
			return True
	inside=[0]*2
	class CustomToolbar(NavigationToolbar):
		toolitems=((u'Pan', u'Pan axes with left mouse, zoom with right', u'move', u'pan'), (u'Zoom', u'Zoom to rectangle', u'zoom_to_rect', u'zoom'),  )
	hboxb = Gtk.Box(spacing=0, orientation=Gtk.Orientation.VERTICAL)
	inmatplotlibbox = Gtk.Box(spacing=0, orientation=Gtk.Orientation.VERTICAL)
	inmatplotlibboxb = Gtk.Box(spacing=0, orientation=Gtk.Orientation.VERTICAL)
	inmatplotlibboxc = Gtk.Box(spacing=0, orientation=Gtk.Orientation.HORIZONTAL)
	inmatplotlibboxd = Gtk.Box(spacing=0, orientation=Gtk.Orientation.HORIZONTAL)
	inmatplotlibboxe = Gtk.Box(spacing=0, orientation=Gtk.Orientation.HORIZONTAL)
	inmatplotlibboxf = Gtk.Box(spacing=0, orientation=Gtk.Orientation.HORIZONTAL)
	inmatplotlibbox.get_style_context().add_class(Gtk.STYLE_CLASS_LIST)
	inmatplotlibboxb.get_style_context().add_class(Gtk.STYLE_CLASS_LIST)
	inmatplotlibboxc.get_style_context().add_class(Gtk.STYLE_CLASS_LIST)
	inmatplotlibboxd.get_style_context().add_class(Gtk.STYLE_CLASS_LIST)
	inmatplotlibboxe.get_style_context().add_class(Gtk.STYLE_CLASS_LIST)
	inmatplotlibboxf.get_style_context().add_class(Gtk.STYLE_CLASS_LIST)
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
		labelbottom=False)
	ax.tick_params(
		axis='y',
		which='both',
		left=False,
		right=False,
		labelleft=True)
	matplotlib.rc('font', **font)
	plt.rc('font', **font)
	canvas = FigureCanvas(fig)
	def mousemoveevent(event):
		intpos=[0]*2
		val=(str(event).split("xy=(")[1].split(")")[0].split(","))
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
	canvas.mpl_connect('motion_notify_event', mousemoveevent)
	canvas.set_size_request(200,200)
	inmatplotlibtoolbarb = CustomToolbar(canvas, None)
	#currentdataarr=[0]*maxnlines
	currentdataarr=[0]*0
	linearr=[None]*0
	#j=0
	#while(j<len(currentdataarr)):
	#	currentdataarr[j]=[0]*bufferlength
	#	j=j+1
	#linearr=[None]*maxnlines
	#j=0
	#while(j<len(linearr)):
	#	linearr[j],=ax.plot(np.arange(bufferlength),np.asarray(currentdataarr[j]),label='',marker='o', markersize=15,linewidth=3)
	#	ax.lines.remove(linearr[j])
	#	j=j+1
	realtimestampfrompproc1=[0]*0;
	j=0
	#line,=ax.plot(np.arange(200),np.asarray(currentdataarr),label='',marker='o', markersize=15,linewidth=3,color=(selectcolor[0]/1.2,selectcolor[1]/1.2,selectcolor[2]/1.2))
	def addline(currentinitvarlines):
		currentdataarr.append([0]*bufferlength)
		contarray.append([0]*0)
		totaldataarr.append([0]*0)
		realtimestampfrompproc1.append([0]*bufferlength)
		intvara,=ax.plot(np.arange(bufferlength),np.asarray([23]*bufferlength),label='',marker='o', markersize=15,linewidth=3)
		linearr.append(intvara)
		ax.lines.remove(intvara)
		currentinitvarlines[0]=currentinitvarlines[0]+1
	currentinitvarlines=[0]*2
	#addline(currentinitvarlines)
	#addline(currentinitvarlines)
	#addline(currentinitvarlines)
	ax.tick_params(axis='both', which='major', pad=15)
	mousepos=[0]*2
	mousepostext=[0]*2
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
	inmatplotlibbox.pack_start(inmatplotlibboxb, False, True, 0)
	inmatplotlibbox.pack_start(inmatplotlibboxc, True, True, 0)
	inmatplotlibboxc.pack_start(inmatplotlibboxd, False, False, 0)
	inmatplotlibboxc.pack_start(inmatplotlibboxe, True, True, 0)
	inmatplotlibboxc.pack_start(inmatplotlibboxf, False, False, 0)
	inmatplotlibboxe.pack_start(inmatplotlibsw, True, True, 0)
	inmatplotlibboxf.pack_start(matplotlibshowtoolbar, False, True, 0)



	i=[0]*5
	winsize=[0]*3
	timecounterb=[0]*3
	#hboxb.pack_start(self.inmatplotlibbox, True, True, 0)
	rect = patches.Rectangle((0.3,0.2),((ax.axis())[1]-(ax.axis())[0])*2.3/100.0,((ax.axis())[3]-(ax.axis())[2])*2.3/100.0,linewidth=0,facecolor=(selectcolor[0]/1.2,selectcolor[1]/1.2,selectcolor[2]/1.2))
	recttext=ax.text(0.3,0.2, "x,y=(0,0)", bbox=dict(facecolor='gray', alpha=0.5),fontsize=59)
	ax.add_patch(rect)
	rect.set_visible(False)
	recttext.set_visible(False)
	selecteddatarows=[None]*0
	linesshown=[None]*0
	graphtimearray=[0]*bufferlength
	k=0
	starttime=time.time()
	while(k<len(graphtimearray)):
		graphtimearray[k]=k/10000.0
		k=k+1
	def desenhagrafico(i,autoscaleval,entrytextselectstyle,rect,mousepos,inside,recttext,mousepostext,winsize,timecounterb,infotext,returnedfalse,oldmouseposloopcontrol,selecteddatarows,linesshown,graphtimearray,currentshowlength,datamodeselected,realtimestampfrompproc1):
		#-------------------------------
		#-------------------------------
		#-------------------------------
		#if(str(get_size())==winsize[0]):
		#-------------------------------
		#-------------------------------
		#-------------------------------
		if(True):
			if(time.time()-i[0]>0.1):
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
						recttext.set_position((mousepos[0],mousepos[1]))
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
						#line.set_color((selectcolor[0]/1.2,selectcolor[1]/1.2,selectcolor[2]/1.2))
						addcolor=0
						if(selectcolor[1]>0):
							#lineb.set_color((selectcolor[0],selectcolor[1]/5.2,selectcolor[2]/5.2))
							addcolor=1
						if(selectcolor[2]>0):
							#lineb.set_color((selectcolor[0],selectcolor[1]/5.2,selectcolor[2]/5.2))
							addcolor=1
						if(addcolor==0):
							if(selectcolor[0]<0.7):
								addcolor=0.3
							else:
								addcolor=-0.3
							#lineb.set_color((selectcolor[0]+addcolor,selectcolor[1]/5.2,selectcolor[2]/5.2))
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
				timecounterb[0]=0
				if(timecounterb[0]<=0):
					updateddatatryexcept=1
					#try:
					#	igsighgjihkurgrgf=(spectrum.value.split(" "))
					#	intcounterdatanp=0
					#	intveclist=[0]*(len(igsighgjihkurgrgf))
					#	for v in igsighgjihkurgrgf:
					#		intveclist[intcounterdatanp]=float(v)
					#		intcounterdatanp=intcounterdatanp+1
					#	igsighgjihkurgrgf=(spectrumx.value.split(" "))
					#	intcounterdatanp=0
					#	intveclistx=[0]*len(igsighgjihkurgrgf)
					#	for v in igsighgjihkurgrgf:
					#		intveclistx[intcounterdatanp]=float(v)
					#		intcounterdatanp=intcounterdatanp+1
					#except:
					#	updateddatatryexcept=0
					#if(len(spectrumx.value.split(" "))!=len(spectrum.value.split(" "))):
					#	updateddatatryexcept=0
					#if(updateddatatryexcept==1):
					#	try:
					#		vpeaks=infotext.value.split("Peaks:[")[1].split("]")[0].split(" ")
					#		ipeaks=0
					#		vpeaksnum=[]
					#		for v in vpeaks:
					#			try:
					#				vpeaksnum.append(float(v))
					#			except:
					#				null=None
					#		ipeaks=ipeaks+1
					#		line.set_data(np.asarray(intveclistx), np.asarray(intveclist))
					#		match_y=[0]*(len(vpeaksnum))
					#		match=0
					#		ipeaksb=0
					#		ipeaksc=0
					#		for v in vpeaksnum:
					#			match=abs(intveclistx[0]-v)
					#			ipeaksb=0
					#			for v2 in intveclistx:
					#				if(abs(v-v2)<match):
					#					match=abs(v-v2)
					#					match_y[ipeaksc]=intveclist[ipeaksb]
					#				ipeaksb=ipeaksb+1
					#			ipeaksc=ipeaksc+1
					if(datamodeselected[0]==1):
						#------------------------------------------------------------------
						#------------------------------------------------------------------
						#------------------------------------------------------------------
						#-----------------------Data window mode---------------------------
						#------------------------------------------------------------------
						#------------------------------------------------------------------
						#------------------------------------------------------------------
						j=0
						#print linearr
						while(j<len(linearr)):
							if(bufferlength==len(currentdataarr[j])):
								showlenfotrhisj=currentshowlength.value
								jlenarr=0
								j_is_selected=0
								jb_is_selected=0
								#print selecteddatarows
								for v in selecteddatarows:
									if(j+1==v):
										j_is_selected=1
									if(j==v):
										jb_is_selected=1
								if(jb_is_selected==1):
									if((realtimestampfrompproc1[j-1][-currentshowlength.value:])[0]==0):
										for v in realtimestampfrompproc1[j-1][-currentshowlength.value:]:
											if(v==0):
												showlenfotrhisj=currentshowlength.value-(jlenarr+1)
											jlenarr=jlenarr+1
								#linearr[j-1].set_data(np.arange(bufferlength),np.asarray(currentdataarr[j-1]))
								#old-no-timer-from-source linearr[j-1].set_data(np.arange(currentshowlength.value),np.asarray(currentdataarr[j-1][-currentshowlength.value:]))
								if(startstopupdategraph[0]==1):
									linearr[j-1].set_data((np.asarray(realtimestampfrompproc1[j-1][-showlenfotrhisj:])-realtimestampfrompproc1[j-1][(len(realtimestampfrompproc1[j-1])-1)]),np.asarray(currentdataarr[j-1][-showlenfotrhisj:]))
								#linearr[j-1].set_data(np.asarray(graphtimearray[-currentshowlength.value:]),np.asarray(currentdataarr[j-1][-currentshowlength.value:]))
								if(j_is_selected==1):
									#print j
									is_shown=0
									for vb in linesshown:
										if(vb==j):
											is_shown=1
									if(is_shown==0):
										ax.add_line(linearr[j])
										linesshown.append(j)
								else:
									is_shown=0
									intcnthgsghj=0
									for vb in linesshown:
										if(vb==j):
											is_shown=1
											del linesshown[intcnthgsghj]
										intcnthgsghj=intcnthgsghj+1
									if(is_shown==1):
										ax.lines.remove(linearr[j])
								#print ax.lines.remove(linearr[j])
								#print ax.add_line(linearr[j])
								#print ax.lines
							else:
								print "Error on graph update: bufferlength and currentdataarr[j] are not the same length."
							j=j+1
						#linearr[0].set_data(np.arange(200),np.asarray(currentdataarr))
						#	except:
						#		null=None
						#------------------------------------------------------------------
						#------------------------------------------------------------------
						#------------------------------------------------------------------
						#-------------------End data window mode---------------------------
						#------------------------------------------------------------------
						#------------------------------------------------------------------
						#------------------------------------------------------------------
					else:
						#------------------------------------------------------------------
						#------------------------------------------------------------------
						#------------------------------------------------------------------
						#-------------------Continuous data mode---------------------------
						#------------------------------------------------------------------
						#------------------------------------------------------------------
						#------------------------------------------------------------------
						j=0
						#print linearr
						while(j<len(linearr)):
							#if(bufferlength==len(currentdataarr[j])):
							if(True):
								#linearr[j-1].set_data(np.arange(bufferlength),np.asarray(currentdataarr[j-1]))
								if(startstopupdategraph[0]==1):
									######################linearr[j-1].set_data(np.arange(len(contarray[j-1])),np.asarray(contarray[j-1]))
								j_is_selected=0
								#print selecteddatarows
								for v in selecteddatarows:
									if(j+1==v):
										j_is_selected=1
								if(j_is_selected==1):
									#print j
									is_shown=0
									for vb in linesshown:
										if(vb==j):
											is_shown=1
									if(is_shown==0):
										ax.add_line(linearr[j])
										linesshown.append(j)
								else:
									is_shown=0
									intcnthgsghj=0
									for vb in linesshown:
										if(vb==j):
											is_shown=1
											del linesshown[intcnthgsghj]
										intcnthgsghj=intcnthgsghj+1
									if(is_shown==1):
										ax.lines.remove(linearr[j])
								#print ax.lines.remove(linearr[j])
								#print ax.add_line(linearr[j])
								#print ax.lines
							else:
								print "Error on graph update: bufferlength and currentdataarr[j] are not the same length."
							j=j+1
						#linearr[0].set_data(np.arange(200),np.asarray(currentdataarr))
						#	except:
						#		null=None
						#------------------------------------------------------------------
						#------------------------------------------------------------------
						#------------------------------------------------------------------
						#-------------------End continuous data mode-----------------------
						#------------------------------------------------------------------
						#------------------------------------------------------------------
						#------------------------------------------------------------------
				ax.relim()
				fig.canvas.draw()
		else:
			i[0]=time.time()
		#-------------------------------
		#-------------------------------
		#-------------------------------
		winsize[0]=0
		#-------------------------------
		#-------------------------------
		#-------------------------------
		#i[0]=i[0]+1
		#ax.draw_artist(line)
		#fig.canvas.blit(ax.bbox)
		returnedfalse[0]=1
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
	def loopcontrol(i,autoscaleval,entrytextselectstyle,rect,mousepos,inside,recttext,mousepostext,winsize,timecounterb,infotext,returnedfalse,oldmouseposloopcontrol,selecteddatarows,linesshown,graphtimearray,currentshowlength,datamodeselected,realtimestampfrompproc1):
		#GObject.idle_add(desenhagrafico,i,autoscaleval,entrytextselectstyle,rect,mousepos,inside,recttext,mousepostext,winsize,timecounterb)
		if(returnedfalse[0]==1):
			GObject.timeout_add(50, desenhagrafico,i,autoscaleval,entrytextselectstyle,rect,mousepos,inside,recttext,mousepostext,winsize,timecounterb,infotext,returnedfalse,oldmouseposloopcontrol,selecteddatarows,linesshown,graphtimearray,currentshowlength,datamodeselected,realtimestampfrompproc1,priority=GObject.PRIORITY_LOW)
		return True
	infotext=""
	GObject.timeout_add(100, loopcontrol,i,autoscaleval,entrytextselectstyle,rect,mousepos,inside,recttext,mousepostext,winsize,timecounterb,infotext,returnedfalse,oldmouseposloopcontrol,selecteddatarows,linesshown,graphtimearray,currentshowlength,datamodeselected,realtimestampfrompproc1,priority=GObject.PRIORITY_LOW)

	builder = Gtk.Builder()
	builder.add_from_file(os.path.dirname(os.path.realpath(__file__))+"/ui.glade")
	window = builder.get_object("window1")
	windowc = builder.get_object("about")
	windowbbb = builder.get_object("graphcontrolbar")
	datapointsentrywidget = builder.get_object("datapointsselector")
	listbox = builder.get_object("list")
	listboxcommandstruc=[None]*10
	listboxcommandstruc[0]=0
	listboxcommandstruc[1]=[None]*1
	listboxcommandstruc[2]=[None]*1
	listboxcommandstruc[3]=[None]*1
	rowlabelstruc=[None]*2
	rowlabelstruc[0]=[None]*1
	rowlabelstruc[1]=[None]*1
	def addlistbox(textlabel,listboxvar,listboxcommandstruc,currentinitvarlines):
		addline(currentinitvarlines)
		listboxcommandstruc[1].append(None)
		listboxcommandstruc[2].append(None)
		listboxcommandstruc[3].append(None)
		row = Gtk.ListBoxRow(selectable=False)
		listboxcommandstruc[2][listboxcommandstruc[0]]=row
		hbox = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=5)
		row.add(hbox)
		imageremove = Gtk.Image()
		imageremove.set_from_stock(Gtk.STOCK_REMOVE, Gtk.IconSize.BUTTON)
		imageadd = Gtk.Image()
		imageadd.set_from_stock(Gtk.STOCK_ADD, Gtk.IconSize.BUTTON)
		exec("def intfunction(self):\n\tglobal listboxvar\n\tglobal listboxcommandstruc\n\tglobal imageadd\n\tglobal imageremove\n\tif ((listboxcommandstruc[2]["+str(listboxcommandstruc[0])+"]).is_selected()==True):\n\t\tlistboxvar.unselect_row(listboxcommandstruc[2]["+str(listboxcommandstruc[0])+"])\n\t\t(listboxcommandstruc[2]["+str(listboxcommandstruc[0])+"]).set_selectable(False)\n\t\t(listboxcommandstruc[3]["+str(listboxcommandstruc[0])+"]).set_image(imageadd)\n\telse:\n\t\t(listboxcommandstruc[2]["+str(listboxcommandstruc[0])+"]).set_selectable(True)\n\t\t(listboxcommandstruc[3]["+str(listboxcommandstruc[0])+"]).set_image(imageremove)\n\t\tlistboxvar.select_row(listboxcommandstruc[2]["+str(listboxcommandstruc[0])+"])") in locals()
		listboxcommandstruc[1][listboxcommandstruc[0]]=intfunction
		listbtn=Gtk.Button()
		listbtn.set_image(imageadd)
		listboxcommandstruc[3][listboxcommandstruc[0]]=listbtn
		listbtn.connect("clicked", listboxcommandstruc[1][listboxcommandstruc[0]])
		vbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
		vboxb = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
		hbox.pack_start(vboxb, False, True, 0)
		hbox.pack_start(vbox, True, True, 0)
		hbox.pack_start(listbtn, False, True, 0)
		label1 = Gtk.Label(textlabel, xalign=0)
		entry=Gtk.Entry()
		entry.set_width_chars(1)
		entry.set_max_width_chars(1)
		entry.set_sensitive(False)
		if(listboxcommandstruc[0]<len(linearr)):
			currentcolor=matplotlib.colors.to_rgba(linearr[listboxcommandstruc[0]].get_color())
			gtkcurrentcolor= Gdk.RGBA(currentcolor[0],currentcolor[1],currentcolor[2],1)
			gtkcolorb=Gdk.color_parse(str(linearr[listboxcommandstruc[0]].get_color()))
			entry.modify_bg(Gtk.StateFlags.NORMAL, gtkcolorb)
			entry.modify_bg(Gtk.StateFlags.ACTIVE, gtkcolorb)
			entry.modify_bg(Gtk.StateFlags.SELECTED, gtkcolorb)
			entry.override_color(Gtk.StateFlags.NORMAL, gtkcurrentcolor)
			entry.override_color(Gtk.StateFlags.ACTIVE, gtkcurrentcolor)
			entry.override_color(Gtk.StateFlags.SELECTED, gtkcurrentcolor)
		vboxb.pack_start(entry, False, True, 0)
		#entry.set_size_request(50,30)
		vbox.pack_start(label1, True, True, 0)
		listboxvar.add(row)
		rowlabelstruc[0].append(row)
		rowlabelstruc[1].append(textlabel)
		row.show_all()
		listboxcommandstruc[0]=listboxcommandstruc[0]+1
		return listboxcommandstruc[0]-1
	def removelistboxrow(listboxvar,i,listboxcommandstruc):
		listboxvar.remove(listboxcommandstruc[2][i])
	listboxrowref=[None]*100
	#def sort_func(row_1, row_2, data, notify_destroy):
	#	i=0
	#	sortdata=[None]*2
	#	j=0
	#	for v in rowlabelstruc[0]:
	#		#print v
	#		#print row_1
	#		#print "\n"
	#		if(v==row_1):
	#			sortdata[0]=rowlabelstruc[1][i]
	#			j=i
	#		sortdata[1]=rowlabelstruc[1][j-1]
	#		i=i+1
	#	sortdataold=sortdata
	#	sortdata.sort()
	#	#print sortdataold
	#	print sortdata
	#	#print "\n"
	#	if(sortdataold==sortdata):
	#		return 0
	#	return -1
	#listbox.set_sort_func(sort_func, False, None)
	addedsourcesstruc=[None]*3
	addedsourcesstruc[0]=[None]*1 #datavarname
	addedsourcesstruc[1]=[None]*1 #datavarlength
	addedsourcesstruc[2]=[None]*1 #integerid
	datasourcesupdateinforealname=[None]*0
	def adddatasources(datavar,listbox,listboxcommandstruc,addedsourcesstruc,currentinitvarlines,datasourcesupdateinforealname):
		datavara=datavar.value.split("|")
		del datavara[0]
		for v in datavara:
			datavarb=v.split(":")
			datavarname=datavarb[0]
			if(datavarb[1]!=""):
				datavarlength=len(datavarb[1].split(" "))
			else:
				datavarlength=0
			#datavarname, datavarlength
			isnameadded=0
			index=-1
			i=0
			for vb in addedsourcesstruc[0]:
				if(vb==datavarname):
					isnameadded=1
					index=i
				i=i+1
			if(isnameadded==1):
				if(addedsourcesstruc[1][index]<datavarlength):
					#add missing elements
					i=addedsourcesstruc[1][index]+1
					lastint=addedsourcesstruc[2][index]
					while(i<=datavarlength):
						lastint.append(addlistbox(str(datavarname)+" peak:"+str(i),listbox,listboxcommandstruc,currentinitvarlines))
						datasourcesupdateinforealname.append(str(datavarname)+"|"+str(i))
						i=i+1
					addedsourcesstruc[0].append(str(datavarname))
					addedsourcesstruc[1].append(datavarlength)
					addedsourcesstruc[2].append(lastint)
				if(addedsourcesstruc[1][index]>datavarlength):
					#remove extra elements
					ndataremove=abs(addedsourcesstruc[1][index]-datavarlength)
					lastint=addedsourcesstruc[2][index]
					i=len(lastint)-1
					while(ndataremove>0):
						removelistboxrow(listbox,addedsourcesstruc[2][index][i],listboxcommandstruc)
						del lastint[i]
						ndataremove=ndataremove-1
						i=i-1
					addedsourcesstruc[2][index]=lastint
					addedsourcesstruc[1][index]=datavarlength
			else:
				#add all elements
				i=1
				lastint=[None]*1
				while(i<=datavarlength):
					lastint.append(addlistbox(str(datavarname)+" peak:"+str(i),listbox,listboxcommandstruc,currentinitvarlines))
					datasourcesupdateinforealname.append(str(datavarname)+"|"+str(i))
					i=i+1
				addedsourcesstruc[0].append(str(datavarname))
				addedsourcesstruc[1].append(datavarlength)
				addedsourcesstruc[2].append(lastint)
		return True
	import random
	datavaronlyone.value=datavar.value
	lastupdatetime=[time.time()]*1
	pointdeleteval=[0]*1
	currentpointtime=[0]*1
	dataupdaterunning=[0]*1
	def updatedata(datavar,datavaronlyone,currentdataarr,datasourcesupdateinforealname,lastupdatetime,graphtimearray,starttime,pointdeleteval,totaldataarr,currentpointtime,dataupdaterunning,realtimestampfrompproc1):
		newdatavara=datavar.value.split(",")
		datavar.value=""
		for newdatavarb in newdatavara:
			if(newdatavarb!=""):
				datavaronlyone.value=newdatavarb
				#print datasourcesupdateinforealname
				cdataprim=newdatavarb.split("|")
				i=0
				del cdataprim[0]
				cdataprimnames=newdatavarb.split("|")
				cdataprimtimes=newdatavarb.split("|")
				del cdataprimnames[0]
				del cdataprimtimes[0]
				while(i<len(cdataprim)):
					cdataprim[i]=((cdataprim[i]).split(":")[1]).split(" ")
					cdataprimtimes[i]=((cdataprimtimes[i]).split(":")[1]).split(" ")
					ncountaaa=0
					while(ncountaaa<len(cdataprim[i])):
						cdataprim[i][ncountaaa]=cdataprim[i][ncountaaa].split(";")[0]
						cdataprimtimes[i][ncountaaa]=cdataprimtimes[i][ncountaaa].split(";")[1]
						#print cdataprimtimes[i][ncountaaa]
						ncountaaa=ncountaaa+1
					cdataprimnames[i]=((cdataprimnames[i]).split(":")[0])
					i=i+1
				#print cdataprimnames
				#j=0
				#while(j<len(currentdataarr)):
				#	currentdataarr[j].append(random.random()+j)
				#	del currentdataarr[j][0]
				#	j=j+1
				#print currentdataarr
				#print cdataprim
				#print cdataprimnames
				#print datasourcesupdateinforealname
				if(abs(lastupdatetime[0]-time.time())>=0.0):
					#print cdataprimnames
					#print cdataprim
					if(1==1):
						graphtimearray.append(currentpointtime[0])
						del graphtimearray[0]
						contimearr.append(currentpointtime[0])
					i=0
					currentpointtime[0]=currentpointtime[0]+1
					for v in cdataprimnames:
						j=0
						cdataprimtimescounter=0
						for vb in cdataprim[i]:
							cval=-1
							kb=0
							for k in datasourcesupdateinforealname:
								if(k==v+"|"+str(j+1)):
									cval=kb
									#print(v+"|"+str(j+1)+"\n"+k)
								kb=kb+1
							#print("found:"+str(cval)+"\n\n")
							if(cval!=-1):
								if(1==1):
									realtimestampfrompproc1[cval].append(float(cdataprimtimes[i][cdataprimtimescounter]))
									del realtimestampfrompproc1[cval][0]
									currentdataarr[cval].append(float(vb))
									del currentdataarr[cval][0]
									contarray[cval].append(float(vb))
									#totaldataarr[cval].append(float(vb))
									#-----------------------------------------------------------
									#------------reduce continuous array size-------------------
									#TODO:Fix this.	This does not work!!!
									if(len(contarray[cval])>1000):
										if(pointdeleteval[0]<len(contarray[cval])):
											del contarray[cval][pointdeleteval[0]]
											pointdeleteval[0]=pointdeleteval[0]+3
										else:
											pointdeleteval[0]=0
											#intdataupdatereducearr=int(len(totaldataarr[cval])/1000.0)
											#contarray[cval]=(np.asarray(totaldataarr[cval])[:len(totaldataarr[cval])/intdataupdatereducearr*intdataupdatereducearr].reshape(-1,intdataupdatereducearr).mean(1)).tolist()
									#----------end reduce continuous array size-----------------
									#-----------------------------------------------------------
							else:
								print "err"
							#print cval
							#print vb
							j=j+1
							cdataprimtimescounter=cdataprimtimescounter+1
						i=i+1
					lastupdatetime[0]=time.time()
		return True
	def changeselectedvar(a,b,selecteddatarowstemp):
		i=0
		for v in rowlabelstruc[0]:
			if(v==b):
				selecteddatarowstemp.append(i)
			i=i+1
	def check_selected_itens(listbox,selecteddatarows):
		selecteddatarowstemp=[None]*0
		listbox.selected_foreach(changeselectedvar,selecteddatarowstemp)
		i=0
		#while(i<len(selecteddatarowstemp)):
		#	selecteddatarows[i]=selecteddatarowstemp[i]
		#	i=i+1
		#while(i<len(selecteddatarows)):
		#	selecteddatarows[i]=None
		#	i=i+1
		while(i<len(selecteddatarows)):
			del selecteddatarows[i]
			i=i+1
		i=0
		while(i<len(selecteddatarowstemp)):
			selecteddatarows.append(selecteddatarowstemp[i])
			i=i+1
		return True
	GObject.timeout_add(500, adddatasources,datavaronlyone,listbox,listboxcommandstruc,addedsourcesstruc,currentinitvarlines,datasourcesupdateinforealname,priority=GObject.PRIORITY_LOW)
	GObject.timeout_add(1, updatedata,datavar,datavaronlyone,currentdataarr,datasourcesupdateinforealname,lastupdatetime,graphtimearray,starttime,pointdeleteval,totaldataarr,currentpointtime,dataupdaterunning,realtimestampfrompproc1,priority=GObject.PRIORITY_LOW)
	GObject.timeout_add(100, check_selected_itens,listbox,selecteddatarows,priority=GObject.PRIORITY_LOW)
	#addlistbox("wrgwgr",listbox,listboxcommandstruc)
	#addlistbox("dfh",listbox,listboxcommandstruc)
	#addlistbox("wrghfdhfwgr",listbox,listboxcommandstruc)
	#addlistbox("wrgfhwgr",listbox,listboxcommandstruc)
	windowbbb.pack_start(inmatplotlibbox, True, True, 0)
	windowbbb.pack_start(hboxb, False, False, 0)
	help=builder.get_object("helpmenu")
	about=builder.get_object("aboutmenu")
	helpwin=builder.get_object("helpwin")
	radiobutton1=builder.get_object("radiobutton1")
	radiobutton2=builder.get_object("radiobutton2")
	#radiobutton2.set_active(True)
	#datamodeselected[0]=1
	class Handler:
		def closeaboutdialog(self,*args):
			windowc.hide()
		def openabout(self,*args):
			windowc.show()
		def openhelp(self,*args):
			helpwin.show()
		def close(self,*args):
			helpwin.hide()
		def saveas(self,*args):
			#helpwin.hide()
			print "save"
		def quitapp(self,*args):
			windowclose.value="closed"
			window.destroy()
		def startstop(self,*args):
			if(startstopupdategraph[0]==1):
				startstopupdategraph[0]=0
			else:
				startstopupdategraph[0]=1
		def changesize(self,*args):
			global currentshowlength
			currentshowlength.value=int(datapointsentrywidget.get_value())
		def changedradio(self,*args):
			#global datamodeselected
			if(radiobutton1.get_active()==True):
				datamodeselected[0]=0
			else:
				datamodeselected[0]=1
		def clear(self,*args):
			i=0
			while(i<len(graphtimearray)):
				j=0
				if(type(graphtimearray[i])==list):
					while(j<len(graphtimearray[i])):
						graphtimearray[i][j]=0
						j=j+1
				else:
					graphtimearray[i]=0
				i=i+1
			i=0
			while(i<len(contimearr)):
				j=0
				if(type(contimearr[i])==list):
					while(j<len(contimearr[i])):
						contimearr[i][j]=0
						j=j+1
				else:
					contimearr[i]=0
				i=i+1
			i=0
			while(i<len(currentdataarr)):
				j=0
				if(type(currentdataarr[i])==list):
					while(j<len(currentdataarr[i])):
						currentdataarr[i][j]=0
						j=j+1
				else:
					currentdataarr[i]=0
				i=i+1
			i=0
			while(i<len(realtimestampfrompproc1)):
				j=0
				if(type(realtimestampfrompproc1[i])==list):
					while(j<len(realtimestampfrompproc1[i])):
						realtimestampfrompproc1[i][j]=0
						j=j+1
				else:
					realtimestampfrompproc1[i]=0
				i=i+1
			i=0
			while(i<len(contarray)):
				j=0
				if(type(contarray[i])==list):
					while(j<len(contarray[i])):
						contarray[i][j]=0
						j=j+1
				else:
					contarray[i]=0
				i=i+1
	windowc.hide()
	windowc.show_all()
	windowc.hide()
	helpwin.hide()
	helpwin.show_all()
	helpwin.hide()
	builder.connect_signals(Handler())
	fastmmap.write(mmap,",03main window load")
	time.sleep(3)
	fastmmap.write(mmap,",\2")
	window.show_all()
	Gtk.main()
	window.destroy()
	windowclose.value="closed"
managerb = Manager()
currentshowlength= managerb.Value(c_int, 200)
import math
def mainuiload(processid):
	global mmap
	global currentshowlength
	global managerb
	windowclose = managerb.Value(c_char_p, "opened")
	datavar = managerb.Value(c_char_p, "|qimon:1;0 2;0 3;0 4;0 5;0 6;0|spectraread:7;0 8;0 9;0,")
	datavaronlyone = managerb.Value(c_char_p, "")
	p2=Process(target=initgui, args=(windowclose,datavar,datavaronlyone,currentshowlength,))
	mmap=fastmmap.connectmmap("peakss",""+str(processid))
	while mmap==-1:
		mmap=fastmmap.connectmmap("peakss",""+str(processid))
		print("Connection failed...Reconnecting")
		time.sleep(0.1)
	fastmmap.write(mmap,",03starting gui")
	time.sleep(0.1)
	p2.start()
	i=0
	#while(windowclose.value=="opened"):
	#	if(i>9):
	#		datavar.value="|qimon:1 2 3 4 5 6|spectraread:7 8 9 10,|qimon:1 2 3 4 5 6|spectraread:7 8 9 10,|qimon:1 2 3 4 5 6|spectraread:7 8 9 10,"
	#	if(i>17):
	#		datavar.value="|qimon:1 2 3 4 5 6 7|spectraread:8 9 10 11,"
	#	if(i>25):
	#		datavar.value="|spectraread:8 9 10|qimon:1 2 3 4 5 6 7,"
	#	if(i>30):
	#		datavar.value="|spectraread:8 9|qimon:1 2 3 4 5 6 7,"
	#	if(i>35):
	#		datavar.value="|spectraread:7 8 9|qimon:1 2 3 4 5 6,"
	#	i=i+1
	#	time.sleep(1)
	aaa=0
	j=0
	timectn=0
	while(windowclose.value=="opened"):
		if(aaa<200):
			a=""
			a=a+"|qimon:"
			i=0
			while(i<5):
				a=a+str(i+random.random())+";"+str(timectn)+" "
				i=i+1
			a=a+str(i+random.random())+";"+str(timectn)+""
			a=a+"|spectraread:"
			while(i<11):
				a=a+str(i+random.random())+";"+str(timectn)+" "
				i=i+1
			a=a+str(i+random.random())+";"+str(timectn)+""
			a=a+","
			datavar.value=datavar.value+a
			time.sleep(0.01)
			a=""
			aaa=aaa+1
		else:
			if(aaa<300):
				a=""
				a=a+"|qimon:"
				i=0
				while(i<2):
					a=a+str(i+random.random())+";"+str(timectn)+" "
					i=i+1
				a=a+str(i+random.random())+";"+str(timectn)+""
				a=a+"|spectraread:"
				while(i<3):
					a=a+str(i+random.random())+";"+str(timectn)+" "
					i=i+1
				a=a+str(i+random.random())+";"+str(timectn)+""
				a=a+","
				datavar.value=datavar.value+a
				time.sleep(0.01)
				aaa=aaa+1
			else:
				a=""
				a=a+"|qimon:"
				i=0
				while(i<4):
					a=a+str(i+23*math.sin(j/20.0))+";"+str(timectn)+" "
					i=i+1
				a=a+str(i+random.random())+";"+str(timectn)+""
				a=a+"|spectraread:"
				j=j+1
				while(i<7):
					a=a+str(i+random.random())+";"+str(timectn)+" "
					i=i+1
				a=a+str(i+random.random())+";"+str(timectn)+""
				a=a+","
				datavar.value=datavar.value+a
				time.sleep(0.01)
		timectn=timectn+1
	print "terminate"
	p2.terminate()
