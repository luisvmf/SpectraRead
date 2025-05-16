x86_64:
	util/download-frozen-image-v2.sh ubnamd ubuntu:focal-20221019
	/bin/bash -c "cd ubnamd/&&cd \$$(ls -d */|head -n 1)&&mkdir RuntimeX86&&mv layer.tar RuntimeX86/layer.tar&&cd RuntimeX86&&tar -xf layer.tar&&rm layer.tar&&cd ..&&mv RuntimeX86 ../../RuntimeX86"
	rm -rf ubnamd
	gcc -o nstest nstest.c
	mkdir RuntimeX86/put_old
	rm -rf RuntimeX86/run/
	mkdir RuntimeX86/run/
	cp fixaptcontainer RuntimeX86/fix.sh
	chmod ugo+x RuntimeX86/fix.sh
	./nstest RuntimeX86/ 0 0 0 1 "/bin/bash -c '/fix.sh'"
	tar -xvf proot-src.tar
	 cp -R proot-src/ RuntimeX86/proot
	./nstest RuntimeX86/ 0 0 0 0 "/bin/bash -c 'cd proot&&make'"
	cp RuntimeX86/proot/proot proot
	cp RuntimeX86/proot/loader-m32.elf loader-m32.elf
	cp RuntimeX86/proot/loader.elf loader.elf
	cp RuntimeX86/proot/libtallocsr.so libtallocsr.so
	mkdir RuntimeX86/opt/SpectraRead
	mkdir RuntimeX86/opt/qimon
	mkdir RuntimeX86/opt/extrac
	mkdir RuntimeX86/opt/extrab
	mkdir RuntimeX86/opt/extraa
	rm RuntimeX86/usr/lib/x86_64-linux-gnu/libLLVM-12.so.1
	rm -rf RuntimeX86/usr/lib/x86_64-linux-gnu/dri
arm:
	util/download-frozen-image-v2.sh ubnarm arm64v8/ubuntu:focal-20221019
	/bin/bash -c "cd ubnarm/&&cd \$$(ls -d */|head -n 1)&&mkdir RuntimeArm&&mv layer.tar RuntimeArm/layer.tar&&cd RuntimeArm&&tar -xf layer.tar&&rm layer.tar&&cd ..&&mv RuntimeArm ../../RuntimeArm"
	rm -rf ubnarm
riscv:
	util/download-frozen-image-v2.sh ubnriscv riscv64/ubuntu:focal-20221019
	/bin/bash -c "cd ubnriscv/&&cd \$$(ls -d */|head -n 1)&&mkdir Runtimeriscv&&mv layer.tar Runtimeriscv/layer.tar&&cd Runtimeriscv&&tar -xf layer.tar&&rm layer.tar&&cd ..&&mv Runtimeriscv ../../Runtimeriscv"
	rm -rf ubnriscv
