import imonconnection
import time
ser=imonconnection.ser
def read(rsize):
	try:
		return ser.read(size=rsize)
	except:
		imonconnection.popen=False
		return ""
def inwaiting():
	try:
		return ser.in_waiting
	except:
		imonconnection.popen=False
		return 0
def write(data):
	try:
		ser.write(data)
	except:
		imonconnection.popen=False
def isopen():
	global ser
	a=False
	popen=imonconnection.popen
	try:
		a=ser.isOpen()
	except Exception as e:
		a=False
	if(popen==False):
		imonconnection.open()
		ser=imonconnection.ser
		popen=imonconnection.popen
		if(popen==True):
			time.sleep(2)
	return a
