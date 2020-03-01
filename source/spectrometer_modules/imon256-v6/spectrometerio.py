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
#
#
#
#
#
#This is the Imon 256 v6.0 device interface module for SpectraRead. 
#-------------------------------------
#-------------------------------------
#-------------------------------------
import serial
import numpy
import random
import time
import sys
import os
from multiprocessing import Pool, Process, Manager, Value, Array, Queue
import acquisitiondialog as dialog
from ctypes import c_char_p

def readprocess(intbufferprocacq):
	import serial
	import os, errno, time
	ser = serial.Serial('/home/luisvmf/Desktop/spectraread-git-workhere/spectraread/spectrometer_modules/imon256/imon256', 921600,timeout=3,parity=serial.PARITY_NONE,bytesize=serial.EIGHTBITS,stopbits=serial.STOPBITS_ONE, rtscts=True, dsrdtr=True)
	intbufferb=""
	while(True):
		intbufferb=intbufferb+ser.read()
		intbufferb=intbufferb+ser.read(size=ser.in_waiting)
		if(len(intbufferb)>40):
			intbufferprocacq.put(intbufferb)
			intbufferb=""


intbufferprocacq =  Queue()
p2=Process(target=readprocess, args=(intbufferprocacq,))
p2.start()


#-------------------------------------
#-------------------------------------
#Here are some constants used in this module.
#-------------------------------------
ser=None
isusingfakeimon=1 #Set the port using rtscts and dsrdtr and use a file in /tmp so that we don't require the imon emulator script to be root;
debugging=0 #Write processing time, CCD temperature and other usefull debug info to the terminal
#totalnspec=6000*5 #Number of spectra served by Imon every fast measurement. CCD temperature is acquired when this number of spectra is read.
totalnspec=90000
maxfailspectra=20 #Number of spectra read failures before restarting acquisition.
maxfailbytes=15000 #Number of byte read failures before spectra read fails.
PATH = "/dev/shm/imon256fifo-"+str(os.getuid());
#-------------------------------------
#-------------------------------------
#-------------------------------------
#-------------------------------------
#-------------------------------------
#-------------------------------------
#Here we have some global variables and functions to be used inside the module.
#The functions readspectra(), setcurrentoptions() and close() are defined on the end of the file and called from outside the module.
#-------------------------------------
#-------------------------------------
if(isusingfakeimon==1):
	try:
		ser = serial.Serial('/home/luisvmf/Desktop/spectraread-git-workhere/spectraread/spectrometer_modules/imon256/imon256', 921600,timeout=3,parity=serial.PARITY_NONE,bytesize=serial.EIGHTBITS,stopbits=serial.STOPBITS_ONE, rtscts=True, dsrdtr=True)
	except:
		if(debugging==1):
			print("Port not open")
else:
	try:
		ser = serial.Serial('/dev/ttyUSB0', 921600,timeout=3,parity=serial.PARITY_NONE,bytesize=serial.EIGHTBITS,stopbits=serial.STOPBITS_ONE)
	except:
		if(debugging==1):
			print("Port not open")
totalnspecread=totalnspec+1 # Number of spectra read from Imon in current fast measurement.
							#Initiate this variable with a value larger than totalnspec to trigger acquisition start.
integrationtime=30 #Default integration time. This is just for starting the program, changed later when the SpectraRead GUI loads.
acquisitionfrequency=200 #Default acquisition frequency. This is just for starting the program, changed later when the SpectraRead GUI loads.
restartacquisition=0
closeready=1
closerequested=0
starterror=False
CCDtemperature="--"
def decode(bytea):
	n=2 # Number of bytes to form one integer (in this case 2 because Imon sends 16 bit integers)
	spectraread=""
	if(bytea!=None):
		spectraarray=list(reversed([bytea[i:i+n] for i in range(0, len(bytea)-1, n)]))
		for i in range(1, len(spectraarray)-1 ,2 ):
			spectraread=spectraread+str(int(spectraarray[i-1]+spectraarray[i],(n*8))*10)+" "
	return (spectraread)
