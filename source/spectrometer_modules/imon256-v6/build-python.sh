cd spectrometer_modules
cd imon256-v6
export PATH=$PWD/../../Python/bin:$PATH&&export PYTHONPATH=$PWD/../../Python/lib/python2.7&&export PYTHONHOME=$PWD/../../Python/
export LD_LIBRARY_PATH=$PWD/../../Python/lib
rm build/lib.linux-x86_64-2.7/imondecode.so
rm build/temp.linux-x86_64-2.7/imon_decode.o
rm lib/python/imondecode.so
rm imondecode.so
$PWD/../../Python/bin/python setup.py install --home=./
cp lib/python/imondecode.so imondecode.so
