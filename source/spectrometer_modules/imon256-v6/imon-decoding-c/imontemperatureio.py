import time
from imonpreferences import *
def readtempe():
	if(imonrawread.inwaiting()!=0):
		if(debuglevel==1):
			print "Error! Buffer not empty. Flushing buffer before temperature measurement"
		if(debuglevel==2):
			print "Error! Buffer not empty. Flushing buffer before temperature measurement"
		while(imonrawread.inwaiting()>0):
			null=imonrawread.read(imonrawread.inwaiting())
		time.sleep(0.05)
		while(imonrawread.inwaiting()>0):
			null=imonrawread.read(imonrawread.inwaiting())
	try:
		imonrawread.write("\x2a\x4d\x45\x41\x53\x3a\x54\x45\x4d\x50\x45\x0d")
		ireadtempe=0
		tempe=""
		while(ireadtempe<90):
			ireadtempe=0
			tempebyteread=imonrawread.read(1)
			tempe=tempe+tempebyteread
			if(tempebyteread=="\n"): #Imon sends \r\n, so the presence of \n means that data transmission has ended
				ireadtempe=91
				break
			ireadtempe=ireadtempe+1
		if(debuglevel==2):
			print tempe
		def customnumcheck(value):
			if(value.replace(".","").isdigit()==True):
				return True
			return False
		readdatatempefloat=float(([float(s) for s in tempe.split() if customnumcheck(s)])[0])
	except Exception as e:
		if(debuglevel==1):
			print e
		if(debuglevel==2):
			print e
		return None
	if(imonrawread.inwaiting()!=0):
		if(debuglevel==1):
			print "Error! Buffer not empty. Flushing buffer after temperature measurement"
		if(debuglevel==2):
			print "Error! Buffer not empty. Flushing buffer after temperature measurement"
		while(imonrawread.inwaiting()>0):
			null=imonrawread.read(imonrawread.inwaiting())
		time.sleep(0.05)
		while(imonrawread.inwaiting()>0):
			null=imonrawread.read(imonrawread.inwaiting())
	return readdatatempefloat
