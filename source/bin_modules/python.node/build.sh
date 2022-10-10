export PATH=$PWD/../../Python/bin:$PATH&&export PYTHONPATH=$PWD/../../Python/lib/python2.7&&export PYTHONHOME=$PWD/../../Python/
export LD_LIBRARY_PATH=$PWD/../../Python/lib
export PATH=../../Python/bin:$PATH&&export PYTHONPATH=../../Python/lib/python2.7&&export PYTHONHOME=../../Python/&&LD_PRELOAD="/home/luisvmf/Desktop/spectraread/SpectraRead/source/Python/lib/libpython2.7.so.1.0" $PWD/../../spectrareadd node_modules/node-gyp/bin/node-gyp configure --target=4.4.2
export PATH=../../Python/bin:$PATH&&export PYTHONPATH=../../Python/lib/python2.7&&export PYTHONHOME=../../Python/&&LD_PRELOAD="/home/luisvmf/Desktop/spectraread/SpectraRead/source/Python/lib/libpython2.7.so.1.0" $PWD/../../spectrareadd node_modules/node-gyp/bin/node-gyp build --arch=[x64] --target=4.4.2

