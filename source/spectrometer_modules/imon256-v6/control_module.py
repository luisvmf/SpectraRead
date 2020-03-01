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

import time
import random
import sys
import os
from multiprocessing import Process,Pool
import fastmmap
from spectrareadapi import setxdata, setydata, seterr, setwarning, setinfo, pushval
def multiprocessingmainworker(instid):
	from imonpreferences import *
	import imonio
	mmapb=fastmmap.connectmmap("spectrareadd","imonv6b"+instid)
	while(mmapb==-1):
		mmapb=fastmmap.connectmmap("spectrareadd","imonv6b"+instid)
		time.sleep(0.1)
	aquisitionstatus=0 #0 if aquisition is not running.
	                   #1 if aquisition is running.
	nspectra=0 #Number of spectra measured in current aquisition.
	commanddata=["False","False","False","False","False","False"]
	timelastup=0
	initialized=0
	lastcommanddata=["null","null","null","null","null","null"]
	checkaborttime=0
	timelastabort=0
	while(True):
		if(initialized==0):
			if(imonio.isopen()==True):
				imonio.escapeaquisition()
				time.sleep(0.1)
				print "escok"
				imonio.initdecodingmmap(instid)
				print "initok"
				imonio.sendcalibrationdata()
				print "sendok"
				initialized=1
		if(imonio.isopen()==True):
			try:
				if(abs(time.time()-timelastup)>0.1):
					commanddata=fastmmap.getsharedstring(mmapb).split("-")
					timelastup=time.time()
				if(commanddata[1]=="True"):
					if(aquisitionstatus==0):
						try:
							if(initialized==0):
								if(imonio.isopen()==True):
									imonio.escapeaquisition()
									time.sleep(0.1)
									print "escok"
									imonio.initdecodingmmap(instid)
									print "initok"
									imonio.sendcalibrationdata()
									print "sendok"
									initialized=1
							imonio.sendcalibrationdata()
							tint=int(float(commanddata[3]))
							if(tint<3):
								tint=3
							if(tint>9854):
								tint=3
							freq=int(float(commanddata[2]))
							realfreq=100
							if(freq==1):
								realfreq=30
							if(freq==2):
								realfreq=100
							if(freq==3):
								realfreq=500
							if(freq==4):
								realfreq=1000
							if(freq==5):
								realfreq=2000
							if(freq==6):
								realfreq=3000
							if(freq==7):
								realfreq=4000
							if(freq==8):
								realfreq=5000
							if(freq==9):
								realfreq=5999
							null=imonio.checkchangedcommanddata(commanddata,lastcommanddata)
							nspectraonerun=realfreq*timespectraonerun
							imonio.requestaquisition(realfreq,tint,nspectraonerun)
							aquisitionstatus=1
						except Exception as e:
							print e
							null=None
						nspectra=0
					else:
						countinit=nspectra
						while(nspectra<nspectraonerun):
							if(nspectra<dropspectraafterrestart):
								imonio.readonespectra(1)
								print "drop restart"
							else:
								imonio.readonespectra(0)
								imonio.forcesenddata()
							nspectra=nspectra+1
							if(abs((float(nspectra*timespectraonerun)/float(nspectraonerun))-checkaborttime)>checkchangedparamabortinterval):
								checkaborttime=(float(nspectra*timespectraonerun)/float(nspectraonerun))
								commanddata=fastmmap.getsharedstring(mmapb).split("-")
								if(imonio.checkchangedcommanddata(commanddata,lastcommanddata)):
									if(abs(timelastabort-time.time())>abortmininterval):
										print "escape"
										imonio.escapeaquisition()
										nspectra=nspectraonerun
										imonio.forcesenddata()
										timelastabort=time.time()
							if(nspectra-countinit>500):
								break
						if(nspectra>=nspectraonerun):
							aquisitionstatus=0
				else:
					if(aquisitionstatus==1):
						#------------------------
							imonio.escapeaquisition()
							aquisitionstatus=0
						#-------------------------
					else:
						commanddata=fastmmap.getsharedstring(mmapb).split("-")
						fastmmap.writesharedstring(mmapb,str(commanddata[0])+"-"+str(commanddata[1])+"-"+str(commanddata[2])+"-"+str(commanddata[3])+"-"+str(commanddata[4])+"-False")
						if(commanddata[5]=="True"):
							print "Driver measure single"
							time.sleep(0.07)
							imonio.sendcalibrationdata()
							time.sleep(0.07)
							nspectraonerun=50
							tint=int(float(commanddata[3]))
							if(tint<3):
								tint=3
							if(tint>9854):
								tint=3
							imonio.requestaquisition(100,tint,50)
							nspectra=0
							countinit=nspectra
							while(nspectra<nspectraonerun):
								imonio.readonespectra()
								imonio.forcesenddata()
								nspectra=nspectra+1
								if(nspectra-countinit>500):
									break
							imonio.forcesenddata()
						else:
							time.sleep(0.1)
			except Exception as e:
				print e
		else:
			commanddata=fastmmap.getsharedstring(mmapb).split("-")
			#print commanddata
			if(len(commanddata)<5):
				commanddata=["False","False","False","False","False","False"]
			if(commanddata[1]=="True"):
				print "not open"
				fastmmap.write(mmapb,"e")
			if(commanddata[5]=="True"):
				print "not open"
				fastmmap.write(mmapb,"e")
				fastmmap.writesharedstring(mmapb,str(commanddata[0])+"-"+str(commanddata[1])+"-"+str(commanddata[2])+"-"+str(commanddata[3])+"-"+str(commanddata[4])+"-False")
			time.sleep(0.1)

