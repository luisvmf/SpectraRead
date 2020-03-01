export PATH=$PWD/../../Python/bin:$PATH&&export PYTHONPATH=$PWD/../../Python/lib/python2.7&&export PYTHONHOME=$PWD/../../Python/
export LD_LIBRARY_PATH=$PWD/../../Python/lib
$PWD/../../spectrareadd node_modules/node-gyp/bin/node-gyp configure --target=4.4.2
$PWD/../../spectrareadd node_modules/node-gyp/bin/node-gyp build --arch=[x64] --target=4.4.2
cp build/Release/polfit.node ../polfit.node
