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

set VCPRJEXT=vcproj
if "%QTCFG%"=="win32-msvc2010" set VCPRJEXT=vcxproj
if "%QTCFG%"=="win32-msvc2012" set VCPRJEXT=vcxproj

 : attention - relative paths is useless for QMAKE ("%VS80COMNTOOLS%..\..\VC\bin")
if exist %ODADIR%\exe\vc8dlldbg\OdaMfcApp.exe set MSVCBIN=%ProgramFiles%\Microsoft Visual Studio 8\VC\BIN
if exist %ODADIR%\exe\vc9dlldbg\OdaMfcApp.exe set MSVCBIN=%ProgramFiles%\Microsoft Visual Studio 9.0\VC\BIN
if exist %ODADIR%\exe\vc10dlldbg\OdaMfcApp.exe set MSVCBIN=%ProgramFiles%\Microsoft Visual Studio 10.0\VC\BIN
if exist %ODADIR%\exe\vc12dlldbg\OdaMfcApp.exe set MSVCBIN=%ProgramFiles%\Microsoft Visual Studio 12.0\VC\BIN
if exist %ODADIR%\exe\vc8dll\OdaMfcApp.exe set MSVCBIN=%ProgramFiles%\Microsoft Visual Studio 8\VC\BIN
if exist %ODADIR%\exe\vc9dll\OdaMfcApp.exe set MSVCBIN=%ProgramFiles%\Microsoft Visual Studio 9.0\VC\BIN
if exist %ODADIR%\exe\vc10dll\OdaMfcApp.exe set MSVCBIN=%ProgramFiles%\Microsoft Visual Studio 10.0\VC\BIN
if exist %ODADIR%\exe\vc12dll\OdaMfcApp.exe set MSVCBIN=%ProgramFiles%\Microsoft Visual Studio 12.0\VC\BIN

:set QMAKESPEC=win32-msvc2005
set QMAKESPEC=%QTCFG%

: look also qt\src\corelib\global\qconfig.cpp : contains direct paths to mkspec

: resolve QMAKE warning about current_version of VC :
set PATH=%MSVCBIN%;%PATH%
::set PATH=%QTDIR%\bin;%PATH%

%QTDIR%\bin\qmake.exe -t vclib -o %2%1.%VCPRJEXT% %1.pro
