import sys
import time
#import numpy
if not hasattr(sys, 'argv'):
	sys.argv  = ['']
time.sleep(5)
from setuptools.command import easy_install
#easy_install.main( ["-U","scipy==0.9.0"] )
time.sleep(1)
easy_install.main( ["-U","-v","sources/pyserial-3.4.tar.gz"] )
time.sleep(3)
easy_install.main( ["-U","-v","sources/pycairo-1.15.4.tar.gz"] )
time.sleep(5)
#easy_install.main( ["-U","pyserial==3.4"] )
