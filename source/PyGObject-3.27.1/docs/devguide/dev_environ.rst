.. include:: ../icons.rst

==================================
Creating a Development Environment
==================================

This describes how to work on PyGObject itself. Please follow the instructions
on ":ref:`gettingstarted`" first, as they are a pre-requirement.


|ubuntu-logo| Ubuntu / |debian-logo| Debian
-------------------------------------------

.. code:: console

    sudo apt build-dep pygobject
    sudo apt install autoconf-archive
    git clone https://git.gnome.org/browse/pygobject
    cd pygobject
    ./autogen.sh
    make
    make check


|windows-logo| Windows
----------------------

.. code:: console

    pacman -S --needed --noconfirm base-devel mingw-w64-i686-toolchain git \
        mingw-w64-i686-python3 mingw-w64-i686-python3-cairo \
        mingw-w64-i686-gobject-introspection mingw-w64-i686-gtk3 \
        mingw-w64-i686-libffi autoconf-archive
    git clone https://git.gnome.org/browse/pygobject
    cd pygobject
    ./autogen.sh
    make
    make check


|macosx-logo| macOS
-------------------

.. code:: console

    # TODO
