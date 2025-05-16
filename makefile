all:
	cp -R mime-type src/mime-type
	cp -R package-controlfiles src/package-controlfiles
	cd SpectraRead-Runtimelibs-main&&make
	cp -R src SpectraRead-Runtimelibs-main/RuntimeX86/src
	cd SpectraRead-Runtimelibs-main&&./nstest RuntimeX86/ 1 0 0 0 "/bin/bash -c 'cd /src&&mkdir $HOME&&make'"
	mkdir build
	mv SpectraRead-Runtimelibs-main/RuntimeX86/src/debfile build/debfile
	rm -rf SpectraRead-Runtimelibs-main/RuntimeX86/src/
	mv SpectraRead-Runtimelibs-main/RuntimeX86 build/debfile/usr/lib/luisvmf.com-spectraread/RuntimeX86
	cp SpectraRead-Runtimelibs-main/proot build/debfile/usr/lib/luisvmf.com-spectraread/proot
	cp SpectraRead-Runtimelibs-main/loader-m32.elf build/debfile/usr/lib/luisvmf.com-spectraread/loader-m32.elf
	cp SpectraRead-Runtimelibs-main/loader.elf build/debfile/usr/lib/luisvmf.com-spectraread/loader.elf
	cp spectraread.conf  build/debfile/usr/lib/luisvmf.com-spectraread/SpectraRead/spectraread.conf
