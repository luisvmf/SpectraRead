#import serial
import numpy
#import peakutils
import random
import time
import sys
from multiprocessing import Process, Manager, Value, Array
from ctypes import c_char_p, c_int

def replace_all(text, dic):
	for i, j in dic.iteritems():
		text = text.replace(i, j)
	return text



def measure(a,boxcar,baseline,postprocess):
	from multiprocessing import Process, Manager, Value, Array
	from ctypes import c_char_p, c_int
	import time
	import serial
	import re
	ser = serial.Serial('/dev/ttyUSB0', 921600,timeout=0,parity=serial.PARITY_NONE,bytesize=serial.EIGHTBITS,stopbits=serial.STOPBITS_ONE)
	while True:
			nspec=300
			ser.write("*PARA:FFTPARA 3000 30 "+str(nspec)+"\r")
			time.sleep(0.5)
			ser.write("*MEAS:FSTMEAS\r")
			bytesread=0
			bytea=""
			bytesreadcurrent=0
			errtest=0
			while (bytesread<nspec*(510)*2+1):
				errtest=errtest+1
				if (ser.in_waiting):
					errtest=0
					bytesread=bytesread+ser.in_waiting
					bytea=bytea+ser.read(size=ser.in_waiting)
					bytesreadcurrent=bytesread
					print(bytesread)
				if(errtest>15000):
					print("f")
					break
			a.value=a.value+('_'.join(re.findall('.{1,3078}',' '.join(re.findall('.{1,2}', ''+bytea.encode("hex"))))))

manager = Manager()
a = manager.Value(c_char_p, "")
boxcar = manager.Value(c_int, 1)
baseline = manager.Value(c_int, 7)
postprocess = manager.Value(c_int, 0)
p1=Process(target=measure, args=(a,boxcar,baseline,postprocess, ))
p1.start()



def spectrumqueuelength(a):
	while(True):
		ghjkjg=a.value.split("_")
		if(len(ghjkjg)>500):
			a.value=""+ghjkjg[len(ghjkjg)-3]
			print("Dropping processed spectrum")
		time.sleep(3)

p2=Process(target=spectrumqueuelength, args=(a, ))
p2.start()

#time.sleep(0.0001)
#p1.terminate()
def closethread():
	print("Terminating data acquisition thread")
	p1.terminate()
	p2.terminate()
#	p3.terminate()
	print("Terminated")
	print("Terminating manager acquisition thread")
	manager.shutdown()
	print("Terminated")
	sys.exit()
def acquiredata(boxcarf,baselinef,postprocessf):
	global a
	global boxcar
	global baseline
	boxcar.value=boxcarf
	baseline.value=baselinef
	postprocess.value=postprocessf
	c=a.value
	a.value=""
	#print(a.value)
	#print("\n\nini"+c)
	return(c)
