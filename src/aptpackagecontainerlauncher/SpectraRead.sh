#!/bin/bash

U=$USER
H=$HOME
D=$DISPLAY
P=$PATH

#for i in $(env | awk -F"=" '{print $1}') ; do
#unset $i ; done


export USER=$U
export HOME=$H
export DISPLAY=$D
export PATH=$P
cd /usr/lib/luisvmf.com-spectraread
echo "Try running Spectraread in existing namespace"
./server
export PATH=/usr/kerberos/bin:/bin:/usr/bin:/usr/bin/X11:/usr/local/bin:$PATH



export PATH=/usr/kerberos/bin:/bin:/usr/bin:/usr/bin/X11:/usr/local/bin:$PATH

LD_PRELOAD="libtallocsr.so /proot/libtallocsr.so" ./proot -r RuntimeX86/ -b /dev:/dev -b /tmp:/tmp -b /home:/home -b SpectraRead:/opt/SpectraRead -b /usr/share/fonts/:/usr/share/fonts/ -b /usr/share/themes/:/usr/share/themes/ -b /usr/share/icons/:/usr/share/icons/ -b /run:/run -b /sys:/sys -b /proc:/proc -0 /bin/bash -c 'cd /&&/opt/SpectraRead/spectraread'
