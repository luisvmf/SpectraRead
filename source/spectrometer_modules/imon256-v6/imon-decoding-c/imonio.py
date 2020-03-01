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

import imondecode
from imonpreferences import *
import imonrawread
import time
import imoncalibration
import fastmmap
imoncalibration.imonrawread=imonrawread
fastmmapmqid=-1
lastusedparameters=[-1,-1,-1] #Frequency, tint, nspec
def sendcalibrationdata():
	fastmmap.writesharedstring(fastmmapmqid,str(imoncalibration.getxaxiscalibrateddata()))
def initdecodingmmap(identifier):
	global fastmmapmqid
	imondecode.addhexdata("none",identifier)
	fastmmapmqid=fastmmap.createmmap("funcwave"+identifier,"rwx------")
def addhexdataskip(hexdata):
	imondecode.addhexdata(hexdata.replace("\x01","\x01\x03").replace("\x00","\x01\x02"))
def readraw(i):
	if(imonrawread.inwaiting()!=0):
		queuesize=imonrawread.inwaiting()
		if(i+queuesize>1020):
			if(debuglevel==2):
				print "reading:"+str(1020-i)+" read:"+str(i)+" to read:"+str(queuesize)
			queuesize=1020-i
		else:
			if(debuglevel==2):
				print "reading:"+str(queuesize)+" read:"+str(i)+" to read:"+str(queuesize)
		return [imonrawread.read(queuesize),queuesize]
	else:
		return [None,0]
def readonespectra():
	i=0
	removedsyncmarc=0
	failed=0
	inittime=time.time()
	while(i<1020):
		auxspec=readraw(i)
		if(auxspec[1]!=0):
			addhexdataskip(auxspec[0])
			i=i+auxspec[1]
		else:
			failed=failed+1
		if(failed>2):
			failed=0
			if(abs(time.time()-inittime)>waitforspectimeout):
				if(debuglevel==1):
					print "Error! Aborting readonespectra(). Read "+str(i)+" Bytes. To read "+str(auxspec[1])+" bytes."
				if(debuglevel==2):
					print "Error! Aborting readonespectra(). Read "+str(i)+" Bytes. To read "+str(auxspec[1])+" bytes."
				break;
	imondecode.addterminator()
def error():
	if(debuglevel==1):
		print "Error! Aborting requestaquisition(). Invalid argument"
	if(debuglevel==2):
		print "Error! Aborting requestaquisition(). Invalid argument"
def requestaquisition(freq,tint,totalnspec):
	global lastusedparameters
	if(tint<3):
		error()
		return -1
	if(tint>65530):
		error()
		return -1
	if(freq>5999):
		error()
		return -1
	if(freq<10):
		error()
		return -1
	if(lastusedparameters!=[int(float(freq)),int(float(tint)),int(float(totalnspec))]):
		if(debuglevel==2):
			print "Seting parameters (freq,tint,nspec): "+str(freq)+","+str(tint)+","+str(totalnspec)
		imonrawread.write("\x2a\x50\x41\x52\x41\x3a\x46\x46\x54\x50\x41\x52\x41\x20"+str(freq)+"\x20"+str(tint)+"\x20"+str(totalnspec)+"\x0d")
		a=imonrawread.read(1)
		if(debuglevel==2):
			print "Got answer (ack) in:"+str(time.time()-inittime)+" seconds. Answer:"+str(a.encode("hex"))
		if(debuglevel==2):
			if(a.encode("hex")!="06"):
				print "Error! ack not ok."
		lastusedparameters=[int(float(freq)),int(float(tint)),int(float(totalnspec))]
		if(debuglevel==1):
			if(a.encode("hex")!="06"):
				print "Got answer (ack) in:"+str(time.time()-inittime)+" seconds. Answer:"+str(a.encode("hex"))
				print "Error! ack not ok."
	if(debuglevel==2):
		inittime=time.time()
		print "Request aquisition"
	imonrawread.write("\x2a\x4d\x45\x41\x53\x3a\x46\x53\x54\x4d\x45\x41\x53\x0d")
	return 0
def forcesenddata():
	imondecode.forcesenddata()
