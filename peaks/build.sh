node node_modules/node-gyp/bin/node-gyp configure
node node_modules/node-gyp/bin/node-gyp build --arch=[x64] --target=4.4.2
cp build/Release/polfit.node polfit.node
