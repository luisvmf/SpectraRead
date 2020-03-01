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
#This is the Imon 256 v6.0 device emulator.
#The purpose of this file is to emulate the Imon 256 for testing without the device.
#This file is used for testing only and should respond to firmware commands used by SpectraRead
#and also emulate known bugs/limitations in Imon firmware that could affect SpectraRead (with error messages for debugging printed to the terminal).
#-------------------------------------
#-------------------------------------
#-------------------------------------
from serial_emulator import SerialEmulator
import dataexample
import datacalexample
import time
import numpy
def proc(arr):
	i=0
	for v in arr:
		arr[i]=chr(int(v/10))
		i=i+1
	return arr

emulator = SerialEmulator('./ttydevice','./imon256')
readcmd=""
freq=0
nspec=0
a=0
inttime=0
a=0
#print '\\x'.join(x.encode('hex') for x in (''.join(list(reversed(proc((numpy.random.rand(510)*1000).tolist())))))+(''.join(list(reversed(proc((numpy.random.rand(510)*0).tolist())))))   )
while(True):
	readcmd=emulator.read()
	if(readcmd!=""):
		print("Got command: "+readcmd)
	if(readcmd=="*RDUSR2 0\r"):
		print("Eprom 2 read.")
		emulator.write(datacalexample.rdrusr20c)
	if(readcmd=="*RDUSR2 1\r"):
		print("Eprom 1 read.")
		emulator.write(datacalexample.rdrusr21c)
	if(readcmd=="*MEAS:TEMPE\r"):
		print("Measuring temperature")
		emulator.write(("\x54\x65\x6D\x70\x65\x72\x61\x74\x75\x72\x65\x3A\x20\x09\x31\x39\x2E\x30\x0D\x0A"))
	if(readcmd.find("*PARA:FFTPARA ")!=-1):
		freq=readcmd.split(" ")[1]
		inttime=readcmd.split(" ")[2]
		nspec=readcmd.split(" ")[3].split("\r")[0]
		emulator.write(("\06").encode('utf-8'))
		print("CONFIGURING "+str(nspec)+" spectrums with a frequency of "+str(freq)+" hz and a integration time of "+str(inttime)+" us")
		a=0
	if(readcmd=="*MEAS:FSTMEAS\r"):
		print("MEASURING "+str(nspec)+" spectrums with a frequency of "+str(freq)+" hz and a integration time of "+str(inttime)+" us")
		i=0
		while(i<int(nspec)):
			readcmd=emulator.read()
			if(readcmd.find("ESC")!=-1):
				print "Got command: ESC"
				i=int(nspec)
			if(a==0):
				emulator.write(dataexample.corruptedfirstspec)
				a=a+1
				i=i+1
			if(i>=int(nspec)):
				break
			time.sleep(0.016)
			if(a==1):
				emulator.write(dataexample.speca)
				a=a+1
				i=i+1
			if(i>=int(nspec)):
				break
			time.sleep(0.016)
			if(a==2):
				emulator.write(dataexample.specb)
				a=a+1
				i=i+1
			if(i>=int(nspec)):
				break
			time.sleep(0.016)
			if(a==3):
				emulator.write(dataexample.specc)
				a=a+1
				i=i+1
			if(i>=int(nspec)):
				break
			time.sleep(0.016)
			if(a==4):
				emulator.write(dataexample.specd)
				a=a+1
				i=i+1
			if(i>=int(nspec)):
				break
			time.sleep(0.016)
			if(a==5):
				emulator.write(dataexample.spece)
				a=1
				i=i+1
			#emulator.write(((''.join(list(reversed(proc((numpy.random.rand(510)*1000).tolist())))))+(''.join(list(reversed(proc((numpy.random.rand(510)*0).tolist())))))))
			#i=i+1
			time.sleep(0.016)
		print("Acquisition is over")
	time.sleep(0.001)
