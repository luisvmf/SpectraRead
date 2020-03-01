rm build/lib.linux-x86_64-2.7/imondecode.so
rm build/temp.linux-x86_64-2.7/imon_decode.o
rm lib/python/imondecode.so
rm imondecode.so
python setup.py install --home=./
cp lib/python/imondecode.so imondecode.so
