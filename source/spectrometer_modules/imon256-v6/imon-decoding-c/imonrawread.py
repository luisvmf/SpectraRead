import imonconnection
ser=imonconnection.ser
def read(rsize):
	return ser.read(size=rsize)
def inwaiting():
	return ser.in_waiting
def write(data):
	ser.write(data)
