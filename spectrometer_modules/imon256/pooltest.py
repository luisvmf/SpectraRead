import serial
import time
ser = serial.Serial('/home/luisvmf/Desktop/spectraread-git-workhere/spectraread/spectrometer_modules/imon256/imon256', 921600,timeout=3,parity=serial.PARITY_NONE,bytesize=serial.EIGHTBITS,stopbits=serial.STOPBITS_ONE, rtscts=True, dsrdtr=True)
ser.write("*PARA:FFTPARA 1000 10 1000000\r")
time.sleep(0.3)
ser.write("*MEAS:FSTMEAS\r")
maxwai=0
i=0
while(i<10000):
	inwaiting=ser.in_waiting
	if(inwaiting>maxwai):
		maxwai=inwaiting
	aaa=ser.read(size=inwaiting)
	i=i+1
print(maxwai)
