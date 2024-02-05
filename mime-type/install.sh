cd /usr/lib/luisvmf.com-spectraread/SpectraRead/mime-type
export PATH=$PATH:$PWD/xdg-utils-1.0.2/scripts
xdg-icon-resource install --context mimetypes --size 256 icon-spectrareadfileluisvmfcom-256.png application-spectrareadluisvmfcom
xdg-icon-resource install --context apps --size 256 icon-spectrareadluisvmfcom-256.png application-spectrareadluisvmfcomapp
xdg-mime install luisvmfcom-spectraread.xml
xdg-desktop-menu install luisvmfcom-spectraread.desktop
