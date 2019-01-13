./node-v4.4.2 node_modules/node-gyp/bin/node-gyp configure --target=4.4.2
./node-v4.4.2 node_modules/node-gyp/bin/node-gyp build --arch=[x64] --target=4.4.2
cp build/Release/polfit.node polfit.node