rawprereadspectrabuffer=""
def readonespectra():
	global rawprereadspectrabuffer
	global intbufferprocacq
	returnvar=""
	while(len(rawprereadspectrabuffer)<1020*4*2):
		#rawprereadspectrabuffer=rawprereadspectrabuffer+intbufferprocacq.value.encode("hex")
		rawprereadspectrabuffer=rawprereadspectrabuffer+intbufferprocacq.get().encode("hex")
		#intbufferprocacq.value=""
	nbytesinonespectra=(1020*2)
	inwaiting=0
	errtest=0
	bytesread=0
	while(bytesread<nbytesinonespectra):
		inwaiting=len(rawprereadspectrabuffer)
		errtest=errtest+1
		#print(len(rawprereadspectrabuffer))
		if(bytesread+inwaiting>nbytesinonespectra):
			inwaiting=nbytesinonespectra-bytesread #Number of bytes remaining to complete one spectra
		if (inwaiting):
			returnvar=returnvar+rawprereadspectrabuffer[0:inwaiting]
			#print("aaa"+rawprereadspectrabuffer)
			rawprereadspectrabuffer=rawprereadspectrabuffer[inwaiting:]
			#print("bbb"+rawprereadspectrabuffer)
			errtest=0
			bytesread=bytesread+inwaiting
			#print(returnvar)
		else:
			time.sleep(0.0000005)
			if(errtest>maxfailbytes/5):
				time.sleep(0.0000005)
		if(errtest>maxfailbytes):
			if(debugging==1):
				print("Warning::Failed to read spectrum, corrupted data. Bytes read in current reading specrum:"+str(bytesread))
			break
	return returnvar[0:255*4]   #Imon sends 512 values but has only 256 pixels. Last values are always 0. 
								#Cut the string. The first character shoud be ignored also because it is just to check for missing data
								#TODO:Use first byte to check for missing data and do something.

def setparameters(tintsetparameters,freqsetparameters,measuringsetparameters):
	global closerequested
	global restartacquisition
	global integrationtime
	global acquisitionfrequency
	integrationtime=tintsetparameters
	acquisitionfrequency=freqsetparameters
	if(measuringsetparameters==1):
		if(closerequested==0):
			restartacquisition=1
	if(measuringsetparameters==0):
		restartacquisition=0

