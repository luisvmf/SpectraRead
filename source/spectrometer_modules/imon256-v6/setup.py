from distutils.core import setup, Extension
setup(name='imondecode', version='1.0',  \
      ext_modules=[Extension('imondecode', ['imon_decode.c'])])
