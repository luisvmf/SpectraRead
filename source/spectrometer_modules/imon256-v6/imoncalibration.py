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
from imonpreferences import *
import imontemperatureio


readtempe=imontemperatureio.readtempe
def readcalibrationcoeficients():
	def readcalibrationcoeficientsraw():
		coefstr=[""]*7
		try:
			time.sleep(eepromreadwaittime)
			imonrawread.write("\x2a\x52\x44\x55\x53\x52\x32\x20\x30\x0d")
			time.sleep(eepromreadwaittime)
			while(imonrawread.inwaiting()==0):
				null=None
			ireadtempe=0
			realreadcoef=0
			memposread=0
			while(ireadtempe<eepromsendsize):
				while(imonrawread.inwaiting()>0):
					#ireadtempe=0
					tempebyteread=imonrawread.read(1)
					if(realreadcoef>=0):
						if(realreadcoef<=15):
							memposread=0
					if(realreadcoef>=16):
						if(realreadcoef<=31):
							memposread=1
					if(realreadcoef>=32):
						if(realreadcoef<=47):
							memposread=2
					if(realreadcoef>=48):
						if(realreadcoef<=63):
							memposread=3
					if(realreadcoef>=64):
						if(realreadcoef<=79):
							memposread=4
					if(realreadcoef>=80):
						if(realreadcoef<=95):
							memposread=5
					if(realreadcoef>=96):
							memposread=6
					realreadcoef=realreadcoef+1
					coefstr[memposread]=coefstr[memposread]+tempebyteread
					if(realreadcoef>eepromsendsize):
						ireadtempe=eepromsendsize+1
						break
				ireadtempe=ireadtempe+1
			del coefstr[6]
			ireadtempe=0
			while(ireadtempe<6):
				coefstr[ireadtempe]=float(coefstr[ireadtempe])
				ireadtempe=ireadtempe+1
		except:
			null=None
		return coefstr
	ireadrawtest=0
	lastread=[]*6
	while(imonrawread.inwaiting()>0):
		null=imonrawread.read(imonrawread.inwaiting())
		if(debuglevel==1):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 1"
		if(debuglevel==2):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 1"
	while(imonrawread.inwaiting()>0):
		null=imonrawread.read(imonrawread.inwaiting())
		if(debuglevel==1):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 2"
		if(debuglevel==2):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 2"
	lastread=readcalibrationcoeficientsraw()
	while(imonrawread.inwaiting()>0):
		null=imonrawread.read(imonrawread.inwaiting())
		if(debuglevel==1):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 3"
		if(debuglevel==2):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 3"
	while(imonrawread.inwaiting()>0):
		null=imonrawread.read(imonrawread.inwaiting())
		if(debuglevel==1):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 4"
		if(debuglevel==2):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 4"
	testreadaaa=readcalibrationcoeficientsraw()
	while(imonrawread.inwaiting()>0):
		null=imonrawread.read(imonrawread.inwaiting())
		if(debuglevel==1):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 5"
		if(debuglevel==2):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 5"
	if(lastread!=testreadaaa):
		testreadaaab=readcalibrationcoeficientsraw()
		if(debuglevel==1):
			print "Error! Eeprom data not match: "+str(lastread)+" :: "+str(testreadaaa)
		if(debuglevel==2):
			print "Error! Eeprom data not match: "+str(lastread)+" :: "+str(testreadaaa)
		while(imonrawread.inwaiting()>0):
			null=imonrawread.read(imonrawread.inwaiting())
			if(debuglevel==1):
				print "Error! Buffer not empty. Flushing buffer before eeprom read 6"
			if(debuglevel==2):
				print "Error! Buffer not empty. Flushing buffer before eeprom read 6"
		if(testreadaaab==testreadaaa):
			lastread=testreadaaa
	while(imonrawread.inwaiting()>0):
		null=imonrawread.read(imonrawread.inwaiting())
		if(debuglevel==1):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 7"
		if(debuglevel==2):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 7"
	time.sleep(eepromextrabytewaittime)
	while(imonrawread.inwaiting()>0):
		null=imonrawread.read(imonrawread.inwaiting())
		if(debuglevel==1):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 8"
		if(debuglevel==2):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 8"
	if(debuglevel==2):
		print lastread
	return lastread
