@echo off
echo Clearing Release...
rmdir /S /Q Release
del /F /Q "*.VC.db"
cd bin
del /F /Q "olympus.*"
cd ../
cd "StarRender"
rmdir /S /Q bin
rmdir /S /Q Release
cd ../
cd "StarCore"
rmdir /S /Q bin
rmdir /S /Q Release
cd ../
cd lib
del /F /Q "PrometheusRender.lib"
del /F /Q "PrometheusCore.lib"
cd ../
echo Done.