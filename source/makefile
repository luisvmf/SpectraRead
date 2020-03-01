all: $(OBJS)
	cd Python-2.7.9&&./build.sh
	cp --recursive setuptools-0.9.8 Python/bin/setuptools-0.9.8&&cd Python/bin/setuptools-0.9.8&&export PYTHONPATH=../../lib/python2.7&&export PYTHONHOME=../../&&LD_PRELOAD="../../lib/libpython2.7.so.1.0" ../python2.7 setup.py install
	./buildpynode.sh
	cp --recursive pycairo Python/include/pycairo
	./buildpyserial.sh
	cd bin_modules&&make
clean:
	echo "Nothing to do."