#-------------------------------------
#-------------------------------------
#Here we have the functions readspectra(), setcurrentoptions() and close() called from outside the module
#-------------------------------------
#-------------------------------------
pb = Pool(95)
portclosed=0
measuring=0
firstspectra=1
def readspectra():
	global totalnspec
	global totalnspecread
	global integrationtime
	global acquisitionfrequency
	global closeready
	global closerequested
	global ser
	global restartacquisition
	global measuring
	global portclosed
	global CCDtemperature
	global starterror
	global firstspectra
	abortportnotopen=0
	try:
		testportisopen=ser.in_waiting
	except:
		abortportnotopen=1
	if(ser!=None) and (ser.is_open==True) and (abortportnotopen==0):
		if(totalnspecread>=totalnspec):
			measuring=0
			if(restartacquisition==1):
				closeready=0
				ser.flushInput()
				ser.flushOutput()
				#ser.write("*MEAS:TEMPE\r") #
				ser.flushInput()
				ser.flushOutput()
				tempmeasurementmaxerror=0
				tempmeasurementresult=""
				#while(True):
				#	if(ser.in_waiting):
				#		tempmeasurementmaxerror=0
				#		tempmeasurementresult=tempmeasurementresult+ser.read(size=ser.in_waiting)
				#	else:
				#		tempmeasurementmaxerror=tempmeasurementmaxerror+1
				#		time.sleep(0.0003)
				#	if(tempmeasurementmaxerror>550):
				#		if(debugging==1):
				#			print("Temperature measurement fail")
				#		break
				#	if(len(tempmeasurementresult)>=17):
				#		if(debugging==1):
				#			print("Temperature measurement complete")
				#		break
				#while(ser.in_waiting):
				#	fggshfsdguhsgd=""
				#if(debugging==1):
				#	print("Temp: "+tempmeasurementresult.split("\r")[0])
				CCDtemperature=0
				#CCDtemperature=tempmeasurementresult.split(" ")[1].split("\r")[0]
				ser.flushInput()
				ser.flushOutput()
				totalnspecread=0
				acquisitionparametersresponse=""
				if(debugging==1):
					print("Configuring...")
				time.sleep(0.03)
				#ser.write("*PARA:FFTPARA "+str(totalnspec)+" "+str(integrationtime)+" "+str(acquisitionfrequency)+"\r") #Set up the acquisition parameters
				ser.write("*PARA:FFTPARA 1000 "+str(int(float(integrationtime)))+" "+str(totalnspec)+"\r") 
				#while(acquisitionparametersresponse+""==""):
				#	acquisitionparametersresponse=ser.read(size=1).encode("hex") 	#Read the ack byte (\x06 = OK)
																					#TODO:Check if byte is \x06 and do something if it is not.
				if(debugging==1):
					print("Imon configuration answer: "+acquisitionparametersresponse)
				time.sleep(0.00000003)
				time.sleep(0.03)
				time.sleep(0.3)
				if(debugging==1):
					print("Starting acquisition")
				ser.write("*MEAS:FSTMEAS\r") #
				measuring=1
				firstspectra=1
		if(measuring==1):
			#nspec=900 # Number of spectra to obtain before returning data to main.js
			nspec=600 # Number of spectra to obtain before returning data to main.js
			spectraread=0 #
			spectrabuffer=[None]*2401 #
			nb = 511#
			ib=0 #
			countfail=0
			ab=""#
			spectrabufferi=0#
			if(debugging==1):
				timea=time.time()
			while (spectraread<=nspec):
				currentspectrareadprocess=readonespectra()
				if(str(currentspectrareadprocess)!=""):
					if(firstspectra<3):
						firstspectra=firstspectra+1
					countfail=0
					totalnspecread=totalnspecread+1
					spectraread=spectraread+1
					if(firstspectra>=3):
						spectrabuffer[spectrabufferi]=currentspectrareadprocess
						spectrabufferi=spectrabufferi+1
						#print("a"+spectrabuffer[spectrabufferi-1])
				else:
					countfail=countfail+1
					print(countfail)
				if(totalnspecread>=totalnspec):
					spectraread=nspec+1
					totalnspecread=totalnspec+1
				if(countfail>maxfailspectra):
					#reset the acquisition
					if(debugging==1):
						print("Nspecread:"+str(spectraread))
						print("TotalNspecread:"+str(totalnspecread))
					spectraread=nspec+1
					totalnspecread=totalnspec+1
					if(debugging==1):
						print("Error. Expected more data, but no data received... Restarting acquisition")
				#print(spectrabufferi)
				if(spectrabufferi>200): #800
					ab=ab+'_'.join((pb.map(decode, spectrabuffer)))
					#print("decoding")
					#print(ab)
					spectrabufferi=0
			if(debugging==1):
				print("done"+str(time.time()-timea))
			closeready=1
			if(ab==""):
				ab=ab+'_'.join((pb.map(decode, spectrabuffer)))
			return(ab)
		else:
			if(debugging==1):
				print("Not measuring.")
			if(closeready==1):
				if(closerequested==1):
					if(portclosed==0):
						if(debugging==1):
							print("Imon 256 v6 module is unloading.")
						ser.close()
						portclosed=1
			time.sleep(0.1)
			return ""
	else:
		if(restartacquisition==1):
			if(isusingfakeimon==1):
				try:
					ser = serial.Serial('/home/luisvmf/Desktop/spectraread-git-workhere/spectraread/spectrometer_modules/imon256/imon256', 921600,timeout=3,parity=serial.PARITY_NONE,bytesize=serial.EIGHTBITS,stopbits=serial.STOPBITS_ONE, rtscts=True, dsrdtr=True)
				except:
					if(debugging==1):
						print("Port not open")
					if(starterror==False):
						dialog.showerrormessage("Failed to open device.")
						starterror=True
			else:
				try:
					ser = serial.Serial('/dev/ttyUSB0', 921600,timeout=3,parity=serial.PARITY_NONE,bytesize=serial.EIGHTBITS,stopbits=serial.STOPBITS_ONE)
				except:
					if(debugging==1):
						print("Port not open")
					if(starterror==False):
						dialog.showerrormessage("Failed to open device.")
						starterror=True
		time.sleep(0.1)
		return ""

def getinfo():
	global CCDtemperature
	global starterror
	return [CCDtemperature,"",starterror]


def setcurrentoptions(optionssetcurrentoptions):
	global starterror
	currentoptionssetcurrentoptions=optionssetcurrentoptions.split(";")
	freqsetcurrentoptions=100
	meassetcurrentoptions=0
	#Below we decide if we start continuous acquisition mode or no.
	if(debugging==1):
		print(str(currentoptionssetcurrentoptions[3]))
	if(str(currentoptionssetcurrentoptions[3])=="True"):
		meassetcurrentoptions=1
	else:
		meassetcurrentoptions=0
		starterror=False
	#Below we decide the continuous acquisition mode frequency.
	if(str(currentoptionssetcurrentoptions[0])=="1"):
		freqsetcurrentoptions=100
	setparameters(currentoptionssetcurrentoptions[1],freqsetcurrentoptions,meassetcurrentoptions)

def close():
	global closerequested
	global restartacquisition
	print("close")
	closerequested=1
	restartacquisition=0
	pb.terminate()
	try:
		ser.close()
	except:
		print("Port not opened")



