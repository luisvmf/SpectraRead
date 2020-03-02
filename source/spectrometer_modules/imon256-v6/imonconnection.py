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

import serial
from imonpreferences import *
ser=None
popen=True
try:
	if(isusingimonemulator==1):
		ser = serial.Serial('/home/luisvmf/Desktop/spectraread-git-workhere/spectraread/imon-decoding-c/imon-new-emulator/imon256', 9210600,timeout=serialrawtimeout,parity=serial.PARITY_NONE,bytesize=serial.EIGHTBITS,stopbits=serial.STOPBITS_ONE, rtscts=True, dsrdtr=True)
	if(isusingimonemulator==0):
		ser = serial.Serial('/dev/imon256v6', 115200,timeout=serialrawtimeout,parity=serial.PARITY_NONE,bytesize=serial.EIGHTBITS,stopbits=serial.STOPBITS_ONE)
except:
	null=None
	ser=None
	popen=False
def open():
	global ser
	global popen
	ser=None
	popen=True
	try:
		if(isusingimonemulator==1):
			ser = serial.Serial('/home/admin/Desktop/spectraread/imon-decoding-c/imon-new-emulator/imon256', 115200,timeout=serialrawtimeout,parity=serial.PARITY_NONE,bytesize=serial.EIGHTBITS,stopbits=serial.STOPBITS_ONE, rtscts=True, dsrdtr=True)
		if(isusingimonemulator==0):
			ser = serial.Serial('/dev/imon256v6', 115200,timeout=serialrawtimeout,parity=serial.PARITY_NONE,bytesize=serial.EIGHTBITS,stopbits=serial.STOPBITS_ONE)
	except Exception as e:
		null=None
		ser=None
		popen=False
		#print e
