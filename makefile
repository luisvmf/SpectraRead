all:
	cp -R mime-type src/mime-type
	cp -R package-controlfiles src/package-controlfiles
	cd SpectraRead-Runtimelibs-main&&make
	cp -R src SpectraRead-Runtimelibs-main/RuntimeX86/src
	./nstest RuntimeX86/ 1 0 0 0 /bin/bash
