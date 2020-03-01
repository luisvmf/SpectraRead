export PATH=$PWD/../../Python/bin:$PATH&&export PYTHONPATH=$PWD/../../Python/lib/python2.7&&export PYTHONHOME=$PWD/../../Python/
export LD_LIBRARY_PATH=$PWD/../../Python/lib
$PWD/../../Python/bin/python setup.py install --home=./
cp lib/python/fastmmap.so ../fastmmap.so