def readcalibrationcoeficientstemperature():
	def readcalibrationcoeficientstemperatureraw():
		coefstr=[""]*5
		try:
			time.sleep(eepromreadwaittime)
			imonrawread.write("\x2a\x52\x44\x55\x53\x52\x32\x20\x31\x0d")
			time.sleep(eepromreadwaittime)
			while(imonrawread.inwaiting()==0):
				null=None
			ireadtempe=0
			realreadcoef=0
			memposread=0
			while(ireadtempe<eepromsendsize):
				while(imonrawread.inwaiting()>0):
					ireadtempe=0
					tempebyteread=imonrawread.read(1)
					if(realreadcoef>=0):
						if(realreadcoef<=15):
							memposread=0
					if(realreadcoef>=16):
						if(realreadcoef<=31):
							memposread=1
					if(realreadcoef>=32):
						if(realreadcoef<=47):
							memposread=2
					if(realreadcoef>=48):
						if(realreadcoef<=63):
							memposread=3
					if(realreadcoef>=64):
							memposread=4
					realreadcoef=realreadcoef+1
					coefstr[memposread]=coefstr[memposread]+tempebyteread
					if(realreadcoef>eepromsendsize):
						ireadtempe=eepromsendsize+1
						break
				ireadtempe=ireadtempe+1
			del coefstr[4]
			ireadtempe=0
			while(ireadtempe<5):
				coefstr[ireadtempe]=float(coefstr[ireadtempe])
				ireadtempe=ireadtempe+1
		except:
			null=None
		return coefstr
	ireadrawtest=0
	lastread=[]*6
	while(imonrawread.inwaiting()>0):
		null=imonrawread.read(imonrawread.inwaiting())
		if(debuglevel==1):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 9"
		if(debuglevel==2):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 9"
	while(imonrawread.inwaiting()>0):
		null=imonrawread.read(imonrawread.inwaiting())
		if(debuglevel==1):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 10"
		if(debuglevel==2):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 10"
	lastread=readcalibrationcoeficientstemperatureraw()
	while(imonrawread.inwaiting()>0):
		null=imonrawread.read(imonrawread.inwaiting())
		if(debuglevel==1):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 11"
		if(debuglevel==2):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 11"
	while(imonrawread.inwaiting()>0):
		null=imonrawread.read(imonrawread.inwaiting())
		if(debuglevel==1):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 12"
		if(debuglevel==2):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 12"
	testreadaaa=readcalibrationcoeficientstemperatureraw()
	if(lastread!=testreadaaa):
		if(debuglevel==1):
			print "Error! Eeprom data not match: "+str(lastread)+" :: "+str(testreadaaa)
		if(debuglevel==2):
			print "Error! Eeprom data not match: "+str(lastread)+" :: "+str(testreadaaa)
		while(imonrawread.inwaiting()>0):
			null=imonrawread.read(imonrawread.inwaiting())
			if(debuglevel==1):
				print "Error! Buffer not empty. Flushing buffer before eeprom read 13"
			if(debuglevel==2):
				print "Error! Buffer not empty. Flushing buffer before eeprom read 13"
		testreadaaab=readcalibrationcoeficientstemperatureraw()
		while(imonrawread.inwaiting()>0):
			null=imonrawread.read(imonrawread.inwaiting())
			if(debuglevel==1):
				print "Error! Buffer not empty. Flushing buffer before eeprom read 14"
			if(debuglevel==2):
				print "Error! Buffer not empty. Flushing buffer before eeprom read 14"
		if(testreadaaab==testreadaaa):
			lastread=testreadaaa
	while(imonrawread.inwaiting()>0):
		null=imonrawread.read(imonrawread.inwaiting())
		if(debuglevel==1):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 15"
		if(debuglevel==2):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 15"
	time.sleep(eepromextrabytewaittime)
	while(imonrawread.inwaiting()>0):
		null=imonrawread.read(imonrawread.inwaiting())
		if(debuglevel==1):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 16"
		if(debuglevel==2):
			print "Error! Buffer not empty. Flushing buffer before eeprom read 16"
	if(debuglevel==2):
		print lastread
	return lastread
def getrawxarray(calcoefglobala,calcoefglobalb,temp):
	ipixelraw=0
	wavelength=[0]*256
	try:
		while(ipixelraw<256):
			wavelength[ipixelraw]=(float((calcoefglobala[0]+calcoefglobala[1]*ipixelraw+calcoefglobala[2]*ipixelraw*ipixelraw+calcoefglobala[3]*ipixelraw*ipixelraw*ipixelraw+calcoefglobala[4]*ipixelraw*ipixelraw*ipixelraw*ipixelraw+calcoefglobala[5]*ipixelraw*ipixelraw*ipixelraw*ipixelraw*ipixelraw)-(calcoefglobalb[2]*temp+calcoefglobalb[3])))/(float(1.0+calcoefglobalb[0]*temp+calcoefglobalb[1]))
			ipixelraw=ipixelraw+1
		return wavelength
	except:
		print "Error on getrawxarray"
iswavelengthcalibrated=0
calcoefglobala=[]
calcoefglobalb=[]
lastreadtempedatatimestamp=time.time()
globaltempe=-275
def getxaxiscalibrateddata():
	try:
		global globaltempe
		global lastreadtempedatatimestamp
		global calcoefglobala
		global calcoefglobalb
		global iswavelengthcalibrated
		if(iswavelengthcalibrated==0):
			calcoefglobala=readcalibrationcoeficients()
			calcoefglobalb=readcalibrationcoeficientstemperature()
			iswavelengthcalibrated=1
		if(globaltempe==-275):
			imontemperatureio.imonrawread=imonrawread
			if(debuglevel==2):
				print "Reading temperature"
			globaltempe=readtempe()
			if(debuglevel==2):
				print "Decoded temperature: "+str(globaltempe)
			lastreadtempedatatimestamp=time.time()
		if(abs(time.time()-lastreadtempedatatimestamp)>15):
			if(debuglevel==2):
				print "Reading temperature"
			globaltempe=readtempe()
			if(debuglevel==2):
				print "Decoded temperature: "+str(globaltempe)
			lastreadtempedatatimestamp=time.time()
		return [getrawxarray(calcoefglobala,calcoefglobalb,globaltempe),globaltempe]
	except:
		print "Error on getxaxiscalibrateddata"
