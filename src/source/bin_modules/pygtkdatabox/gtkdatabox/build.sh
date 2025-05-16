./autogen.sh
mkdir install
./configure --prefix=$PWD/install
make
make install
