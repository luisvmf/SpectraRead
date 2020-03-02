all: $(OBJS)
	cd spectraread_splash_screenfiles&&make
	cd peaksmonitor_splash_screenfiles&&make
	cp spectraread_splash_screenfiles/spectraread spectraread
	cp spectraread_splash_screenfiles/spectrareads spectrareads
	cp peaksmonitor_splash_screenfiles/peaks peaks
	cp peaksmonitor_splash_screenfiles/peakss source/peakss
	cp spectraread source/spectraread
	cp spectrareads source/spectrareads
	cp peaks source/peaks
	cd source&&make
clean:
	echo "Nothing to do."
