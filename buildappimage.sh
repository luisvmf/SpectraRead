cd build/debfile/usr/lib/
cp -R luisvmf.com-spectraread/ AppDir
cd AppDir
echo "#!/bin/bash">AppRun
echo "cd \$APPDIR">>AppRun
echo "./SpectraRead.sh">>AppRun
chmod ugo+x AppRun
cp ../../../../../AppImage/application-spectrareadluisvmfcomapp.svg application-spectrareadluisvmfcomapp.svg
cp ../../../../../AppImage/MyApp.desktop MyApp.desktop
cd ..
wget https://github.com/AppImage/appimagetool/releases/download/1.9.0/appimagetool-x86_64.AppImage
chmod u+x appimagetool-x86_64.AppImage
ARCH=x86_64 ./appimagetool-x86_64.AppImage AppDir/ SpectraRead.appimage
