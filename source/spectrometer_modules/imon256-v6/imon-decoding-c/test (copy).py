import imondecode
import fastmmap
#this is working at more than 6khz if the printf is removed from void senddata() in imon_decoding_utils.h
imondecode.addhexdata("none","a")
def addhexdataskip(hexdata):
	imondecode.addhexdata(hexdata.replace("\x01","\x01\x03").replace("\x00","\x01\x02"))
i=0
while(i<2560):
	addhexdataskip("\x01\x02\x03\x00\x01\x01\x02\x03\x02\x01\x01\x01")
	i=i+1
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
