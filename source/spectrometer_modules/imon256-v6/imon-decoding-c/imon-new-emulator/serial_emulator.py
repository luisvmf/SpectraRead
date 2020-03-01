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
import os, subprocess, serial, time

# this script lets you emulate a serial device
# the client program should use the serial port file specifed by client_port

# if the port is a location that the user can't access (ex: /dev/ttyUSB0 often),
# sudo is required

class SerialEmulator(object):
	def __init__(self, device_port='./ttydevice', client_port='./ttyclient'):
		self.device_port = device_port
		self.client_port = client_port
		cmd=['./socat','-d','-d','PTY,link=%s,raw,echo=0' % self.device_port, 'PTY,link=%s,raw,echo=0' % self.client_port]
		cmdb=['chmod','ugo+rwx' ,''+client_port]
		self.proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		time.sleep(1)
		self.procb = subprocess.Popen(cmdb, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		self.serial = serial.Serial(self.device_port, 9210600, rtscts=True, dsrdtr=True)
		self.err = ''
		self.out = ''
	def write(self, out):
		self.serial.write(out)

	def read(self):
		line = ''
		while self.serial.inWaiting() > 0:
			line += self.serial.read(1)
		return line

	def __del__(self):
		self.stop()

	def stop(self):
		self.proc.kill()
		self.out, self.err = self.proc.communicate()
