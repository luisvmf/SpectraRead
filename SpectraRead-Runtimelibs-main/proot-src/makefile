all:
	cd proot-5.4.0/talloc-2.4.3&&./configure --disable-rpath --disable-python
	cd proot-5.4.0/talloc-2.4.3&&make
	cp proot-5.4.0/talloc-2.4.3/bin/default/libtalloc.so proot-5.4.0/libtallocsr.so
	cp proot-5.4.0/talloc-2.4.3/bin/default/libtalloc.so proot-5.4.0/src/libtallocsr.so
	cd proot-5.4.0&&make -C src loader.elf loader-m32.elf build.h
	cd proot-5.4.0&&make -C src proot
	cp proot-5.4.0/libtallocsr.so libtallocsr.so 
	cp proot-5.4.0/src/proot proot
	cp proot-5.4.0/src/loader.elf loader.elf
	cp proot-5.4.0/src/loader-m32.elf loader-m32.elf