mmapb=-1
mmapc=-1
mmapd=-1
instid=-1
lastsingle=0
queue=[]
timelastcal=0
connectedfinal=0
mmaperror=-1
errorcurrent=0
npix=256#256
strxdata="256 255 254 253 252 251 250 249 248 247 246 245 244 243 242 241 240 239 238 237 236 235 234 233 232 231 230 229 228 227 226 225 224 223 222 221 220 219 218 217 216 215 214 213 212 211 210 209 208 207 206 205 204 203 202 201 200 199 198 197 196 195 194 193 192 191 190 189 188 187 186 185 184 183 182 181 180 179 178 177 176 175 174 173 172 171 170 169 168 167 166 165 164 163 162 161 160 159 158 157 156 155 154 153 152 151 150 149 148 147 146 145 144 143 142 141 140 139 138 137 136 135 134 133 132 131 130 129 128 127 126 125 124 123 122 121 120 119 118 117 116 115 114 113 112 111 110 109 108 107 106 105 104 103 102 101 100 99 98 97 96 95 94 93 92 91 90 89 88 87 86 85 84 83 82 81 80 79 78 77 76 75 74 73 72 71 70 69 68 67 66 65 64 63 62 61 60 59 58 57 56 55 54 53 52 51 50 49 48 47 46 45 44 43 42 41 40 39 38 37 36 35 34 33 32 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1"
def mainloop(meas,single,par):
	global multiproccessingpool
	global mmapb
	global mmapc
	global mmapd
	global queue
	global timelastcal
	global strxdata
	global lastsingle
	global connectedfinal
	global instid
	global mmaperror
	global errorcurrent
	if(single==True):
		errorcurrent=0
	if(len(fastmmap.read(mmapb,0).split("e"))>1):
		if(errorcurrent==0):
			print "detected error"
			seterr("Error. Port not open.")
			fastmmap.writesharedstring(mmapb,"no-"+str(False)+"-"+str(par[0])+"-"+str(par[1])+"-"+str(par[2])+"-"+str(single))
			errorcurrent=time.time()
			return pushval()
		else:
			if(abs(time.time()-errorcurrent)>5):
				errorcurrent=0
			return False
	if(connectedfinal==0):
		fastmmap.writesharedstring(mmapb,"no-"+str(meas)+"-"+str(par[0])+"-"+str(par[1])+"-"+str(par[2])+"-"+str(single))
		if(mmapc==-1):
			mmapc=fastmmap.connectmmap("spectrareadd","imdec"+instid)
			time.sleep(0.1)
		if(mmapc==-1):
			return False
		if(mmapd==-1):
			mmapd=fastmmap.connectmmap("spectrareadd","funcwave"+instid)
			time.sleep(0.1)
		if(mmapd==-1):
			return False
		connectedfinal=1
	if(abs(time.time()-timelastcal)>0.1):
		fastmmap.writesharedstring(mmapb,"no-"+str(meas)+"-"+str(par[0])+"-"+str(par[1])+"-"+str(par[2])+"-"+str(single))
		dataarr=[[0],"---"]
		while(dataarr==[[0],"---"]):
			try:
				dataarr=eval(fastmmap.getsharedstring(mmapd))
			except:
				null=None
		if(len(dataarr[0])>100):
			strxdata=' '.join(str(x) for x in dataarr[0])
			v=strxdata
			while(v[0]==" "):
				v=v[1:]
			datvect=v.split(" ")
			datastr=""
			i=0
			#print datvect
			while(i<npix):
				if(i>1):
					datastr=datastr+" "+datvect[i]
				i=i+1
			while(datastr[0]==" "):
				datastr=datastr[1:]
			strxdata=datastr
		setinfo(u"CCD temperature: "+str(dataarr[1])+u" \u00B0C")
		timelastcal=time.time()
	if(single==True):
		if(meas==False):
			fastmmap.writesharedstring(mmapb,"no-"+str(meas)+"-"+str(par[0])+"-"+str(par[1])+"-"+str(par[2])+"-"+str(single))
			setxdata(strxdata)
			a=""
			while(len(a)<25):
				a=fastmmap.read(mmapc,0)
				time.sleep(0.1)
			a=a.split("?")
			i=0
			for v in a:
				try:
					if(i<5):
						while(v[0]==" "):
							v=v[1:]
						datvect=v.split(" ")
						datastr=""
						i=0
						#print datvect
						while(i<npix):
							if(i>1):
								datastr=datastr+" "+datvect[i]
							i=i+1
						while(datastr[0]==" "):
							datastr=datastr[1:]
						setydata(datastr)
					i=i+1
				except:
					null=None
			return pushval()
	if(meas==True) or (single==True):
		setxdata(strxdata)
		a=fastmmap.read(mmapc,0)
		#print a[len(a)-2]
		#del a[len(a)-1]
		while(len(a)<10) and (len(queue)<1):
			time.sleep(0.02)
			a=a+fastmmap.read(mmapc,0)
		a=a.split("?")
		for v in a:
			try:
				while(v[0]==" "):
					v=v[1:]
				datvect=v.split(" ")
				datastr=""
				i=0
				while(i<npix):
					if(i>1):
						datastr=datastr+" "+datvect[i]
					i=i+1
				while(datastr[0]==" "):
					datastr=datastr[1:]
				queue.append(datastr)
			except Exception as e:
				null=None
				#print e
		try:
			setydata(queue[0])
			del(queue[0])
		except:
			return False
		return pushval()
	else:
		time.sleep(0.3)
		return False
def load(instidb,devidfinal):
	global mmapb
	global mmapc
	global mmapd
	global p2
	global instid
	global mmaperror
	global mutex
	instid=instidb
	print("Debug:Loading imon256 control module"+str(devidfinal))
	p2=Process(target=multiprocessingmainworker, args=(instid,))
	p2.start()
	if(mmapb==-1):
		mmapb=fastmmap.createmmap("imonv6b"+instid,"rwx------")
def unload(instid):
	global p2
	print("Debug:Unloading imon256 control module")
