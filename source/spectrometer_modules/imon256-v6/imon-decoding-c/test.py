
import fastmmap
import time
import imonio

imonio.initdecodingmmap("a")
imonio.sendcalibrationdata()
imonio.requestaquisition(10,5,3)
imonio.readonespectra()
imonio.forcesenddata()
mmap=fastmmap.connectmmap("python","imdeca")
mmapb=fastmmap.connectmmap("python","funcwavea")
print fastmmap.read(mmap,0)
print fastmmap.read(mmapb,0)
imonio.readonespectra()
imonio.forcesenddata()
print fastmmap.read(mmap,0)
imonio.readonespectra()
imonio.forcesenddata()


print fastmmap.read(mmap,0)
#print imondecode.gethexdata(258)
#print imondecode.gethexdata(768)
#print imondecode.gethexdata(257)
#print imondecode.gethexdata(515)
#print imondecode.gethexdata(513)
#print imondecode.gethexdata(257)
#i=0
#a="\x03\x02"
#b=""
#while(i<6000):
#	b=a.encode("hex")
#	i=i+1
#print i
