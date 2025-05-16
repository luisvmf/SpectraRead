all: $(OBJS)
	mkdir source/spectrometer_modules
	cd aptpackagecontainerlauncher&&make
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
	mkdir debfile
	mkdir debfile/usr
	mkdir debfile/usr/lib/
	cp -R aptpackagecontainerlauncher debfile/usr/lib/luisvmf.com-spectraread
	mkdir debfile/usr/lib/luisvmf.com-spectraread/SpectraRead
	cp -R mime-type debfile/usr/lib/luisvmf.com-spectraread/SpectraRead/mime-type
	cp -R peaksmonitor_splash_screenfiles debfile/usr/lib/luisvmf.com-spectraread/SpectraRead/peaksmonitor_splash_screenfiles
	cp -R res debfile/usr/lib/luisvmf.com-spectraread/SpectraRead/res
	cp -R source debfile/usr/lib/luisvmf.com-spectraread/SpectraRead/source
	cp -R spectraread_splash_screenfiles debfile/usr/lib/luisvmf.com-spectraread/SpectraRead/spectraread_splash_screenfiles
	cp -R spectraread.conf debfile/usr/lib/luisvmf.com-spectraread/SpectraRead/spectraread.conf
	cp -R peaks debfile/usr/lib/luisvmf.com-spectraread/SpectraRead/peaks
	cp -R spectraread debfile/usr/lib/luisvmf.com-spectraread/SpectraRead/spectraread
	cp -R spectrareads debfile/usr/lib/luisvmf.com-spectraread/SpectraRead/spectrareads
	cp -R README.md debfile/usr/lib/luisvmf.com-spectraread/SpectraRead/README.md
	cp -R package-controlfiles debfile/DEBIAN
clean:
	echo "Nothing to do."
