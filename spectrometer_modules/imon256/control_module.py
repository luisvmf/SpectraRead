import time
import random
from spectrareadapi import setxdata, setydata, seterr, setwarning, setinfo, pushval
from multiprocessing import Process
import fastmmap
import sys
import os
mapa=-1
mapc=-1
p2=-1
def readprocess(instid):
	print "l3"
	mapb=fastmmap.connectmmap(os.getcwd()+"/spectrareadd "+os.getcwd()+"/daemon.js spectrareadcurrentprocid:"+str(instid),"w"+str(instid))
	while(mapb==-1):
		mapb=fastmmap.connectmmap(os.getcwd()+"/spectrareadd "+os.getcwd()+"/daemon.js spectrareadcurrentprocid:"+str(instid),"w"+str(instid))
	mapd=fastmmap.connectmmap(os.getcwd()+"/spectrareadd "+os.getcwd()+"/daemon.js spectrareadcurrentprocid:"+str(instid),"z"+str(instid))
	while(mapd==-1):
		mapd=fastmmap.connectmmap(os.getcwd()+"/spectrareadd "+os.getcwd()+"/daemon.js spectrareadcurrentprocid:"+str(instid),"z"+str(instid))
	print "l3"
	tint=50
	freq=200
	meas=False
	getnewcommands=time.time()
	while(True):
		i=0
		strdata=""
		while(i<=256):
			strdata=strdata+str(((i**2)*random.random()))+" "
			i=i+1
		strdata=strdata[:-1]
		fastmmap.write(mapb,","+strdata)
		if(abs(time.time()-getnewcommands)>0.3):
			commandsread=fastmmap.read(mapd,0)
			if(commandsread.find("yes")!=-1):
				print "single"
			if(commandsread.find(";")!=-1):
				if(commandsread.find("_")!=-1):
					commandsread=commandsread.split("_")[0].split(";")
					commandsread=commandsread[len(commandsread)-1]
					commandsread=commandsread.split("-")
					if(len(commandsread)>=4):
						if(commandsread[1]=="True"):
							meas=True
						else:
							meas=False
						freq=int(commandsread[2])
						tint=float(commandsread[3])
						if(freq==1):
							freq=100
						if(freq==2):
							freq=500
						if(freq==3):
							freq=1000
						if(freq==4):
							freq=2000
						if(freq==5):
							freq=3000
						if(freq==6):
							freq=4000
						if(freq==7):
							freq=5000
						if(freq==8):
							freq=6000
		time.sleep(0.03)
		if(meas==False):
			time.sleep(0.3)

strtime=time.time()
def mainloop(meas,single,par):
	global strtime
	if(meas==True) or (single==True):
		try:
			strdata=fastmmap.read(mapa,0).split(",")[1]
			if(abs(time.time()-strtime)>0.1):
				fastmmap.write(mapc,";no-"+str(meas)+"-"+str(par[0])+"-"+str(par[1])+"-"+str(par[2])+"_")
				strtime=time.time()
			if(single==True):
				fastmmap.write(mapc,";yes-"+str(meas)+"-"+str(par[0])+"-"+str(par[1])+"-"+str(par[2])+"_")
			strdata=strdata[:-1]
			if(strdata==""):
				return False
			if(strdata==" "):
				return False
			time.sleep(0.01)#0.02 300Hz
			setxdata("0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 256")
			setydata(strdata)
			setinfo(u"CCD temperature: 89\u00B0C")
			#seterr("dhfgfd")
			#print par
		except:
			null=None
		return pushval()
	else:
		fastmmap.write(mapc,";no-"+str(meas)+"-"+str(par[0])+"-"+str(par[1])+"-"+str(par[2])+"_")
		time.sleep(0.3)
		return False
def load(instid):
	global mapa
	global mapc
	global p2
	print("Debug:Loading imon256 control module")
	p2=Process(target=readprocess, args=(instid,))
	p2.start()
	if(mapa==-1):
		mapa=fastmmap.createmmap("w"+str(instid),"rwx------")
	if(mapc==-1):
		mapc=fastmmap.createmmap("z"+str(instid),"rwx------")
def unload(instid):
	global p2
	try:
		p2.terminate()
	except:
		null=None
	print("Debug:Unloading imon256 control module")
