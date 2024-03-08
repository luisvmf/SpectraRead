# ***SpectraRead***
![alt tag](https://img.shields.io/badge/build-pass-green.svg)

SpectraRead is a program for data acquisition of devices that generate XY data, ex spectrometers, oscilloscopes... It has peak detection and baseline removal algorithms. It can be used for high speed acquisitions and support for devices can be added easily with plugins written in Python (no need to change spectraread code, just copy plugin to spectrometer_modules folder).

The GUI uses GTK3, support is currently just for Linux.
![alt tag](https://raw.githubusercontent.com/luisvmf/SpectraRead/master/Screenshots/main.png)
Features:
- Add support for devices easily  with plugins written in Python (no need to change spectraread code, just copy plugin to spectrometer_modules folder)
- Select Plugin on runtime
- Baseline removal
- Boxcar
- Peak position fit
- Save single/multiple spectra/peaks position data
- Parallel processing/ very fast
- Linear/ Log scale for viewing
- Baseline subtract/ Scope/ Absorbance/ Transmitance mode
- Zoom/ autoscale
- Dark theme support
- Get data on real time on another process using shared memory (fastmmapmq)
- Peak position monitor

Current plugins:
- ArduinoOsciloscope (Arduino data acquisition demo - osciloscope, check https://git.luisvmf.com/Spectraread-Plugins/ArduinoOsciloscope/firmware for arduino code)
- Demo (Just generates fake data for test);
- imon256-v6
___
# ***Installing:***

## Distributions with apt (Debian, Ubuntu, Linux Mint), for tested distributions versions, check [here](https://packages.luisvmf.com/versions.html):
### Add repository:

	sudo apt-get install curl gpg
	echo "deb http://packages.luisvmf.com/ stable main" | sudo tee /etc/apt/sources.list.d/luisvmf.com.list
	curl -fsSL https://luisvmf.com/pgp-key.public | gpg --dearmor | sudo tee /etc/apt/trusted.gpg.d/luisvmfcom.gpg > /dev/null
	sudo apt-get update
### Install:

	sudo apt-get install luisvmf.com-spectraread 

### Install required plugins:

    sudo apt-get install luisvmf.com-spectraread-demoplugin
    sudo apt-get install luisvmf.com-spectraread-imonv6plugin
    sudo apt-get install luisvmf.com-spectraread-arduinoosciloscope
Now spectraread should be available on menu.

___

### Requirements to run spectraread (***should be enabled by default on recent distributions***):

        procfs mounted with support for /proc/pid/fd
        x86_64 Linux Kernel with user namespace support:
            CONFIG_USER_NS
            One of the folowing (depending on distribution):
                 /proc/sys/kernel/userns_restrict is zero
                 sysctl user.max_user_namespaces larger than 10
                 sudo sysctl -w kernel.unprivileged_userns_clone=1
        /dev/shm mounted


___

### ***Building from source***:
It is recommended to install from apt repository as above. To build from source:

      git clone https://git.luisvmf.com/Namespace
      cd Namespace
      make
      cp nstest ../nstest
      cd ..
      Get demo containner: https://github.com/luisvmf/Namespace/releases/tag/2 file Runtime-build.tar and extract
      cd Runtime
      git clone https://git.luisvmf.com/SpectraRead
      cd ..
      ./nstest Runtime/ 1 1 1 1 /bin/bash
      Now inside container:
          mkdir $HOME
	  cd SpectraRead
          make
      test:
        ./spectraread
      Now spectraread should open, but there are no devices to select on device combobox
      Clone plugins from Spectraread-Plugins repository and build (if required), inside source/spectrometer_modules folder.
      Folder debfile contains directory structure for creating .deb package. It requires package luisvmf.com-runtimelibs which puts contents of https://github.com/luisvmf/Namespace/releases/tag/2 file Runtime-run.tar on /usr/lib/luisvmf.com-Runtimelibs (/usr/lib/luisvmf.com-Runtimelibs/Runtime).

### TODO:
- Plugins development docs
- Peaks Gui
- Not Working if path name of install folder is too big, probably because of fastmmapmq.
- Generic plugin


![alt tag](https://raw.githubusercontent.com/luisvmf/SpectraRead/master/Screenshots/main.png)

![alt tag](https://raw.githubusercontent.com/luisvmf/SpectraRead/master/Screenshots/log-scale.png)

![alt tag](https://raw.githubusercontent.com/luisvmf/SpectraRead/master/Screenshots/help.png)

![alt tag](https://raw.githubusercontent.com/luisvmf/SpectraRead/master/Screenshots/dark-theme.png)

![alt tag](https://raw.githubusercontent.com/luisvmf/SpectraRead/master/Screenshots/peaksgui.png)

![alt tag](https://raw.githubusercontent.com/luisvmf/SpectraRead/master/Screenshots/save1.png)


![alt tag](https://raw.githubusercontent.com/luisvmf/SpectraRead/master/Screenshots/save2.png)


![alt tag](https://raw.githubusercontent.com/luisvmf/SpectraRead/master/Screenshots/save3.png)
