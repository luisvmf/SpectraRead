from distutils.core import setup, Extension
from subprocess import PIPE, Popen

def pkgconfig(*packages):
    flags = {
        '-D': 'define_macros',
        '-I': 'include_dirs',
        '-L': 'library_dirs',
        '-l': 'libraries'}
    cmd = ['pkg-config', '--cflags', '--libs'] + list(packages)
    proc = Popen(cmd, stdout=PIPE, stderr=PIPE)
    output, error = proc.stdout.read(), proc.stderr.read()

    if error:
        raise ValueError(error)

    config = {}

    for token in output.split():
        if token != '-pthread':
            flag, value = token[:2], token[2:]
            config.setdefault(flags[flag], []).append(value)

    if 'define_macros' in config:
        macros = [(name, None) for name in config['define_macros']]
        config['define_macros'] = macros

    return config

module = Extension('pygtkdatabox',
                   sources=[ 'graph.c'],
                   #extra_compile_args=['-fsanitize=address','-std=c99','-pthread','-export-dynamic','-fPIC','-L./gtkdatabox/install/lib','-lgtkdatabox','-I./gtkdatabox/install/include','-pipe','-fPIC','-W','-pedantic','-DG_DISABLE_DEPRECATED','-DGDK_DISABLE_DEPRECATED','-DGDK_PIXBUF_DISABLE_DEPRECATED','-DGTK_DISABLE_DEPRECATED','-DGSEAL_ENABLED','-DGTK_DISABLE_SINGLE_INCLUDES'],
					extra_compile_args=['-std=c99','-pthread','-export-dynamic','-fPIC','-L./gtkdatabox/install/lib','-lgtkdatabox','-I./gtkdatabox/install/include','-I../../PyGObject-3.27.1/gi','-pipe','-fPIC','-W','-pedantic','-DG_DISABLE_DEPRECATED','-DGDK_DISABLE_DEPRECATED','-DGDK_PIXBUF_DISABLE_DEPRECATED','-DGTK_DISABLE_DEPRECATED','-DGSEAL_ENABLED','-DGTK_DISABLE_SINGLE_INCLUDES'],
                   **pkgconfig('gtk+-3.0'))

setup(
    ext_modules=[module]
    # Further package description
)

