import os
import subprocess
from multiprocessing import Process, Manager, Value, Array
from ctypes import c_char_p
import time
def initprocui(procid):
	while(procid.value==""):
		pass
	print os.path.realpath(__file__)
	#os.chdir("..")
	#__import__(os.path.realpath(__file__).replace("/main-uiproc.pyc","").replace("/main-uiproc.py","")+"/main-ui.py")
	from mainui import mainload,stopacquisition,readyshowwin,getsavecommand,getselectedvalues,clearsavecommand,updateinfopeaks,updateinfotext,updategraphspectrumvariable,iswindowclosed,fastmmap
	mapida=fastmmap.createmmap("sfhdfghdbfdb"+procid.value,"rwx------")
	up=0
	while True:
		n=fastmmap.read(mapida,0)
		b=[]
		if(up==0):
			time.sleep(0.1)
		else:
			time.sleep(0.02)
			up=0
		if (n.count("Mainload-")>0):
			b=n.split("Mainload-")
			#print b
		for command in b:
			if(command.count("load")>0):
				#print "load"
				mainload(command.split("load")[1].replace(" ",""))
			if(command=="readyshowwin "):
				#print "readyshowwin"
				readyshowwin()
			if(command=="clearsavecommand "):
				clearsavecommand()
				a=getsavecommand()+"--main-uiproc.pydataseparetor657576--"+getselectedvalues()+"--main-uiproc.pydataseparetor657576--"+iswindowclosed()
				fastmmap.writesharedstring(mapida,a)
			if(command=="upui "):
				up=1
			if(command=="stopacquisition "):
				stopacquisition()
				up=1
			if(command.count("updateinfopeaks")>0):
				updateinfopeaks(command.split("updateinfopeaks")[1].split("spectrareadintterminator")[0])
				up=1
			if(command.count("updateinfotext")>0):
				up=1
				#print "load"
				#print command.split("updateinfotext")[1].split("/intspectrareadsepghff")[0]
				try:
					xylabeldata=command.split("updateinfotext")[1].split("/intspectrareadsepghff")[1].split("--spectrareadsepinfoxylabel--")[1]
				except:
					xylabeldata=""
				#print command.split("updateinfotext")[1].split("/intspectrareadsepghff")[2]
				updateinfotext(command.split("updateinfotext")[1].split("/intspectrareadsepghff")[0],command.split("updateinfotext")[1].split("/intspectrareadsepghff")[1].split("--spectrareadsepinfoxylabel--")[0],command.split("updateinfotext")[1].split("/intspectrareadsepghff")[2],xylabeldata)
			if(command.count("updategraphspectrumvariable")>0):
				#print "load"
				updategraphspectrumvariable(command.split("updategraphspectrumvariable")[1].split("/intspectrareadsepghff")[0],command.split("updategraphspectrumvariable")[1].split("/intspectrareadsepghff")[1])
		if(fastmmap.getsharedstring(mapida).split("--main-uiproc.pydataseparetor657576--")[0]!="Falseb"):
			a=getsavecommand()+"--main-uiproc.pydataseparetor657576--"+getselectedvalues()+"--main-uiproc.pydataseparetor657576--"+iswindowclosed()
			fastmmap.writesharedstring(mapida,a)
		
def loadinta(procida):
	managerb = Manager()
	procid = managerb.Value(c_char_p, "")
	procid.value=procida
	p2=Process(target=initprocui, args=(procid,))
	p2.start()
