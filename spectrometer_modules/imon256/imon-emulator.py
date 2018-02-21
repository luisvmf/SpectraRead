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
import time
emulator = SerialEmulator('./ttydevice','./imon256')
readcmd=""
freq=0
nspec=0
inttime=0
while(True):
	readcmd=emulator.read()
	print("Got command: "+readcmd)
	if(readcmd=="*MEAS:TEMPE\r"):
		print("Measuring temperature")
		emulator.write(("Temperature: 22 C\r").encode('utf-8'))
	if(readcmd.find("*PARA:FFTPARA ")!=-1):
		nspec=readcmd.split(" ")[1]
		inttime=readcmd.split(" ")[2]
		freq=readcmd.split(" ")[3].split("\r")[0]
		emulator.write(("\06").encode('utf-8'))
		print("CONFIGURING "+str(nspec)+" spectrums with a frequency of "+str(freq)+" hz and a integration time of "+str(inttime)+" us")
	if(readcmd=="*MEAS:FSTMEAS\r"):
		print("MEASURING "+str(nspec)+" spectrums with a frequency of "+str(freq)+" hz and a integration time of "+str(inttime)+" us")
		i=0
		while(i<int(nspec)):
			emulator.write(("\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\03\02\11\03\02\11\03\02\11\03\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\06\07\03\02\11\03\02\11\03\02\11\03\02\11\03\00"))
			i=i+1
			time.sleep(0.000016)
		print("Acquisition is over")
	time.sleep(0.001)
