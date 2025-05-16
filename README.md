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



### ***Building from source***:
	make
	This downloads a ubuntu docker container to use dependencies from, no docker daemon is required, container is downloaded with a script (SpectraRead-Runtimelibs-main/util/download-frozen-image-v2.sh).
	After build is finished directory build is created. Launch Spectraread with script build/debfile/usr/lib/luisvmf.com-spectraread/SpectraRead.sh

### TODO:
- Plugins development docs
- Peaks Gui only updates data sources on start
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
