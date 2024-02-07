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
./clearprocesses
echo "Try running Spectraread in existing namespace"
./server
export PATH=/usr/kerberos/bin:/bin:/usr/bin:/usr/bin/X11:/usr/local/bin:$PATH
pidlistsrnstestluisvmfcom=$(pidof /usr/lib/luisvmf.com-spectraread/nstest)
#Here we check if Spectraread is already running and enter the namespace for running another instance, in this case exit code is different than 1 after pressing close button, and we exit.
for pidsrnstestluisvmfcom in $pidlistsrnstestluisvmfcom; do ./run-suid 1 /proc/$pidsrnstestluisvmfcom/ns/user /proc/$pidsrnstestluisvmfcom/ns/pid /proc/$pidsrnstestluisvmfcom/ns/uts /proc/$pidsrnstestluisvmfcom/ns/mnt a; if [ $? != 1 ]; then exit; break; fi; done

pidlistsrnstestluisvmfcom=$(pidof /usr/lib/luisvmf.com-qimon/nstest)
#Here we check if Spectraread is already running and enter the namespace for running another instance, in this case exit code is different than 1 after pressing close button, and we exit.
for pidsrnstestluisvmfcom in $pidlistsrnstestluisvmfcom; do ./run-suid 1 /proc/$pidsrnstestluisvmfcom/ns/user /proc/$pidsrnstestluisvmfcom/ns/pid /proc/$pidsrnstestluisvmfcom/ns/uts /proc/$pidsrnstestluisvmfcom/ns/mnt a; if [ $? != 1 ]; then exit; break; fi; done

#If we get here, we haven't run exit() yet, which means we were unable to enter a namespace because another instance of spectraread was not open, lets run spectraread...
echo "Run Spectraread creating namespace"


export PATH=/usr/kerberos/bin:/bin:/usr/bin:/usr/bin/X11:/usr/local/bin:$PATH
./run-suid 0
