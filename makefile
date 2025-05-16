all:
	cp -R mime-type src/mime-type
	cp -R package-controlfiles src/package-controlfiles
	cd SpectraRead-Runtimelibs-main&&make
	LD_PRELOAD="SpectraRead-Runtimelibs-main/libtallocsr.so /proot/libtallocsr.so" SpectraRead-Runtimelibs-main/proot -r SpectraRead-Runtimelibs-main/RuntimeX86/ -b /dev:/dev -b /tmp:/tmp -b /home:/home -b src:/opt/SpectraRead -b /usr/share/fonts/:/usr/share/fonts/ -b /usr/share/themes/:/usr/share/themes/ -b /usr/share/icons/:/usr/share/icons/ -b /run:/run -b /sys:/sys -b /proc:/proc -0 /bin/bash -c 'cd /opt/SpectraRead/&&make'
