import sys
import time
if not hasattr(sys, 'argv'):
	sys.argv  = ['']
import os
import fastmmap
import signal
import warnings
#warnings.filterwarnings("ignore")
#sys.tracebacklimit=0
loaded_spectrometer_modules=[None]*1
loaded_spectrometer_names=[None]*1
import_counter=0
import json
import re
print os.popen("groups").read()
spectrometer_modules_dir = "spectrometer_modules"
for child in os.listdir(spectrometer_modules_dir):
	current_spectrometer_module = os.path.join(spectrometer_modules_dir, child)
	if os.path.isdir(current_spectrometer_module):
		loaded_spectrometer_modules.append( __import__(""+current_spectrometer_module.split("/")[1]+".control_module", fromlist=['']))
		loaded_spectrometer_names.append( ""+current_spectrometer_module.split("/")[1])
		import_counter=import_counter+1
currentdeviceinterfacefunctions=None
devinfodata=""
cmdargs=""
cmdargsb=""
dataclickmap=-1
errmap=-1
datamapaaa=-1
datamapaaab=-1
datamaperrfinalb=-1
dropspec=0
idinta=-1
guistata=[None]*8
guistatb=[]
globalfreq=0
globaltemp=0
globalstat=""
timelastspec=time.time()
aaa=1
datamaphs=-1
def connectm(cmdargs):
	global errmap
	global datamapaaa
	global datamapaaab
	global idinta
	global cmdargsb
	global dataclickmap
	global datamaphs
	global datamaphsa
	global datamaphsb
	global datamaphsc
	global datamaphsd
	global datamaphse
	global datamaphsf
	global datamaphsg
	global datamaphsh
	global datamaperrfinalb
	datamapaaa=fastmmap.createmmap("k"+str(cmdargs),"rwx------") #Aqui
	datamapaaab=fastmmap.createmmap("?"+str(cmdargs),"rwx------") #Aqui
	datamaperrfinalb=fastmmap.createmmap("errf"+str(cmdargs),"rwx------") #Aqui
	dataclickmap=fastmmap.createmmap("L"+str(cmdargs),"rwx------")
	datamaphs=fastmmap.createmmap("["+str(cmdargs),"rwx------")
	datamaphsa=fastmmap.createmmap("{"+str(cmdargs),"rwx------")
	datamaphsb=fastmmap.createmmap("("+str(cmdargs),"rwx------")
	datamaphsc=fastmmap.createmmap("~"+str(cmdargs),"rwx------")
	datamaphsd=fastmmap.createmmap(","+str(cmdargs),"rwx------")
	#datamaphse=fastmmap.createmmap("'"+str(cmdargs),"rwx------")
	#datamaphsf=fastmmap.createmmap("%"+str(cmdargs),"rwx------")
	#datamaphsg=fastmmap.createmmap("#"+str(cmdargs),"rwx------")
	#datamaphsh=fastmmap.createmmap(":"+str(cmdargs),"rwx------")
	errmap=fastmmap.connectmmap(os.getcwd()+"/spectrareadd "+os.getcwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"d"+cmdargs)
	while(errmap==-1):
		errmap=fastmmap.connectmmap(os.getcwd()+"/spectrareadd "+os.getcwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"d"+cmdargs)
		time.sleep(0.1)
	idinta=fastmmap.connectmmap(os.getcwd()+"/spectrareadd "+os.getcwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"a"+cmdargs)
	while(idinta==-1):
		idinta=fastmmap.connectmmap(os.getcwd()+"/spectrareadd "+os.getcwd()+"/main.js spectrareadcurrentprocid:"+cmdargs,"a"+cmdargs)
		time.sleep(0.1)
	cmdargsb=cmdargs
def showerr(errdata):
	global errmap
	global datamapaaa
	global idinta
	if(datamapaaa!=-1):
		if(errmap!=-1):
			if(idinta!=-1):
				fastmmap.writesharedstring(datamapaaa,fastmmap.getsharedstring(datamapaaa)+",stopaq:");
				fastmmap.writesharedstring(datamaperrfinalb,",stopaq:");
				fastmmap.write(errmap,"-"+errdata+"_");
def showwarning(errdata):
	global errmap
	global datamapaaa
	global idinta
	if(datamapaaa!=-1):
		if(errmap!=-1):
			if(idinta!=-1):
				fastmmap.write(errmap,"-"+errdata+"_");
def pushvalgui(xstring,ystring):
	global globalfreq
	global globaltemp
	global globalstat
	global errmap
	global datamapaaa
	global datamapaaab
	global idinta
	if(datamapaaa!=-1):
		if(errmap!=-1):
			if(idinta!=-1):
				if(len(xstring.split(" "))==len(ystring.split(" "))):
					fastmmap.writesharedstring(datamapaaab,";buffer:"+str(xstring)+"?"+str(ystring)+"final")
				else:
					#print xstring
					#print ystring
					print "pushval(xstring,ystring) error,\n\txstring.split(" ") and ystring.split(" ") must have the same length."
				fastmmap.writesharedstring(datamapaaa,";frequency:"+str(globalfreq)+",deviceinfo:"+str(globaltemp)+"--int-info-seperator--     "+str(globalstat)+" Dropped spectra: "+str(dropspec));
