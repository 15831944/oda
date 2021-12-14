:@echo off

:if "%ODADIR%"=="" set ODADIR=E:\_Oda\prod_3.4.1_02112011_vc10
:if "%QTDIR%"=="" set QTDIR=C:\Qt\4.7.1
if "%ODADIR%"=="" set ODADIR=\_Oda\trunk
if "%QTDIR%"=="" set QTDIR=C:\Qt\2010.05\qt

if not "%QTCFG%"=="" goto end_def_QTCFG
if exist %ODADIR%\exe\vc8dlldbg\OdaMfcApp.exe set QTCFG=win32-msvc2005
if exist %ODADIR%\exe\vc9dlldbg\OdaMfcApp.exe set QTCFG=win32-msvc2008
if exist %ODADIR%\exe\vc10dlldbg\OdaMfcApp.exe set QTCFG=win32-msvc2010
if exist %ODADIR%\exe\vc12dlldbg\OdaMfcApp.exe set QTCFG=win32-msvc2010
if exist %ODADIR%\exe\vc8dll\OdaMfcApp.exe set QTCFG=win32-msvc2005
if exist %ODADIR%\exe\vc9dll\OdaMfcApp.exe set QTCFG=win32-msvc2008
if exist %ODADIR%\exe\vc10dll\OdaMfcApp.exe set QTCFG=win32-msvc2010
if exist %ODADIR%\exe\vc12dll\OdaMfcApp.exe set QTCFG=win32-msvc2010
:end_def_QTCFG

set VCPRJEXT=vcproj
if "%QTCFG%"=="win32-msvc2010" set VCPRJEXT=vcxproj
if "%QTCFG%"=="win32-msvc2012" set VCPRJEXT=vcxproj

if not "%MSVCBIN%"=="" goto end_def_MSVCBIN
 : attention - relative paths is useless for QMAKE ("%VS80COMNTOOLS%..\..\VC\bin")
if exist %ODADIR%\exe\vc8dlldbg\OdaMfcApp.exe set MSVCBIN=%ProgramFiles%\Microsoft Visual Studio 8\VC\BIN
if exist %ODADIR%\exe\vc9dlldbg\OdaMfcApp.exe set MSVCBIN=%ProgramFiles%\Microsoft Visual Studio 9.0\VC\BIN
if exist %ODADIR%\exe\vc10dlldbg\OdaMfcApp.exe set MSVCBIN=%ProgramFiles%\Microsoft Visual Studio 10.0\VC\BIN
if exist %ODADIR%\exe\vc12dlldbg\OdaMfcApp.exe set MSVCBIN=%ProgramFiles%\Microsoft Visual Studio 12.0\VC\BIN
if exist %ODADIR%\exe\vc8dll\OdaMfcApp.exe set MSVCBIN=%ProgramFiles%\Microsoft Visual Studio 8\VC\BIN
if exist %ODADIR%\exe\vc9dll\OdaMfcApp.exe set MSVCBIN=%ProgramFiles%\Microsoft Visual Studio 9.0\VC\BIN
if exist %ODADIR%\exe\vc10dll\OdaMfcApp.exe set MSVCBIN=%ProgramFiles%\Microsoft Visual Studio 10.0\VC\BIN
if exist %ODADIR%\exe\vc12dll\OdaMfcApp.exe set MSVCBIN=%ProgramFiles%\Microsoft Visual Studio 12.0\VC\BIN
:end_def_MSVCBIN

::set QMAKESPEC=win32-msvc2005
::set PATH=C:\Program Files\Microsoft Visual Studio 8\VC\bin;%PATH%
set QMAKESPEC=%QTCFG%
: resolve QMAKE warning about current_version of VC :
set PATH=%MSVCBIN%;%PATH%

%QTDIR%\bin\qmake.exe -t vcapp -o qsed.%VCPRJEXT% qsed.pro

call "%MSVCBIN%\..\..\Common7\Ide\devenv.com" "qsed.%VCPRJEXT%" /rebuild
