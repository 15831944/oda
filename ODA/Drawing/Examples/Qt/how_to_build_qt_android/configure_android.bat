:@echo off

:: Parameter %1 is the android platform configuration
set ODACFG=%1
if "%ODACFG%" == "" goto print_usage

set ANDROID_ARCH=android_armv5
if "%ODACFG:~17,19%" == "v7" set ANDROID_ARCH=android_armv7
echo %ANDROID_ARCH%

:: ODADIR shoud be set to as env variable 
if "%ODADIR%"=="" (
  echo "Please set up ODADIR (path to main source directory) env variable to continue building. Exiting..."
  goto :EOF
)

if "%QTDIR%"=="" (
  echo "Please set up QTDIR (path to Qt libraries) env variable to continue building. Exiting..."
  goto :EOF
)

if "%ANDROID_SDK_ROOT%"=="" (
  echo "Please set up ANDROID_SDK_ROOT env variable to continue building. Exiting..."
  goto :EOF
)

if "%ANDROID_SDK_HOME%"=="" (
  echo "Please set up ANDROID_SDK_HOME env variable to continue building. Exiting..."
  goto :EOF
)

set QT_RELEASE_DEBUG=release
if "%ODACFG:~-3%" == "dbg" set QT_RELEASE_DEBUG=debug

call :make_oda_project OdaFileConverterAndroid OdaFileConverter ODAFileConverter %QT_RELEASE_DEBUG%
call :make_oda_project OdaFileViewerAndroid OdaQtApp ODADrawingsExplorer %QT_RELEASE_DEBUG%

goto :EOF


:: Bild and create android apk
:: %1 Project file name
:: %2 Project folder name
:: %3 Product file name
:: %4 Release or debug version
:make_oda_project
pushd %ODADIR%\Drawing\Examples\Qt
:: Run qmake

%QTDIR%\%ANDROID_ARCH%\bin\qmake.exe %1.pro -r -spec android-g++
::"CONFIG+=%QT_RELEASE_DEBUG%"

:: Run make
%QTDIR%\..\Tools\mingw48_32\bin\mingw32-make.exe

:: Go to directory with build results
pushd %ODADIR%\Drawing\Examples\Qt\%2

:: Install built library
%QTDIR%\..\Tools\mingw48_32\bin\mingw32-make.exe INSTALL_ROOT=android-build install 

:: Create apk file
:: Path to ant should be set --ant D:\MinGW\msys\1.0\opt\apache-ant-1.9.1\bin\ant.bat

%QTDIR%\%ANDROID_ARCH%\bin\androiddeployqt --input android-lib%3.so-deployment-settings.json --output android-build  --ant %ANT_HOME%\bin\ant.bat

copy /Y android-build\bin\QtApp-debug.apk %ODADIR%\bin\%ODACFG%\%3.apk
rmdir /Q /S android-build
del *.obj

popd
popd


goto :EOF


:print_usage
echo Usage:
echo  %0 ^<android platform configuration name^>
echo.
goto :EOF