def getguistat():
	global guistata
	global guistatb
	global errmap
	global datamapaaa
	global idinta
	try:
		if(datamapaaa!=-1):
			if(errmap!=-1):
				if(idinta!=-1):
					datagui=fastmmap.getsharedstring(idinta)
					try:
						indexof=datagui.index(",")
					except:
						indexof=-1
					if(indexof!=-1):
						try:
							indexof=datagui.index(",")
						except:
							indexof=-1
						if(indexof!=-1):
							v1t=datagui.split(",")[1].split("----------")[0].split(";");
							v2t=datagui.split(",")[1].split("----------")[1].split(";");
							if(v1t!=""):
								if(v2t!=""):
									if(len(v1t)==5):
										if(len(v2t)==5):
											guistata=v1t
											guistata.append(v2t[len(v2t)-1])
											guistatb=v2t[:-1]
	except:
		null=None
	return [guistata,guistatb]
freqcont=0
timefreqcont=time.time()
globvstatinitp=getguistat()
globtimeupdatevstatinitp=time.time()
globcurrentdriver="None"
globolddriver="None"
terminatesignal=0
def closeunloadevent(null,nullb):
	global currentdeviceinterfacefunctions
	global cmdargsb
	global terminatesignal
	if(terminatesignal==0):
		if(currentdeviceinterfacefunctions!=None):
			if(currentdeviceinterfacefunctions!="None"):
				try:
					currentdeviceinterfacefunctions.unload(cmdargsb)
				except Exception as error:
					print("Error on function unload in "+str(globcurrentdriver)+" control module:\n"+str(type(error))+"\n"+str(error))
					null=None
	terminatesignal=1
	time.sleep(3)
	sys.exit(0)
