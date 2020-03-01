import imondecode
import fastmmap
from dataexample import data
#this is working at more than 6khz if the printf is removed from void senddata() in imon_decoding_utils.h




imondecode.addhexdata("none","a")
def addhexdataskip(hexdata):
	imondecode.addhexdata(hexdata.replace("\x01","\x01\x03").replace("\x00","\x01\x02"))
def readraw():
	return [data,1020]
def readonespectra():
	i=0
	removedsyncmarc=0
	while(i<1020):
		auxspec=readraw()
		addhexdataskip(auxspec[0])
		i=i+auxspec[1]
readonespectra()
imondecode.forcesenddata()
mmap=fastmmap.connectmmap("python","imondecodingfunca")
print fastmmap.read(mmap,0)
print imondecode.gethexdata(258)
print imondecode.gethexdata(768)
print imondecode.gethexdata(257)
print imondecode.gethexdata(515)
print imondecode.gethexdata(513)
print imondecode.gethexdata(257)
#i=0
#a="\x03\x02"
#b=""
#while(i<6000):
#	b=a.encode("hex")
#	i=i+1
#print i
