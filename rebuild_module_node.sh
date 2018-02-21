cd node_modules
cd python.node
unset PYTHONPATH
unset PYTHONHOME
LD_PRELOAD="../../Python/lib/libpython2.7.so.1.0" ../../node-v4.4.2 ../node-gyp/bin/node-gyp configure
LD_PRELOAD="../../../Python/lib/libpython2.7.so.1.0" ../../node-v4.4.2 ../node-gyp/bin/node-gyp build --arch=[x64] --target=4.4.2