signal.signal(signal.SIGTERM, closeunloadevent)
testi=0
lastdevid=""
def initp():
	global freqcont
	global timefreqcont
	global timelastspec
	global globalfreq
	global globaltemp
	global globalstat
	global globvstatinitp
	global globtimeupdatevstatinitp
	global globolddriver
	global globcurrentdriver
	global currentdeviceinterfacefunctions
	global loaded_spectrometer_modules
	global loaded_spectrometer_names
	global cmdargsb
	global terminatesignal
	global datamaphs
	global testi
	global 	dropspec
	global datamaphsa
	global datamaphsb
	global datamaphsc
	global datamaphsd
	global datamaphse
	global datamaphsf
	global datamaphsg
	global datamaphsh
	global lastdevid
	makesingle=False
	timestamp=0
	try:
		while(terminatesignal==0):
			if(True):
				if(globcurrentdriver=="None"):
					time.sleep(0.3)
					globalfreq=0
				if((fastmmap.read(dataclickmap,0)).find("c")!=-1):
					makesingle=True
				else:
					makesingle=False
				if(time.time()-globtimeupdatevstatinitp>0.1):
					globvstatinitp=getguistat()
					globtimeupdatevstatinitp=time.time()
					globcurrentdriver=str(globvstatinitp[0][2])
				runfunc=0
				if ((globolddriver==globcurrentdriver) and (globvstatinitp[0][5]==lastdevid)):
					retv=[""]*5	
					if(globvstatinitp[0][4]=="False"):
						globalfreq=0
						try:
							retv=currentdeviceinterfacefunctions.mainloop(False,makesingle,globvstatinitp[1])
						except Exception as error:
							print("Error on function mainloop in "+str(globcurrentdriver)+" control module:\n"+str(type(error))+"\n"+str(error))
						runfunc=1
					if(globvstatinitp[0][4]=="True"):
						try:
							retv=currentdeviceinterfacefunctions.mainloop(True,makesingle,globvstatinitp[1])
						except Exception as error:
							print("Error on function mainloop in "+str(globcurrentdriver)+" control module:\n"+str(type(error))+"\n"+str(error))
						runfunc=1
					if(runfunc==1):
						if(retv!=False):
							if(abs(timelastspec-time.time())>0.01):
								if(retv[0]!=""):
									if(retv[1]!=""):
										pushvalgui(retv[0],retv[1])
										timelastspec=time.time()
							if(retv[0]!=""):
								if(retv[1]!=""):
									if(testi==0):
										if(fastmmap.write(datamaphsa,";"+str(retv[0])+"?"+str(timestamp)+"?"+str(retv[1])+";")==-1):
											if(fastmmap.write(datamaphsc,";"+str(retv[0])+"?"+str(timestamp)+"?"+str(retv[1])+";")==-1):
												dropspec=dropspec+1
									if(testi==1):
										if(fastmmap.write(datamaphsb,";"+str(retv[0])+"?"+str(timestamp)+"?"+str(retv[1])+";")==-1):
											if(fastmmap.write(datamaphs,";"+str(retv[0])+"?"+str(timestamp)+"?"+str(retv[1])+";")==-1):
												dropspec=dropspec+1
									if(testi==2):
										if(fastmmap.write(datamaphs,";"+str(retv[0])+"?"+str(timestamp)+"?"+str(retv[1])+";")==-1):
											if(fastmmap.write(datamaphsb,";"+str(retv[0])+"?"+str(timestamp)+"?"+str(retv[1])+";")==-1):
												dropspec=dropspec+1
									if(testi==3):
										if(fastmmap.write(datamaphsd,";"+str(retv[0])+"?"+str(timestamp)+"?"+str(retv[1])+";")==-1):
											if(fastmmap.write(datamaphsb,";"+str(retv[0])+"?"+str(timestamp)+"?"+str(retv[1])+";")==-1):
												dropspec=dropspec+1
									if(testi==4):
										if(fastmmap.write(datamaphsc,";"+str(retv[0])+"?"+str(timestamp)+"?"+str(retv[1])+";")==-1):
											if(fastmmap.write(datamaphs,";"+str(retv[0])+"?"+str(timestamp)+"?"+str(retv[1])+";")==-1):
												dropspec=dropspec+1
									if(testi==5):
										if(fastmmap.write(datamaphsb,";"+str(retv[0])+"?"+str(timestamp)+"?"+str(retv[1])+";")==-1):
											if(fastmmap.write(datamaphsc,";"+str(retv[0])+"?"+str(timestamp)+"?"+str(retv[1])+";")==-1):
												dropspec=dropspec+1
									if(testi==6):
										if(fastmmap.write(datamaphs,";"+str(retv[0])+"?"+str(timestamp)+"?"+str(retv[1])+";")==-1):
											if(fastmmap.write(datamaphsa,";"+str(retv[0])+"?"+str(timestamp)+"?"+str(retv[1])+";")==-1):
												dropspec=dropspec+1
									if(testi==7):
										if(fastmmap.write(datamaphsc,";"+str(retv[0])+"?"+str(timestamp)+"?"+str(retv[1])+";")==-1):
											if(fastmmap.write(datamaphsa,";"+str(retv[0])+"?"+str(timestamp)+"?"+str(retv[1])+";")==-1):
												dropspec=dropspec+1
									if(testi==8):
										if(fastmmap.write(datamaphsd,";"+str(retv[0])+"?"+str(timestamp)+"?"+str(retv[1])+";")==-1):
											if(fastmmap.write(datamaphsa,";"+str(retv[0])+"?"+str(timestamp)+"?"+str(retv[1])+";")==-1):
												dropspec=dropspec+1
									if(testi==9):
										if(fastmmap.write(datamaphsa,";"+str(retv[0])+"?"+str(timestamp)+"?"+str(retv[1])+";")==-1):
											if(fastmmap.write(datamaphsb,";"+str(retv[0])+"?"+str(timestamp)+"?"+str(retv[1])+";")==-1):
												dropspec=dropspec+1
										testi=-1
									timestamp=timestamp+1
									testi=testi+1
									freqcont=freqcont+1
							if(retv[2]!=""):
								showerr(retv[2])
							if(retv[3]!=""):
								showwarning(retv[3])
							if(retv[4]!=""):
								globaltemp=retv[4]
							if(abs(time.time()-timefreqcont)>1):
								globalfreq=freqcont
								freqcont=0
								timefreqcont=time.time()
				else:
					if(currentdeviceinterfacefunctions!=None):
						if(currentdeviceinterfacefunctions!="None"):
							try:
								currentdeviceinterfacefunctions.unload(cmdargsb)
							except Exception as error:
								print("Error on function unload in "+str(globcurrentdriver)+" control module:\n"+str(type(error))+"\n"+str(error))
					globolddriver=globcurrentdriver
					imodloader=0
					globaltemp="--"
					for v in loaded_spectrometer_names:
						if(str(v)==str(globcurrentdriver)):
							if(str(v)!="None"):
								if(v!=None):
									currentdeviceinterfacefunctions=loaded_spectrometer_modules[imodloader]
						imodloader=imodloader+1
					try:
						if(currentdeviceinterfacefunctions!=None):
							globvstatinitp=getguistat()
							lastdevid=globvstatinitp[0][5]
							currentdeviceinterfacefunctions.load(cmdargsb,globvstatinitp[0][5])
					except Exception as error:
						print("Error on function load in "+str(globcurrentdriver)+" control module:\n"+str(type(error))+"\n"+str(error))
	finally:
		print "Daemon unloading"
		return 0
