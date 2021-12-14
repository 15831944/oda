:
: QT examples vcproj 2005 generation
:

:@echo off

if "%ODADIR%"=="" set ODADIR=\_Oda\trunk
if "%ODADLLSUFFIX%"=="" set ODADLLSUFFIX=_3.05src_8
if "%QTDIR%"=="" set QTDIR=C:\Qt\2010.05\qt
if "%QTMAKEPLATFORMS_FLD_NAME%"=="" set QTMAKEPLATFORMS_FLD_NAME=CMakePlatforms\QtPlatforms

set DEST=%ODADIR%\Drawing\Examples\Qt

if not "%ODACFG%"=="" goto end_def_ODACFG
if exist %ODADIR%\exe\vc8dlldbg\OdaMfcApp.exe set ODACFG=vc8dll
if exist %ODADIR%\exe\vc9dlldbg\OdaMfcApp.exe set ODACFG=vc9dll
if exist %ODADIR%\exe\vc10dlldbg\OdaMfcApp.exe set ODACFG=vc10dll
if exist %ODADIR%\exe\vc12dlldbg\OdaMfcApp.exe set ODACFG=vc12dll
if exist %ODADIR%\exe\vc8dll\OdaMfcApp.exe set ODACFG=vc8dll
if exist %ODADIR%\exe\vc9dll\OdaMfcApp.exe set ODACFG=vc9dll
if exist %ODADIR%\exe\vc10dll\OdaMfcApp.exe set ODACFG=vc10dll
if exist %ODADIR%\exe\vc12dll\OdaMfcApp.exe set ODACFG=vc12dll
:end_def_ODACFG

:--------------------

xcopy /S /Y make_vcproj.bat %DEST%\OdaQtOpenRL\*.* >nul:
pushd %DEST%\OdaQtOpenRL

set TD_CONF_NAME=%ODACFG%dbg
set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%dbg\ProjectFiles\Examples\Qt
set PRJ2SRC_LEV_OFFSET=../../
if not exist %PRJDEST% mkdir %PRJDEST%

call make_vcproj OdaQtOpenRLDebug %PRJDEST%\

set TD_CONF_NAME=%ODACFG%
set PRJDEST=%ODADIR%\%QTMAKEPLATFORMS_FLD_NAME%\%ODACFG%\ProjectFiles\Examples\Qt
set PRJ2SRC_LEV_OFFSET=../../
if not exist %PRJDEST% mkdir %PRJDEST%

call make_vcproj OdaQtOpenRLRelease %PRJDEST%\

popd 
del %DEST%\OdaQtOpenRL\make_vcproj.bat

:--------------------
