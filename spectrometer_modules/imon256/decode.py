import serial
import numpy
import random
import time
import sys
from multiprocessing import Pool

def decodeb(bytea):
	n=2 # Number of bytes to form one integer (in this case 2 because Imon sends 16 bit integers)
	spectraread=""
	if(bytea!=None):
		spectraarray=list(reversed([bytea[i:i+n] for i in range(0, len(bytea)-1, n)]))
		for i in range(1, len(spectraarray)-1 ,2 ):
			spectraread=spectraread+str(int(spectraarray[i-1]+spectraarray[i],(n*8))*10)+" "
	return (spectraread)
pb = Pool(95)
def decode(spectrabuffer):
	ab=""
	#print(spectrabuffer)
	#ab=ab+'_'.join((pb.map(decodeb, spectrabuffer)))
	return(ab)

def async_cb(error, result):
	print 'py async error: ', error, type(error)
	print 'py async result: ', result
decode.async_cb = async_cb
