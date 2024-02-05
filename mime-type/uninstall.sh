cd /usr/lib/luisvmf.com-spectraread/SpectraRead/mime-type
export PATH=$PATH:$PWD/xdg-utils-1.0.2/scripts
xdg-icon-resource uninstall --context mimetypes --size 256 application-spectrareadluisvmfcom
xdg-icon-resource uninstall --context apps --size 256 application-spectrareadluisvmfcomapp
xdg-mime uninstall luisvmfcom-spectraread.xml
xdg-desktop-menu uninstall luisvmfcom-spectraread.desktop
