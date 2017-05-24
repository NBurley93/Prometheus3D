@echo off
echo Clearing Debug...
rmdir /S /Q Debug
del /F /Q "*.VC.db"
cd bin
del /F /Q "olympus.*"
cd ../
cd "StarRender"
rmdir /S /Q bin
rmdir /S /Q Debug
cd ../
cd "StarCore"
rmdir /S /Q bin
rmdir /S /Q Debug
cd ../
cd lib
del /F /Q "PrometheusRender.lib"
del /F /Q "PrometheusCore.lib"
cd ../
echo Done.