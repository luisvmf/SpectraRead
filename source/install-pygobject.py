import sys
import time
if not hasattr(sys, 'argv'):
	sys.argv  = ['']
time.sleep(5)
from setuptools.command import easy_install
#easy_install.main( ["-U","scipy==0.9.0"] )
time.sleep(3)
easy_install.main( ["-U","-v","sources/PyGObject-3.27.1.tar.gz"] )
time.sleep(5)
#easy_install.main( ["-U","pyserial==3.4"] )
