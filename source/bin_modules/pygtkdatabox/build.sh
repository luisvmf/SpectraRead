cd pygtkdatabox
cd gtkdatabox
export PATH=$PWD/../../../Python/bin:$PATH&&export PYTHONPATH=$PWD/../../../Python/lib/python2.7&&export PYTHONHOME=$PWD/../../../Python/
export LD_LIBRARY_PATH=$PWD/../../../Python/lib
./autogen.sh
./configure --prefix=$PWD/install
make
make install
cd ..
export PKG_CONFIG_PATH=$PWD/../../:$PKG_CONFIG_PATH
export PATH=$PWD/../../Python/bin:$PATH&&export PYTHONPATH=$PWD/../../Python/lib/python2.7&&export PYTHONHOME=$PWD/../../Python/
export LD_LIBRARY_PATH=$PWD/../../Python/lib
$PWD/../../Python/bin/python setup.py install --home=./
cp build/*/pygtkdatabox.so pygtkdatabox.so
cp build/*/pygtkdatabox.so ../pygtkdatabox.so
