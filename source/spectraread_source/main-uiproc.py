import os
import subprocess
from multiprocessing import Process, Manager, Value, Array
import time
def initprocui():
	print os.path.realpath(__file__)
	#os.chdir("..")
	#__import__(os.path.realpath(__file__).replace("/main-uiproc.pyc","").replace("/main-uiproc.py","")+"/main-ui.py")
	from mainui import mainload,stopacquisition,readyshowwin,getsavecommand,getselectedvalues,clearsavecommand,updateinfopeaks,updateinfotext,updategraphspectrumvariable,iswindowclosed,fastmmap
	mapida=fastmmap.createmmap("sfhdfghdbfdb","rwx------")#XXX XXX XXX TODO add pid to mmap name
	while True:
		a=getsavecommand()+"--main-uiproc.pydataseparetor657576--"+getselectedvalues()+"--main-uiproc.pydataseparetor657576--"+iswindowclosed()
		fastmmap.writesharedstring(mapida,a)
		a=fastmmap.read(mapida,0)
		b=[]
		if (a.count("Mainload-")>0):
			b=a.split("Mainload-")
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
			if(command=="stopacquisition "):
				stopacquisition()
			if(command.count("updateinfopeaks")>0):
				updateinfopeaks(command.split("updateinfopeaks")[1].split("spectrareadintterminator")[0])
			if(command.count("updateinfotext")>0):
				#print "load"
				updateinfotext(command.split("updateinfotext")[1].split("/intspectrareadsepghff")[0],command.split("updateinfotext")[1].split("/intspectrareadsepghff")[1],command.split("updateinfotext")[1].split("/intspectrareadsepghff")[2])
			if(command.count("updategraphspectrumvariable")>0):
				#print "load"
				updategraphspectrumvariable(command.split("updategraphspectrumvariable")[1].split("/intspectrareadsepghff")[0],command.split("updategraphspectrumvariable")[1].split("/intspectrareadsepghff")[1])
		
p2=Process(target=initprocui, args=())
p2.start()
