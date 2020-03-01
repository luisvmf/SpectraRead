#MIT License
#
#Copyright (c) 2018 Luis Victor Muller Fabris
#
#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:
#
#The above copyright notice and this permission notice shall be included in all
#copies or substantial portions of the Software.
#
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#SOFTWARE.

isusingimonemulator=1
serialrawtimeout=0.1 #timeout in seconds, to wait for avaliable amount of bytes while reading spectra or ack byte \x06 when initiating acquisition.
waitforspectimeout=3 #timeout in seconds to abort aquisition if a full spectrum is unavaliable
debuglevel=2 #Debug level:
             #0: No message print (not recomended).
             #1: Print errors that cause freezes/crashes only
             #2: Print everything (can slow down aquisition frequency/cause data loss, for debug only/not recomended in production).
eepromsendsize=1026 #Was last tested with 1024 and was working, but is 1026 (1024+2 bytes checksum). #If there are extra bytes they are flushed latter and the "Error! Buffer not empty." message is print.
eepromreadwaittime=0.05 #In seconds. Was last tested with 0.1 seconds and was working. Might not be needed. This variable increases module load time. Eeprom is read only on module load.
eepromextrabytewaittime=0.2 #In seconds. Time to wait before checking for extra checksum bytes in case Imon sends more than "eepromsendsize" bytes. This variable increases module load time. Eeprom is read only on module load.
timespectraonerun=40 #Time for one run in seconds. This changes the number of spectra requested from Imon.
logreadbytesspec=0
checkchangedparamabortinterval=1 #In seconds. Interval to recheck for changed command from gui. If the value is small performance can be affected.
abortmininterval=2 #In seconds. Minimun interval between abort() calls.
dropspectraafterrestart=1 #Number of spectra to drop after acquisition restart.
