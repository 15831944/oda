
:@echo off

if "%ODADIR%"=="" set ODADIR=\_Oda\trunk
if "%QTDIR%"=="" set QTDIR=C:\Qt\2010.05\qt

if exist %ODADIR%\exe\vc8dlldbg\OdaMfcApp.exe set QTCFG=win32-msvc2005
if exist %ODADIR%\exe\vc9dlldbg\OdaMfcApp.exe set QTCFG=win32-msvc2008
if exist %ODADIR%\exe\vc10dlldbg\OdaMfcApp.exe set QTCFG=win32-msvc2010
: mkspec win32-msvc2012 is missing
if exist %ODADIR%\exe\vc12dlldbg\OdaMfcApp.exe set QTCFG=win32-msvc2010
if exist %ODADIR%\exe\vc8dll\OdaMfcApp.exe set QTCFG=win32-msvc2005
if exist %ODADIR%\exe\vc9dll\OdaMfcApp.exe set QTCFG=win32-msvc2008
if exist %ODADIR%\exe\vc10dll\OdaMfcApp.exe set QTCFG=win32-msvc2010
: mkspec win32-msvc2012 is missing
if exist %ODADIR%\exe\vc12dll\OdaMfcApp.exe set QTCFG=win32-msvc2010

pushd %QTDIR%

: win XP with qt v4.5.3
:configure.exe -platform win32-msvc2005 -debug-and-release -confirm-license -qt-style-windows -system-sqlite -qt-libpng -qt-libjpeg -no-openssl -opensource -no-qt3support -dont-process

: win seven with qt v4.6
:configure.exe -platform win32-msvc2005 -debug-and-release -confirm-license -qt-style-windows -qt-libpng -qt-libjpeg -no-openssl -opensource -no-qt3support -dont-process
configure.exe -platform %QTCFG% -debug-and-release -confirm-license -qt-style-windows -qt-libpng -qt-libjpeg -no-openssl -opensource -no-qt3support -dont-process

popd
